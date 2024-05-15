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

using namespace std;

const int MAX_THREADS = 10;
const int MAX_FILE_SIZE = 1024;
// const char* SAVE_PATH = "/path/to/save/files/";

mutex mtx;
condition_variable cv;
deque<int> connections;

void signalHandler(int signal)
{
    if (signal == SIGTERM || signal == SIGHUP)
    {
        // Clean up and exit
        exit(0);
    }
}

void handleConnection(int clientSocket, const string &savePath)
{
    // Read file content from client
    char buffer[MAX_FILE_SIZE];
    memset(buffer, 0, MAX_FILE_SIZE);
    recv(clientSocket, buffer, MAX_FILE_SIZE, 0);

    // Save file
    string filePath = savePath + to_string(clientSocket) + "_file.txt";
    ofstream file(filePath);
    file << buffer;

    // Send response to client
    // send(clientSocket, "File saved successfully!", strlen("File saved successfully!"), 0);

    // Send file path to client
    send(clientSocket, filePath.c_str(), filePath.size(), 0);

    close(clientSocket);
}

void server(int port, int maxThreads, int maxFileSize, const string &savePath)
{
    signal(SIGTERM, signalHandler);
    signal(SIGHUP, signalHandler);

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 5);

    vector<thread> threads;
    for (int i = 0; i < maxThreads; ++i)
    {
        threads.emplace_back([&]
                             {
            while (true) {
                int clientSocket;
                {
                    unique_lock<mutex> lock(mtx);
                    cv.wait(lock, [] { return !connections.empty(); });
                    clientSocket = connections.front();
                    connections.pop_front();
                }
                handleConnection(clientSocket, savePath);
            } });
    }

    while (true)
    {
        int clientSocket = accept(serverSocket, NULL, NULL);
        {
            lock_guard<mutex> lock(mtx);
            connections.push_back(clientSocket);
        }
        cv.notify_one();
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
        cout << "Enter 'f' to use a config file or 'm' to enter manually: ";
        cin >> choice;

        if (choice == 'f')
        {
            string configFilePath;
            cout << "Enter the path to the config file (or leave empty for default 'server.cfg'): ";
            cin.ignore(); // Clear input buffer
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
                cerr << "Error opening config file: " << configFilePath << endl;
                return 1;
            }
        }
        else if (choice == 'm')
        {
            cout << "Enter port number: ";
            cin >> port;

            cout << "Enter max number of threads: ";
            cin >> maxThreads;

            cout << "Enter max file size: ";
            cin >> maxFileSize;

            cout << "Enter save path: ";
            cin >> savePath;
        }
        else
        {
            cerr << "Invalid choice" << endl;
            return 1;
        }
    }

    server(port, maxThreads, maxFileSize, savePath);

    return 0;
}