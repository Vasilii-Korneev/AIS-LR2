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
   We are already working on it...
   
## Инструкция по запуску на Windows 10/11
   Мы уже работаем над этим...
## Instructions for launching on Windows 10/11
   We are already working on it...
