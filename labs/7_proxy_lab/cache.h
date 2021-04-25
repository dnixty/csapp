#include "csapp.h"

struct cache_node {
        struct cache_node *prev;
        struct cache_node *next;
        char *uri;
        char *content;
        unsigned int size;
};

typedef struct {
        struct cache_node *head;
        struct cache_node *tail;
        unsigned int size_total;
        unsigned int max_cache_size;
        unsigned int max_object_size;
        sem_t mutex;
} cache;

cache *cache_init(unsigned int max_cache_size, unsigned int max_object_size);
void cache_deinit(cache *ca);
int cache_insert(cache *ca, char *uri, char *buf, unsigned int size);
struct cache_node *cache_find(cache *ca, char *uri);
void cache_print(cache *ca);
