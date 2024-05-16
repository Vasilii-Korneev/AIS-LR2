#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>

using namespace std;

void client(const char *serverAddress, int port, const char *filePath)
{
    // Создание сокета клиента
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverAddress, &serverAddr.sin_addr);

    // Установка соединения с сервером
    connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // Чтение содержимого файла
    ifstream file(filePath);
    string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    // Отправка содержимого файла серверу
    send(clientSocket, fileContent.c_str(), fileContent.size(), 0);

    // Получение ответа от сервера
    char buffer[1024];
    memset(buffer, 0, 1024);
    recv(clientSocket, buffer, 1024, 0);

    // Вывод ответа сервера
    cout << "Server response: " << buffer << endl;

    // Закрытие соединения
    close(clientSocket);

    // Предложение пользователю выполнить ещё одну отправку или завершить работу
    string choice;
    cout << "Хотите выполнить ещё одну отправку? (да/нет): ";
    cin >> choice;

    if (choice == "да")
    {
        // Рекурсивный вызов функции client для отправки ещё одного файла
        client(serverAddress, port, filePath);
    }
    // else
    // {
    //     kill(port, SIGTERM);
    //     kill(port, SIGPIPE);
    // }
}

int main(int argc, char *argv[])
{
    string serverAddress;
    int port;
    string filePath;

    if (argc == 4)
    {
        serverAddress = argv[1];
        port = stoi(argv[2]);
        filePath = argv[3];
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
                configFilePath = "client.cfg";
            }

            ifstream configFile(configFilePath);
            if (configFile.is_open())
            {
                string line;
                while (getline(configFile, line))
                {
                    if (line.find("server_address=") == 0)
                    {
                        serverAddress = line.substr(15);
                    }
                    else if (line.find("port=") == 0)
                    {
                        port = stoi(line.substr(5));
                    }
                    else if (line.find("file_path=") == 0)
                    {
                        filePath = line.substr(10);
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
            cout << "Введите адрес сервера: ";
            cin >> serverAddress;

            cout << "Введите номер порта: ";
            cin >> port;

            cout << "Введите путь к файлу: ";
            cin >> filePath;
        }
    }

    client(serverAddress.c_str(), port, filePath.c_str());
    
    // Выход из программы
    exit(0);
    return 0;
}
