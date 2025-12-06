#include <stdio.h>

#include "slist.h"

/*
Данные для построения списков вводить в режиме диалога.
Во всех работах разработать функцию удаления списков.

6.Определить количество различных элементов списка действительных чисел, если
известно, что его элементы образуют неубывающую последовательность.
*/

int iteractive_list(){
    SList* list;
    while(1){
        fputs("Введите команду (h - помощь): ", stdout);
        char c;
        scanf(" %1c", &c);

        double val;
        size_t index;
        switch(c){
            case 'q':
                return 0;

            case 'n':
                scanf("%lf", &val);
                list = slist_alloc();
                list->val = val;
                break;

            case 'l':
                printf("Длина списка: %zu\n", slist_len(list));
                break;

            case 'a':
                scanf("%zu %lf", &index, &val);
                slist_insert_after(list, val, index);
                break;

            case 'b':
                scanf("%zu %lf", &index, &val);
                if (index == 0){
                    list = slist_insert_before(list, val, index);
                }else{
                    slist_insert_before(list, val, index);
                }
                break;

            case 'd':
                scanf("%zu", &index);
                list = slist_delete(list, index);
                break;

            case 'p':
                size_t uniq = 1;
                int flag_broken = 0;
                for(SList* iter = list, *next; iter != NULL; iter = iter->next){
                    printf("%f -> ", iter->val);
                    if ((next = iter->next) != NULL){
                        if (iter->val < next->val)
                            uniq++;
                        else if (iter->val > next->val)
                            flag_broken = 1;
                    }
                } puts("null");

                if (flag_broken){
                    puts("Последовательность не соотв условию: неубывающая последовательность");
                }else{
                    printf("Уникальных элементов в последовательности: %zu\n", uniq);
                }
                break;

            case 'h':
            default:
                puts(
                    "Справка (h):\n"
                    "\tq - ВЫЙТИ из подпрограммы\n"
                    "\tn <REAL> - новый список (сброс старого), REAL полезная нагрузка элемента\n"
                    "\tl - вывести длину списка\n"
                    "\ta <INTEGER> <REAL> - вставить элемент со значением REAL ПОСЛЕ элемента списка под индексом INTEGER\n"
                    "\tb <INTEGER> <REAL> - вставить элемент со значением REAL ПЕРЕД элементом списка под индексом INTEGER\n"
                    "\td <INTEGER> - удалить элемент под индексом INTEGER\n"
                    "\tp - вывести результат задания"
                );
                break;
        }
    }


    return 0;
}
