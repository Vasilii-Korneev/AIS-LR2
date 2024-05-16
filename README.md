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
### Вариант 1
1) Скачать [исполняемый файл сервера](https://github.com/Vasilii-Korneev/AIS-LR2/blob/main/server)
2) Скачать [исполняемый файл клиента](https://github.com/Vasilii-Korneev/AIS-LR2/blob/main/client)
3) Скачать [исполняемый текстовый файл](https://github.com/Vasilii-Korneev/AIS-LR2/blob/main/file.txt), либо создать его самостоятельно в каталоге с исполняемыми файлами и назвать "file.txt"
4) Открыть терминал (Ctrl+Alt+T)
5) Ввести следующие команды:
   ###### sudo chmod +x ~/Загрузки/server
   ###### sudo ./Загрузки/server 8000 50 1024 'путь к каталогу сохранения файла'
   ###### sudo chmod +x ~/Загрузки/client
   ###### sudo ./Загрузки/server 127.0.0.1 8000 'путь к отправляемому файлу'
7) Следовать указаниям в терминале
### Вариант 2
1) Открыть терминал (Ctrl+Alt+T)
2) Ввести следующие команды:
   ###### sudo apt install git
   ###### cd ~/Документы
   ###### git clone https://github.com/Vasilii-Korneev/AIS-LR1.git
   ###### cd ~/Документы/AIS-LR1
   ###### g++ -o ais-lr1 main.cpp
   ###### ./ais-lr1
4) Следовать указаниям в терминале
5) Для повторного запуска программы введите:   
   ###### ./ais-lr1
## Instructions for running on Linux (Ubuntu):
   We are already working on it...
   
## Инструкция по запуску на Windows 10/11
   Мы уже работаем над этим...
## Instructions for launching on Windows 10/11
   We are already working on it...
