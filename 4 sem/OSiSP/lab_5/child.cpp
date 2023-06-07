#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <ctime>
#include <random>

#define SHM_SIZE 1024 // определение размера разделяемой памяти

typedef struct {
    uint8_t type; // тип сообщения
    uint16_t hash; // хэш-сумма
    uint8_t size; // размер
    char data[256]; // массив символов
} Message;

union semun { // работа с семафорами
    int val;
    struct semid_ds *buf;
    ushort *array;
};

void error(const char* msg) { // вывод ошибок
    perror(msg);
    exit(1);
}

void generateMessage(Message* message) { // генерация рандомного сообщения
    std::random_device rd; // создание объекта для генерации случайных чисел
    std::mt19937 gen(rd()); // инициализация генератора случ чисел
    std::uniform_int_distribution<> sizeDistribution(1, 256); // распед 1-256 (случ размер)
    std::uniform_int_distribution<> dataDistribution(32, 126); // распред 32-126 (случ символы)

    message->type = 1; // установка типа сообщения
    message->hash = 0; // инициализация хэш-значения сообщения
    message->size = sizeDistribution(gen); // случ размер сообщения
    for (int i = 0; i < message->size; ++i) { // цикл по каждому символу
        message->data[i] = dataDistribution(gen); // генерация случ символа
        message->hash += message->data[i]; // вычисление хэш-значения
    }
}

int main() {
    key_t key = ftok("child.cpp", 'R'); // создание ключа для доступа к памяти и семафорам
    if (key == -1)
        error("ftok failed");

    int shmid = shmget(key, SHM_SIZE, 0); // для получения идентификатора раздел памяти
    if (shmid == -1)
        error("shmget failed");

    Message* sharedMemory = (Message*)shmat(shmid, NULL, 0); // присоединение разделяемой памяти
    if (sharedMemory == (void*)-1)
        error("shmat failed");

    int semid = semget(key, 2, 0); // индетификатор семафоров (ключ+сем(2))
    if (semid == -1)
        error("semget failed");

    sembuf buf; // для работы с семафорами
    buf.sem_flg = 0;

    std::cout << "Child process started." << std::endl;

    while (true) {
buf.sem_num = 0;  // Семафор для добавления сообщения
buf.sem_op = -1; // Уменьшить значение семафора на 1        
semop(semid, &buf, 1); // выполнить операцию
        Message* message = sharedMemory;

        generateMessage(message); // заполняем рандом знач 

        buf.sem_num = 1;  // Семафор для потребления сообщения
        buf.sem_op = 1;  // Увеличить значение семафора на 1
        semop(semid, &buf, 1);

        std::cout << "Produced a message. Total: " << (int)message->size << std::endl; // вывод размера

        sleep(1);
    }

    shmdt(sharedMemory); // остоединяемся от адресного пространства

    std::cout << "Child process finished." << std::endl; // завершение дочерного процесса

    return 0;
}
