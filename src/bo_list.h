#ifndef BO_LIST_H
#define BO_LIST_H

#include "bo_result.h"
#include <stdbool.h>

/**
 * Simple linked list.
 */

/**
 * To handle list data.
 */
typedef struct BO_Node
{
    void *value;
    struct BO_Node *next;
    void (*dtor)(void *);
} BO_Node;

typedef struct BO_List
{
    BO_Node *head;
} BO_List;

/**
 * To handle internal list iterator.
 */
typedef struct BO_ListItr
{
    BO_Node *node;
} BO_ListItr;

BO_Result BO_List_assign(BO_List **list);

void BO_List_destroy(BO_List *list);

BO_Result BO_List_push_back(BO_List *list, void *value);

BO_Result BO_List_push_back_dtor(BO_List *list, void *value, void (*dtor)(void *));

void BO_List_remove(BO_List *list, const BO_ListItr *itr);

BO_Result BO_List_iterator_assign_list(const BO_List *list, BO_ListItr **itr);

void BO_List_iterator_destroy(BO_ListItr *itr);

void BO_List_iterator_advance(BO_ListItr **itr);

void BO_List_iterator_reset(const BO_List *list, BO_ListItr **itr);

bool BO_List_iterator_at_end(BO_ListItr *itr);

void *BO_List_iterator_value(const BO_ListItr *itr);

#endif // BO_LIST_H