#include <stdio.h>
#include <string.h>

#include "xs.h"
#include "kv.h"

KV *kvNew(char *key, char *value)
{
    KV *kv = malloc(sizeof(KV));

    kv->key = *xs_new(&xs_literal_empty(), key);
    kv->value = *xs_new(&xs_literal_empty(), value);

    return kv;
}

void kvDel(KV *kv)
{
    xs_clean(&kv->key);
    xs_clean(&kv->value);
    free(kv);
    kv = NULL;
}

static bool kvDelEach(void *kv)
{
    if (kv) {
        xs_clean(&((KV *) kv)->key);
        xs_clean(&((KV *) kv)->value);
    }

    return true;
}

void kvDelList(struct list_head *kvlist)
{
    /**
     * The following implementation has redundant iteration.
     *
     * listForEach(kvlist, kvDelEach);
     * listDel(kvlist);
     *
     * Therefore, this might be a better solution.
     */

    ListCell *target;
    struct list_head *curr, *next;
    list_for_each_safe(curr, next, kvlist) {
        list_remove(curr);
        target = list_entry(curr, ListCell, list);
        kvDel(target->value);
        free(target);
    }

}


static bool kvPrintEach(void *kv)
{
    xs k = ((KV *) kv)->key, v = ((KV *) kv)->value;
    fprintf(stdout, "%s: %s\n", xs_data(&k), xs_data(&v));

    return true;
}


void kvPrintList(struct list_head *kvlist)
{
    listForEach(kvlist, kvPrintEach);
}


char *kvFindList(struct list_head *cell, char *key)
{
    xs k, v;
    while (cell) {
        k = ((KV *) (list_entry(cell, ListCell, list)->value))->key;
        if (!strcmp(xs_data(&k), key)) {
            v = ((KV *) (list_entry(cell, ListCell, list)->value))->value;
            return xs_data(&v);
        }
        cell = cell->next;
    }

    return NULL;
}
