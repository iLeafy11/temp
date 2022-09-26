#ifndef XS_H
#define XS_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// #include "intern.h"

#define MAX_STR_LEN_BITS (54)
#define MAX_STR_LEN ((1UL << MAX_STR_LEN_BITS) - 1)

#define LARGE_STRING_LEN 256
#define MIDDLE_STRING_LEN 16

/* Memory leaks happen if the string is too long but it is still useful for
 * short strings.
 */
#define xs_tmp(x)                                                   \
    ((void) ((struct {                                              \
         _Static_assert(sizeof(x) <= MAX_STR_LEN, "it is too big"); \
         int dummy;                                                 \
     }){1}),                                                        \
     xs_new(&xs_literal_empty(), x))

#define xs_literal_empty() \
    (xs) { .space_left = 15 }

enum xs_type {
    XS_SHORT = 0,
    XS_MEDIUM,
    XS_LARGE,
};

typedef union {
    /* allow strings up to 15 bytes to stay on the stack
     * use the last byte as a null terminator and to store flags
     * much like fbstring:
     * https://github.com/facebook/folly/blob/master/folly/docs/FBString.md
     */
    char data[MIDDLE_STRING_LEN];

    struct {
        uint8_t filler[15],
                /* how many free bytes in this stack allocated string
                 * same idea as fbstring
                 */
                space_left : 4,
                /* if it is on heap, set to 1 */
                is_ptr : 1, is_large_string : 1, flag2 : 1, flag3 : 1;
    };

    /* heap allocated */
    struct {
        char *ptr;
        /* supports strings up to 2^54 - 1 bytes */
        size_t size : 54,
               /* capacity is always a power of 2 (unsigned)-1 */
               capacity : 6;
        /* the last 4 bits are important flags */
    };
} xs;

int xs_type(const xs *);
char *xs_data(const xs *);
size_t xs_size(const xs *);
size_t xs_capacity(const xs *);

xs *xs_new(xs *, const char *);
xs *xs_grow(xs *, size_t);
xs *xs_concat(xs *, const xs *, const xs *);
xs *xs_trim(xs *, const char *);
void xs_clean(xs *);

#endif
