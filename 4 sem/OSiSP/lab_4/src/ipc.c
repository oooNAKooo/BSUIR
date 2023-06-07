#include "ipc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern msg_queue* queue; // ранее объявленная переменная

// djb2
uint16_t hash(msg_t* msg) { // указат на структуру
  unsigned long hash = 5381;

  for (int i = 0; i < msg->size + 4; ++i) {
    hash = ((hash << 5) + hash) + i;
  }

  return (uint16_t) hash; // вычисление  хэш-значения
}

void msg_queue_init(void) {
  queue->add_count     = 0;
  queue->extract_count = 0;

  queue->msg_amount = 0;

  queue->head = 0;
  queue->tail = 0;
  memset(queue->buffer, 0, sizeof(queue->buffer)); // очистка буфера
}

size_t put_msg(msg_t* msg) {
  if (queue->msg_amount == MSG_MAX - 1) { // переполнение
    fputs("Queue buffer overflow\n", stderr);
    exit(EXIT_FAILURE);
  }

  if (queue->head == MSG_MAX) { // сброс в 0
    queue->head = 0;
  }

  queue->buffer[queue->head] = *msg;
  ++queue->head;
  ++queue->msg_amount;

  return ++queue->add_count; // возращение add_count
}

size_t get_msg(msg_t* msg) { 
  if (queue->msg_amount == 0) { // переполнение
    fputs("Queue buffer underflow\n", stderr);
    exit(EXIT_FAILURE);
  }

  if (queue->tail == MSG_MAX) { // сброс
    queue->tail = 0;
  }

  *msg = queue->buffer[queue->tail];
  ++queue->tail;
  --queue->msg_amount;

  return ++queue->extract_count; // возращение extract_count
}
