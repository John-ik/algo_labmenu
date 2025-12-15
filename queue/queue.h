#pragma once
#include <stddef.h>

typedef struct Queue Queue;

/**
 * @brief Инициализирует очередь
 * 
 * @return Queue* 
 */
Queue* queue();

/**
 * @brief Добавить элемент в очередь (в конец)
 * 
 * @param q 
 * @param elem 
 */
void queue_push(Queue *q, void* elem);

/**
 * @brief убрать элемент из очереди (из начала) (память освобождается)
 * 
 * @param q 
 */
void queue_pop(Queue *q);

/**
 * @brief проверка на пустоту
 * 
 * @param q 
 * @return 0 если есть элементы
 */
int queue_empty(Queue *q);

/**
 * @brief показать значение первого элемента (он следующим будет убран)
 * 
 * @param q 
 * @return void* 
 */
void* queue_peek(Queue *q);

/**
 * @brief проитерироваться по всей очереди с начала в конец
 * 
 * @param q 
 * @param f функция которая будет вызываться над каждый элементом
 */
void queue_foreach(Queue *q, void (*f)(void*));

/**
 * @brief удалить всю очередь (память освобождается). Указатель где хранилась очередь затирается
 * 
 * @param q Для безопасности памяти требуется адрес указателя где храниться очередь
 */
void queue_free(Queue **q);
