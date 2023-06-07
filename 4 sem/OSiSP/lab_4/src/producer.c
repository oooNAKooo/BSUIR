#include "producer.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

#include <sys/wait.h>
#include <semaphore.h>
#include <unistd.h>

extern msg_queue* queue;
extern sem_t    * mutex;

extern sem_t* free_space;
extern sem_t* items;

extern pid_t  producers[];
extern size_t producers_amount;

void create_producer(void) { // создание продюссера
  if (producers_amount == CHILD_MAX - 1) { // если макс, то гг
    fputs("Max value of producers\n", stderr);
    return;
  }

  switch (producers[producers_amount] = fork()) {
    default:
      // Parent process
      ++producers_amount;
      return; // результат присваиваем элементу массива продюссера

    case 0:
      // Child process
      srand(getpid()); // рандом чисел для доч.проц-а
      break;

    case -1: // ошибка
      perror("fork");
      exit(errno);
  }

  msg_t  msg;
  size_t add_count_local;
  while (1) {
    produce_msg(&msg); //генерация сообщения

    sem_wait(free_space); // уменьшение своб.мест

    sem_wait(mutex); // для общего рес-а
    add_count_local = put_msg(&msg); // помещаем сообщение в очередь
    sem_post(mutex); // освободить доступ к общ рес-у

    sem_post(items); // увеличить доступ элементов

    printf("%d produce msg: hash=%X, add_count=%zu\n",
           getpid(), msg.hash, add_count_local);

    sleep(5);
  }

}

void remove_producer(void) { // удаление продюссеров
  if (producers_amount == 0) {
    fputs("No producers to delete\n", stderr);
    return;
  }

  --producers_amount;
  kill(producers[producers_amount], SIGKILL);
  wait(NULL);
}

void produce_msg(msg_t* msg) {
  size_t value = rand() % 257;
  if (value == 256) {
    msg->type = -1;
  } else {
    msg->type = 0;
    msg->size = (uint8_t) value; // unsigned int8
  }

  for (size_t i = 0; i < value; ++i) {
    msg->data[i] = (char) (rand() % 256);
  }

  msg->hash = 0;
  msg->hash = hash(msg); // случайные символы записываются в msg->hash
}
