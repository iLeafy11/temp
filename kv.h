#ifndef KV_H
#define KV_H

#include <stdlib.h>
#include <stdbool.h>

#include "list.h"

typedef struct KV {
    xs key;
    xs value;
} KV;

/*
typedef struct KVList {
    KV value;
    struct list_head list;
} kvlist;
*/
// do not need to allocate another block of memory

KV *kvNew(char *, char *);
void kvDel(KV *);
void kvDelList(struct list_head *);
void kvPrintList(struct list_head *);
char *kvFindList(struct list_head *, char *);
#endif
