#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void *mycalloc(size_t nmemb, size_t size);
// void *malloc(size_t size);
// void *memset(void *s, int c, size_t n);

void *mycalloc(size_t nmemb, size_t size)
{
    void *p;
    size_t buf_size = nmemb * size;

    if (!nmemb || !size || buf_size / size != nmemb)
        return NULL;

    p = malloc(buf_size);
    if (!p)
        return NULL;

    p = memset(p, '\0', buf_size);
    if (!p)
        return NULL;
    return p;
}

int main()
{
    void *p;

    p = mycalloc(2, 4);

    assert(*((int *) p) == 0);
    assert(*(((int *) p)+1) == 0);

    free(p);
    return 0;
}
