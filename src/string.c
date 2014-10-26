#include "string.h"
#include "types.h"

void memset(void *dest, int val, size_t len) {
    char  *temp = (char *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}
