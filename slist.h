#include <stddef.h>

#define SList_payload_t double

typedef struct SList SList;
struct SList {
    SList* next;
    SList_payload_t val;
};

/**
 * @brief Вставить новый элементе (память выделиться внутри) перед индексом
 * 
 * @param slist 
 * @param elem 
 * @param index 
 * @return SList* вставленный элемент, `NULL` если не удалось
 */
SList* slist_insert_before(SList* slist, SList_payload_t elem, size_t index);

/**
 * @brief Вставить новый элемент (память выделиться внутри) после индекса
 * 
 * @param slist 
 * @param elem 
 * @param index 
 * @return SList* вставленный элемент, `NULL` если не удалось
 */
SList* slist_insert_after(SList* slist, SList_payload_t elem, size_t index);

/**
 * @brief Удалить и освободить память элемента по индексу
 * 
 * @param slist 
 * @param index 
 * @return SList* вернуть slist, если удален не 0-ой элемент, иначе вернуть 1-ый
 */
SList* slist_delete(SList* slist, size_t index);

/**
 * @brief Вернуть адрес на искомый элемент
 * 
 * @param slist 
 * @param index 
 * @return SList* искомый элемент, NULL если нету
 */
SList* slist_at(SList* slist, size_t index);

/**
 * @brief Вернет число элементов в списке
 * 
 * @param slist список
 * @return size_t число элементов
 */
size_t slist_len(SList* slist);

/**
 * @brief Инициализировать элемент списка. Выполните для создания списка
 * Обработки ошибки `malloc` не производиться
 * 
 * @return SList* 
 */
SList* slist_alloc();