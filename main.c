#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "xs.h"
#include "list.h"

typedef struct __element {
    xs value;
    struct list_head list;
} list_ele_t;

static list_ele_t *get_middle(struct list_head *list)
{
    struct list_head *fast = list->next, *slow;
    list_for_each (slow, list) {
        if (fast->next == list || fast->next->next == list)
            break;
        fast = fast->next->next;
    }
    return list_entry(slow, list_ele_t, list);
}

static void list_merge(struct list_head *lhs,
                       struct list_head *rhs,
                       struct list_head *head)
{
    INIT_LIST_HEAD(head);
    while (!list_empty(lhs) && !list_empty(rhs)) {
        xs l = list_entry(lhs->next, list_ele_t, list)->value;
        xs r = list_entry(rhs->next, list_ele_t, list)->value;
        char *lv = xs_data(&l), *rv = xs_data(&r);
        struct list_head *tmp = strcmp(lv, rv) <= 0 ? lhs->next : rhs->next;
        list_remove(tmp);
        list_add_tail(tmp, head);
    }
    list_splice_tail(list_empty(lhs) ? rhs : lhs, head);
}

bool list_alloc_add_tail(struct list_head *head, char *s)
{
    list_ele_t *new = malloc(sizeof(list_ele_t));
    if (!new) {
        return false;
    }

    new->value = *xs_new(&new->value, s);

    /*
    if (!new->value) {
        free(new);
        return false;
    }
    */

    list_add_tail(&new->list, head);
    return true;
}

void list_free(struct list_head *head)
{
    list_ele_t *target;
    struct list_head *curr, *next;
    list_for_each_safe(curr, next, head) {
        list_remove(curr);
        target = list_entry(curr, list_ele_t, list);
        xs_clean(&target->value);
        // free(target->value);
        free(target);
    }
}

void list_merge_sort(struct list_head *list)
{
    if (list_empty(list) || list_is_singular(list))
        return;

    struct list_head left;
    struct list_head sorted;
    INIT_LIST_HEAD(&left);
    list_cut_position(&left, list, &get_middle(list)->list);
    list_merge_sort(&left);
    list_merge_sort(list);
    list_merge(&left, list, &sorted);
    INIT_LIST_HEAD(list);
    list_splice_tail(&sorted, list);
}

void list_display(struct list_head *list)
{
    struct list_head *walk;
    list_for_each (walk, list) {
        printf("%s", xs_data(&list_entry(walk, list_ele_t, list)->value));
    }
}

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static bool validate(struct list_head *list)
{
    struct list_head *node;
    list_for_each (node, list) {
        if (node->next == list)
            break;
        if (strcmp(xs_data(&list_entry(node, list_ele_t, list)->value),
                   xs_data(&list_entry(node->next, list_ele_t, list)->value)) > 0)
            return false;
    }
    return true;
}

int main(void)
{
    FILE *fp = fopen("cities.txt", "r");
    if (!fp) {
        perror("failed to open cities.txt");
        exit(EXIT_FAILURE);
    }
    LIST_HEAD(list);
    char buf[256];
    while (fgets(buf, 256, fp)) {
        list_alloc_add_tail(&list, buf);
    }

    list_merge_sort(&list);
    list_display(&list);
    fclose(fp);
    assert(validate(&list));
    list_free(&list);
    return 0;
}

