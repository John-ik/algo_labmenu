#include "slist.h"
#include <stdlib.h>

SList* slist_at(SList* slist, size_t index){
    while(slist != NULL){
        if (index == 0){
            return slist;
        }
        slist = slist->next;
        index--;
    }
    return NULL;
}

size_t slist_len(SList* slist){
    size_t len = 0;
    while(slist != NULL){
        len++;
        slist = slist->next;
    }
    return len;
}

SList* slist_delete(SList* slist, size_t index){
    SList* save = slist;
    if (index == 0){
        save = slist->next;
        free(slist);
        return save;
    }

    SList* prev = NULL;
    while (slist != NULL)
    {
        if (index == 1){ // следующий искомый
            prev = slist;
        }else if (index == 0){ // искомый
            prev->next = slist->next;
            /*
    до      пред    ->  цель    ->  след
    после   пред    ->  след
            */
            free(slist);
            return save;
        }
        slist = slist->next;
        index--;
    }
    return save;
}

SList* slist_alloc(){
    SList* a = malloc(sizeof(SList));
    a->next = NULL;
    return a;
}

SList* slist_insert_before(SList* slist, SList_payload_t elem, size_t index){
    SList* new = slist_alloc();
    if (new == NULL) return NULL;
    
    new->val = elem;

    if (index == 0){
        new->next = slist;
    }else{
        SList* prev = slist_at(slist, index - 1);
        if (prev == NULL) return NULL;

        new->next = prev->next;
        prev->next = new;
    }
    return new;
}

SList* slist_insert_after(SList* slist, SList_payload_t elem, size_t index){
    SList* new = slist_alloc();
    if (new == NULL) return NULL;
    
    new->val = elem;
    
    SList* prev = slist_at(slist, index);
    if (prev == NULL) return NULL;

    new->next = prev->next;
    prev->next = new;
    return new;
}