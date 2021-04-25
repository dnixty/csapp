#include "csapp.h"
#include "cache.h"

int
main()
{
        cache *ca;
        char *buf = NULL;
        int size;

        ca = cache_init(10, 4);
        cache_insert(ca, "http://google.com", "abc", 4);
        cache_insert(ca, "http://twitter.com", "zzz", 4);

        size = cache_find(ca, "http://google.com", &buf);
        printf("%d\n", size);

        cache_insert(ca, "http://4chan.org", "lol", 4);

        size = cache_find(ca, "http://google.com", &buf);
        printf("%d\n", size);

        cache_print(ca);
        cache_deinit(ca);

        return 0;
}
