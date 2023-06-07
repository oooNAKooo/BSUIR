#include <stdio.h>
#include <stdlib.h>

#include <semaphore.h> // для раюоты с семафорами

#include "init_and_exit.h"
#include "producer.h"
#include "consumer.h"

static const char options[] = {"\t\t\tMenu:\n"
                               "\t[M] - Show menu\n"
                               "\t[1] - Start producer\n"
                               "\t[2] - Start consumer\n"
                               "\t[3] - Stop producer\n"
                               "\t[4] - Stop consumer\n"
                               "\t[e] - Exit"}; // менюшка

msg_queue* queue; // структура для хранения сообщений в очереди
sem_t    * mutex; // синхронизация меж процессами

sem_t* free_space; // -//-
sem_t* items;      // -//- 

pid_t  producers[CHILD_MAX]; // массив производителей
size_t producers_amount; // кол-во произв

pid_t  consumers[CHILD_MAX]; // массив потребителей
size_t consumers_amount; // кол-во потреб

int main(void) {
  init(); // 

  puts(options);
  while (1) {
    switch (getchar()) {
      case 'M':
        puts(options);
        break;

      case '1':
        create_producer();
        break;

      case '2':
        create_consumer();
        break;

      case '3':
        remove_producer();
        break;

      case '4':
        remove_consumer();
        break;

      case 'e':
        exit(EXIT_SUCCESS);

      case EOF:
        fputs("Input error", stderr);
        exit(EXIT_FAILURE);

      default:
        break;
    }
  }

}
