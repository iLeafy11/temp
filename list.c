#include <stdlib.h>
#include <stdbool.h>

#include "list.h"

/*
typedef enum IterationResult {
    BREAK,
    DONE,
} IterationResult;

typedef struct Cell {
    struct list_head list;
    void *value;
    size_t size;
} ListCell;

typedef bool (*ListIterator)(void *);
*/

void listDel(struct list_head *head)
{
    ListCell *entry;
    struct list_head *curr, *next;
    list_for_each_safe(curr, next, head) {
        list_remove(curr);
        entry = list_entry(curr, ListCell, list);
        free(entry->value);
        free(entry);
    }
}

IterationResult listForEach(struct list_head *head, ListIterator iterator)
{
    if (list_empty(head))
        return DONE;

    bool res;
    struct list_head *pos;
    list_for_each(pos, head) {
        res = iterator(list_entry(pos, ListCell, list)->value);
        if (!res)
            break;
    }

    return res ? DONE : BREAK;
}

