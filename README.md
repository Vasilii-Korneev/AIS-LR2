# AIS-LR2
#### RU: Реализация потокобезопасной конкурентной очереди задач на С++ для Windows и linux

   Проект представляет собой многопоточное клиент-серверное приложение под Linux. 

   Клиент - программа, запускаемая из консоли о отправляющая на сервер текстовый файл. 
   
   Сервер - демон, примающий и сохраняющий файл, завершающийся по сигналам SIGTERM и SIGHUP. Под каждое соединение выделяется дополнительный поток из ограниченного пула. 
   
   Максимальное количество потоков, порт, максимальный размер файла, а также путь для сохранения файлов указываются в аргументах запуска приложений или конфигурационном файле.
   
#### EN: Implementation of a thread-safe competitive task queue in C++ for Windows and Linux

   The project is a multithreaded client-server application for Linux.

   A client is a program that runs from the console and sends a text file to the server.

   The server is a daemon that receives and saves a file, ending with a SIGTERM and SIGHUP signal. An additional stream from a limited pool is allocated for each connection.

   The maximum number of streams, port, maximum file size, as well as the path for saving files are specified in the application startup arguments or configuration file.

## Требования к системе
 ### В операционной системе должны бытб установлены:
   1) Распределённая система контроля версий GIT

      Для установки на Linux (на Debian/Ubuntu/Mint) откройте терминал и введите:
      ###### sudo apt update
      ###### sudo apt install git
      или (на Fedora/RHEL/CentOS)
      ###### sudo apt update
      ###### sudo dnf install git-all

   3) компилятор языка C++ "G++"

      Для установки на Linux (на Debian/Ubuntu/Mint) откройте терминал и введите:
      ###### sudo apt update
      ###### sudo apt install g++
      или (на Fedora/RHEL/CentOS)
      ###### sudo apt update
      ###### sudo dnf install gcc-c++
   
## Инструкция по запуску на Linux (Ubuntu):
1) Открыть терминал (Ctrl+Alt+T)
2) Ввести следующие команды:
   ###### cd ~/Документы
   ###### git clone https://github.com/Vasilii-Korneev/AIS-LR2.git
   ###### cd ~/Документы/AIS-LR2
   ###### g++ -o server server.cpp  
   ###### ./server
3) Следовать указаниям в терминале
4) При вводе пути конфигурационного файла использовать
   ###### /home/wm/Документы/AIS-LR2/server.cfg
5) Открыть новый терминал (Ctrl+Alt+T)
6) Ввести следующие команды:
   ###### cd ~/Документы/AIS-LR2
   ###### g++ -o client client.cpp  
   ###### ./client
7) Следовать указаниям в терминале
8) При вводе пути конфигурационного файла использовать
   ###### /home/wm/Документы/AIS-LR2/client.cfg
## Instructions for running on Linux (Ubuntu):
1) Open the terminal (Ctrl+Alt+T)
2) Enter the following commands:
   ###### cd ~/Documents
   ###### git clone https://github.com/Vasilii-Korneev/AIS-LR2.git
   ###### cd ~/Documents/AIS-LR2
   ###### g++ -o server server.cpp
   ###### ./server
3) Follow the instructions in the terminal
4) When entering the path of the configuration file, use
   ###### /home/wm/Documents/AIS-LR2/server.cfg
5) Open a new terminal (Ctrl+Alt+T)
6) Enter the following commands:
   ###### cd ~/Documents/AIS-LR2
   ###### g++ -o client client.cpp
   ###### ./client
7) Follow the instructions in the terminal
8) When entering the path of the configuration file, use
   ###### /home/wm/Documents/AIS-LR2/client.cfg
   
## Инструкция по запуску на Windows 10/11
   Мы уже работаем над этим...
## Instructions for launching on Windows 10/11
   We are already working on it...
