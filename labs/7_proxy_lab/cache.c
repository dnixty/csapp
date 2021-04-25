#include "cache.h"
#include "csapp.h"

cache *
cache_init(unsigned int max_cache_size, unsigned int max_object_size)
{
        cache *ca = (cache *)Malloc(sizeof(cache));
        ca->head = NULL;
        ca->tail = NULL;
        ca->size_total = 0;
        ca->max_cache_size = max_cache_size;
        ca->max_object_size = max_object_size;
        return ca;
}

void
cache_deinit(cache *ca)
{
        struct cache_node *node, *tmp;

        node = ca->head;

        while (node != NULL) {
                tmp = node;
                node = node->next;
                Free(tmp->uri);
                Free(tmp->content);
                Free(tmp);
        }
        Free(ca);
}

int
cache_insert(cache *ca, char *uri, char *buf, unsigned int size)
{
        struct cache_node *new, *tail;

        if (size > ca->max_object_size)
                return -1;

        if (ca->size_total + size > ca->max_cache_size) {
                tail = ca->tail;
                if(tail->prev)
                        tail->prev->next = NULL;
                Free(tail->uri);
                Free(tail->content);
                ca->size_total -= tail->size;
                ca->tail = tail->prev;
                return cache_insert(ca, uri, buf, size);
        }


        new = (struct cache_node *)Malloc(sizeof(struct cache_node));
        new->uri = (char *)Malloc((strlen(uri)+1)*sizeof(char));
        new->content = (char *)Malloc(size*sizeof(char));

        new->prev = NULL;
        new->next = ca->head;
        new->size = size;
        strcpy(new->uri, uri);
        memcpy(new->content, buf, size);

        if (ca->head)
                ca->head->prev = new;
        if (!ca->tail)
                ca->tail = new;
        ca->head = new;
        ca->size_total += size;
        return 0;
}

int
cache_find(cache *ca, char *uri, char **buf)
{
        struct cache_node *node;

        node = ca->head;

        while (node != NULL) {
                if (!strcmp(node->uri, uri)) {
                        *buf = node->content;
                        return node->size;
                }
                node = node->next;
        }

        return -1;
}

void
cache_print(cache *ca)
{
        struct cache_node *node;

        node = ca->head;

        while(node != NULL) {
                printf("-- %lx --\n", (long)node);
                printf("uri: %s\n", node->uri);
                printf("content: %s\n", node->content);
                printf("size: %d\n", node->size);
                printf("prev: %lx\n", (long)node->prev);
                printf("next: %lx\n\n", (long)node->next);
                node = node->next;
        }
}
