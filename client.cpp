#include <iostream>
#include <fstream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

void client(const char *serverAddress, int port, const char *filePath)
{
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, serverAddress, &serverAddr.sin_addr);

    connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    ifstream file(filePath);
    string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    send(clientSocket, fileContent.c_str(), fileContent.size(), 0);

    char buffer[1024];
    memset(buffer, 0, 1024);
    recv(clientSocket, buffer, 1024, 0);

    cout << "Server response: " << buffer << endl;

    close(clientSocket);
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
        cout << "Enter 'f' to use a config file or 'm' to enter manually: ";
        cin >> choice;

        if (choice == 'f')
        {
            string configFilePath;
            cout << "Enter the path to the config file (or leave empty for default 'client.cfg'): ";
            cin.ignore(); // Clear input buffer
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
                cerr << "Error opening config file: " << configFilePath << endl;
                return 1;
            }
        }
        else if (choice == 'm')
        {
            cout << "Enter server address: ";
            cin >> serverAddress;

            cout << "Enter port number: ";
            cin >> port;

            cout << "Enter file path: ";
            cin >> filePath;
        }
        else
        {
            cerr << "Invalid choice" << endl;
            return 1;
        }
    }

    client(serverAddress.c_str(), port, filePath.c_str());

    return 0;
}