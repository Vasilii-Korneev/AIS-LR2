#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <deque>
#include <fstream>
#include <cstring>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <ctime>
#include <sstream>
#include <iomanip>

using namespace std;

int MAX_THREADS = 10;
int MAX_FILE_SIZE = 1024;

mutex mtx;
condition_variable cv;
deque<int> connections;
bool running = true;
vector<thread> threads;
int serverSocket;

void handleConnection(int clientSocket, const string &savePath)
{
    // Чтение содержимого файла от клиента
    char buffer[MAX_FILE_SIZE];
    memset(buffer, 0, MAX_FILE_SIZE);
    recv(clientSocket, buffer, MAX_FILE_SIZE, 0);

    // Сохранение файла
    time_t now = time(0);
    tm* ltm = localtime(&now);
    stringstream ss;
    ss << put_time(ltm, "%Y-%m-%d_%H-%M-%S");
    string currentDateTime = ss.str();
    string filePath = savePath + currentDateTime + "_file.txt";
    ofstream file(filePath);
    file << buffer;

    // Отправка пути файла клиенту
    send(clientSocket, filePath.c_str(), filePath.size(), 0);

    // Закрытие соединения с клиентом
    close(clientSocket);
}

void server(int port, int maxThreads, int maxFileSize, const string &savePath)
{
    MAX_FILE_SIZE = maxFileSize;
    MAX_THREADS = maxThreads;

    // Создание сокета сервера
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    // Привязка сокета к адресу и порту
    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    // Ожидание входящих соединений
    listen(serverSocket, 5);

    // Создание потоков для обработки соединений
    for (int i = 0; i < maxThreads; ++i)
    {
        threads.emplace_back([&]
                             {
            while (running)
            {
                int clientSocket;
                {
                    // Блокировка мьютекса для безопасного доступа к очереди соединений
                    unique_lock<mutex> lock(mtx);
                    // Ожидание, пока в очереди не появится новое соединение или не придет сигнал остановки
                    cv.wait(lock, [&] { return !connections.empty() || !running; });
                    // Если получен сигнал остановки, выходим из цикла
                    if (!running || connections.empty())
                        break;
                    // Извлечение соединения из очереди
                    clientSocket = connections.front();
                    connections.pop_front();
                }
                // Обработка соединения
                handleConnection(clientSocket, savePath);
            } });
    }

    // Принятие новых соединений и добавление их в очередь
    while (running)
    {
        int clientSocket = accept(serverSocket, NULL, NULL);
        if (clientSocket == -1)
        {
            if (errno == EINTR)
            {
                // Если accept был прерван сигналом, продолжаем работу
                continue;
            }
            else
            {
                // Обработка ошибки accept
                cerr << "Error accepting connection: " << strerror(errno) << endl;
                break;
            }
        }
        {
            // Блокировка мьютекса для безопасного доступа к очереди соединений
            lock_guard<mutex> lock(mtx);
            // Добавление нового соединения в очередь
            connections.push_back(clientSocket);
        }
        // Уведомление одного из потоков о появлении нового соединения
        cv.notify_one();
    }

    // Ожидание завершения всех потоков
    for (auto &thread : threads)
    {
        thread.join();
    }

    // Закрытие серверного сокета
    close(serverSocket);
}

// Обработчик сигналов SIGTERM, SIGHUP и SIGPIPE
void signalHandler(int signum)
{
    if (signum == SIGTERM || signum == SIGHUP || signum == SIGPIPE)
    {
        if (signum == SIGPIPE)
        {
            cout << "Received SIGPIPE signal. Client connection closed.\n";
        }
        else
        {
            cout << "Received signal " << signum << ". Terminating...\n";
        }

        // Установка флага для завершения работы сервера
        running = false;

        // Уведомление всех потоков о необходимости завершения работы
        cv.notify_all();

        // Ожидание завершения всех потоков
        for (auto &thread : threads)
        {
            thread.join();
        }

        // Закрытие серверного сокета
        close(serverSocket);

        // Выход из программы
        exit(0);
    }
}

int main(int argc, char *argv[])
{
    int port, maxThreads, maxFileSize;
    string savePath;

    if (argc == 5)
    {
        port = stoi(argv[1]);
        maxThreads = stoi(argv[2]);
        maxFileSize = stoi(argv[3]);
        savePath = argv[4];
    }
    else
    {
        char choice;
        cout << "Введите 'f' для использования файла конфигурации или любое другое значение для ввода вручную: ";
        cin >> choice;

        if (choice == 'f')
        {
            string configFilePath;
            cout << "Введите путь к файлу конфигурации: ";
            cin.ignore(); // Очистка буфера ввода
            getline(cin, configFilePath);

            if (configFilePath.empty())
            {
                configFilePath = "server.cfg";
            }

            ifstream configFile(configFilePath);
            if (configFile.is_open())
            {
                string line;
                while (getline(configFile, line))
                {
                    if (line.find("port=") == 0)
                    {
                        port = stoi(line.substr(5));
                    }
                    else if (line.find("max_threads=") == 0)
                    {
                        maxThreads = stoi(line.substr(12));
                    }
                    else if (line.find("max_file_size=") == 0)
                    {
                        maxFileSize = stoi(line.substr(14));
                    }
                    else if (line.find("save_path=") == 0)
                    {
                        savePath = line.substr(10);
                    }
                }
                configFile.close();
            }
            else
            {
                cerr << "Ошибка открытия файла конфигурации: " << configFilePath << endl;
                return 1;
            }
        }
        else
        {
            cout << "Введите номер порта: ";
            cin >> port;

            cout << "Введите максимальное количество потоков: ";
            cin >> maxThreads;

            cout << "Введите максимальный размер файла: ";
            cin >> maxFileSize;

            cout << "Введите путь для сохранения файлов: ";
            cin >> savePath;
        }
    }

    // Установка обработчиков сигналов SIGTERM, SIGHUP и SIGPIPE
    signal(SIGTERM, signalHandler);
    signal(SIGHUP, signalHandler);
    signal(SIGPIPE, signalHandler);

    // Установка обработчика сигнала SIGCHLD
    signal(SIGCHLD, SIG_IGN);

    try
    {
        server(port, maxThreads, maxFileSize, savePath);
    }
    catch (const exception &e)
    {
        cerr << "Ошибка: " << e.what() << endl;
    }
    // Выход из программы
    exit(0);
    return 0;
}
