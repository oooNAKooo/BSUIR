#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <csignal>
#include <thread>

using namespace std;

const int bufferSize = 256;

struct Message {
    char data[bufferSize]; // символы
    int size; // размер
    int hash; // хэш
};
//unique_lock vs lock_guard лучше unique_lock так как более гибким
// Размер буфера сообщений
// Циклический буфер для хранения сообщений
queue<Message*> messageQueue; // очередь для хранения сообщений
int messageCount = 0; // кол-во добавленных сообщений
int messageExtracted = 0; // кол-во извлеченных сообщений
mutex queueMutex; // мьютекс для синхронизации к очереди
//это примитив синхронизации, предоставляемый стандартной библиотекой C++11, который позволяет потокам ждать, 
//пока определенное условие станет истинным, прежде чем продолжить. 
condition_variable queueNotEmpty; // очередь не пуста
condition_variable queueNotFull; // очередь не полна
int queueSize = 5; // макс размер

unsigned short calculateHash(char* hashed) { // вычисление хэша сообщения 
    unsigned short crc = 0xFFFF;
    unsigned char i;
    unsigned char len = strlen(hashed);

    while (len--) {
        crc ^= *hashed++ << 8; // значение символа *hashed сдвигается на 8 бит влево для формирования хэша

        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1; // проверка, является ли старший бит crc == 1
    } // если да – то сдвиг влево на 1 бит, если нет -  то просто сдвиг влево на 1 бит
    return crc;
}
// 0х1021 для вычисления контрольной суммы хэша
// XOR применяется в вычислении хэша: для равномерного распределения изменений, //предотвращения коллизий (2 разных сообщение == 1 результат хэша), обеспечение //необратимости хэш-функций (невозможно восстановить исходные данные)


void producer() { // генерация новых сообщений и добавления их в очередь
    srand(time(NULL)); // инициализация генерации рандома

    while (true) { 
        Message* message = new Message(); // создание нового сообщения
        message->size = rand() % 257; // генерация случайного размера

        if (message->size == 0) { // если 0, то 256
            message->size = 256;
        } else if (message->size == 256) { // если 256, то 0
            message->size = 0;
        } // это сделано для теста, когда размер сообщения == граниченым значениям

        for (int i = 0; i < 10; i++) {
            message->data[i] = char(rand() % 100); // заполнение рандомными символами 
        }

        message->hash = calculateHash(message->data); // вычисление хэша 
        // Ожидаем свободного места в очереди
        unique_lock<mutex> locker(queueMutex); // блокировка мьютекса для доступа к очереди
        while (messageQueue.size() >= queueSize) { // когда в очереди появится свободное место
            queueNotFull.wait(locker); // ожидание о появлении свободного места
        }

        messageQueue.push(message); // добавляем сообщение в очередь (в конец) 
        sleep(2); // задержка

        messageCount++; // счетчик размера ++
        cout << "Producer: Added message " << messageCount << endl;

        locker.unlock(); // разблок мьютекса
        queueNotEmpty.notify_one(); // сигнал о появлении нового сообщения
    }
}

void consumer() { // извлечение сообщения из очереди и проверка хэша
    while (true) {
        unique_lock<mutex> locker(queueMutex);
        while (messageQueue.empty()) { // пока в очереди появится сообщение
            queueNotEmpty.wait(locker);
        }

        Message* message = messageQueue.front();
        messageQueue.pop();
        // Снимаем блокировку с очереди
        locker.unlock();

        int calculatedHash = calculateHash(message->data); // проверка хэша
        if (message->hash != calculatedHash) {
            cout << "Consumer: Message hash check failed" << endl;
        }
        sleep(2); // задержка
messageExtracted++;
        cout << "Consumer: Extracted message " << messageExtracted << endl;
        cout << "Got message hash: " << message->hash << "\n"
             << "Size text: " << message->size << "\n"
             << "Text: " << message->data << endl;

        delete message;

        locker.lock();
        queueNotFull.notify_one();
    }
}

void signalHandler(int signum) { // обработчик SIGINT
    cout << "Received signal " << signum << ". Exiting..." << endl;
    exit(signum); //выход
}

int main() {
    signal(SIGINT, signalHandler);  // Обработка сигнала прерывания (Ctrl+C)
    thread producerThread(producer); // одновременный запуск
    thread consumerThread(consumer);
while (true) {
        char key = cin.get();
        if (key == '+') { // + к размеру очереди
            cout << "Queue size : " << queueSize << endl;
            unique_lock<mutex> locker(queueMutex);
            queueSize++;
            cout << "///////////////////////////////////////////////////////////////////"<< endl;
            cout << "Queue size increased: " << queueSize << endl;
            cout << "///////////////////////////////////////////////////////////////////"<< endl;
            locker.unlock();
            queueNotFull.notify_all();
        } else if (key == '-') { // - к размеру очереди
            cout << "Queue size : " << queueSize << endl;
            unique_lock<mutex> locker(queueMutex);
            if (queueSize > 0) {
                queueSize--;
                cout << "///////////////////////////////////////////////////////////////////"<< endl;
                cout << "Queue size decreased: " << queueSize << endl;
                cout << "///////////////////////////////////////////////////////////////////"<< endl;
                locker.unlock();
                queueNotFull.notify_all();
            }
        } else if (key == 'q') {
            cout << "Exiting..." << endl;
            raise(SIGINT);  // Генерация сигнала прерывания (Ctrl+C) для завершения программы
        }
    }

    producerThread.join(); // блокирует текущий поток и ожидает завершение потока
    consumerThread.join();//используется для ожидания завершения выполнения потока

    return 0;
}
