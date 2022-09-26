#ifndef KV_H
#define KV_H

#include <stdlib.h>
#include <stdbool.h>

#include "list.h"

typedef struct KV {
    xs key;
    xs value;
} KV;


KV *kvNew(char *, char *);
void kvDel(KV *);
void kvDelList(struct list_head *);
void kvPrintList(struct list_head *);
char *kvFindList(struct list_head *, char *);
#endif
