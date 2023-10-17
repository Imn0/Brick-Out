#include "bo_list.h"
#include "bo_result.h"
#include "bo_vector2d.h"

#include <assert.h>
#include <stdlib.h>

BO_Result BO_List_assign(BO_List **list)
{
    BO_Result result = BO_SUCCESS;

    BO_List *new_list = (BO_List *)calloc(1, sizeof(BO_List));
    if (new_list == NULL)
    {
        result = BO_FAILED_TO_ALLOCATE_LIST;
        goto fail;
    }

    new_list->head = (BO_Node *)calloc(1, sizeof(BO_Node));
    if (new_list->head == NULL)
    {
        result = BO_FAILED_TO_ALLOCATE_NODE;
        goto fail;
    }

    *list = new_list;

    return result;
fail:
    BO_List_destroy(new_list);
    return result;
}

void BO_List_destroy(BO_List *list)
{
    if (list == NULL)
    {
        return;
    }

    BO_Node *curr = list->head;

    while (curr != NULL)
    {
        BO_Node *next = curr->next;

        if (curr->dtor != NULL)
        {
            curr->dtor(curr->value);
        }

        free(curr);
        curr = next;
    }

    free(list);
}

BO_Result BO_List_push_back(BO_List *list, void *value)
{
    return BO_List_push_back_dtor(list, value, NULL);
}

BO_Result BO_List_push_back_dtor(BO_List *list, void *value, void (*dtor)(void *))
{
    assert(list != NULL);
    BO_Result result = BO_SUCCESS;

    BO_Node *curr = list->head;

    // going to the back
    while (curr->next != NULL)
    {
        curr = curr->next;
    }

    BO_Node *new_node = (BO_Node *)calloc(1, sizeof(BO_Node));
    if (new_node == NULL)
    {
        result = BO_FAILED_TO_ALLOCATE_NODE;
        goto ret;
    }

    new_node->value = value;
    new_node->dtor = dtor;
    curr->next = new_node;

ret:
    return result;
}

void BO_List_remove(BO_List *list, const BO_ListItr *itr)
{
    assert(list != NULL);
    assert(itr != NULL);

    BO_Node *curr = list->head;

    // stop searching one step before node to remove
    while ((curr->next != NULL) && (curr->next != itr->node))
    {
        curr = curr->next;
    }

    // if we are not at the end then we found the node to remove
    if (curr->next != NULL)
    {
        BO_Node *to_remove = curr->next;
        BO_Node *next = to_remove->next;

        if (to_remove->dtor != NULL)
        {
            to_remove->dtor(to_remove->value);
        }

        free(to_remove);
        curr->next = next;
    }
}

BO_Result BO_List_iterator_assign_list(const BO_List *list, BO_ListItr **itr)
{
    assert(list != NULL);
    assert(itr != NULL);

    BO_Result result = BO_SUCCESS;

    BO_ListItr *new_itr = (BO_ListItr *)calloc(1, sizeof(BO_ListItr));
    if (new_itr == NULL)
    {
        result = BO_FAILED_TO_ALLOCATE_ITERATOR;
        goto ret;
    }

    BO_List_iterator_reset(list, &new_itr);

    *itr = new_itr;

ret:
    return result;
}

void BO_List_iterator_destroy(BO_ListItr *itr)
{
    assert(itr != NULL);
    free(itr);
}

void BO_List_iterator_advance(BO_ListItr **itr)
{
    assert(*itr != NULL);

    (*itr)->node = (*itr)->node->next;
}

void BO_List_iterator_reset(const BO_List *list, BO_ListItr **itr)
{
    assert(*itr != NULL);

    // head empty
    (*itr)->node = list->head->next;
}

bool BO_List_iterator_at_end(BO_ListItr *itr)
{
    assert(itr != NULL);
    return itr->node == NULL;
}

void *BO_List_iterator_value(const BO_ListItr *itr)
{
    assert(itr != NULL);
    return itr->node->value;
}