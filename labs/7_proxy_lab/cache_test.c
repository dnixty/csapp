#include "csapp.h"
#include "cache.h"

int
main()
{
        cache *ca;
        char *buf;

        ca = cache_init(10, 4);
        cache_insert(ca, "http://google.com", "abc", 4);
        cache_insert(ca, "http://twitter.com", "zzz", 4);

        buf = cache_find(ca, "http://google.com");
        printf("%lx\n", (long)buf);

        cache_insert(ca, "http://4chan.org", "lol", 4);

        buf = cache_find(ca, "http://google.com");
        printf("%ld\n", (long)buf);

        cache_print(ca);
        cache_deinit(ca);

        return 0;
}
