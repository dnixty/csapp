#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "cachelab.h"

typedef struct cache_line {
    int times;
    int valid;
    int tag;
} CacheLine;

typedef struct args {
    int s;
    int E;
    int b;
    int v;
    char *t;
} Args;

typedef CacheLine *CacheSet;
typedef CacheSet *Cache;

void print_help_and_exit(char *[]);
void initialize_cache(Cache *, Args *);
void free_cache(Cache *, Args *);
void get_args(int, char *[], Args *);
void process(Cache *, Args *, unsigned);

int hits = 0;
int misses = 0;
int evictions = 0;

int main(int argc, char *argv[])
{
    Cache cache;
    Args args;
    FILE *fp;
    char op;
    unsigned address;
    int size = -1;

    args.v = 0;

    get_args(argc, argv, &args);

    initialize_cache(&cache, &args);

    if ((fp = fopen(args.t, "r")) == NULL) {
        printf("Unable to open %s.\n", args.t);
        exit(1);
    }

    while (fscanf(fp, " %c %x,%d", &op, &address, &size) > 0) {
        if (size == -1) continue;
        if (args.v) printf("%c %x,%d", op, address, size);
        switch(op) {
        case 'L':
        case 'S':
            process(&cache, &args, address);
            break;
        case 'M':
            process(&cache, &args, address);
            process(&cache, &args, address);
            break;
        default:
            printf("Error: invalid operation.\n");
            break;
        }
        if (args.v) printf("\n");
    }

    fclose(fp);
    free_cache(&cache, &args);

    printSummary(hits, misses, evictions);
    return 0;
}

void print_help_and_exit(char *argv[])
{
    printf("Usage: %s [-hv] -s <num> -E <num> -b <num> -t <file>\n", argv[0]);
    printf("Options:\n");
    printf("  -h\tPrint this help message.\n");
    printf("  -v\tOptional verbose flag.\n");
    printf("  -s <num>\tNumber of set index bits.\n");
    printf("  -E <num>\tNumber of lines per set.\n");
    printf("  -b <num>\tNumber of block offset bits.\n");
    printf("  -t <file>\tTrace file.\n\n");
    printf("Examples:\n");
    printf("  linux>  ./%s -s 4 -E 1 -b 4 -t traces/yi.trace\n", argv[0]);
    printf("  linux>  ./%s -v -s8 -E 2 -b 4 -t traces/yi.trace\n", argv[0]);
    exit(0);
}

void initialize_cache(Cache *cache, Args *args)
{
    int i, j;
    int S = 1 << args->s;
    int E = args->E;

    *cache = (CacheSet *) malloc(S * sizeof(CacheSet));

    if (*cache == NULL) exit(1);

    for (i = 0; i < S; i++) {
        (*cache)[i] = (CacheLine *) malloc(E * sizeof(CacheLine));

        if ((*cache)[i] == NULL) exit(1);

        for (j = 0; j < E; j++) {
            (*cache)[i][j].valid = 0;
            (*cache)[i][j].tag = -1;
            (*cache)[i][j].times = 0;
        }
    }
}

void free_cache(Cache *cache, Args *args)
{
    int i;
    int S = 1 << args->s;

    for (i = 0; i < S; i++) {
        free((*cache)[i]);
    }
    free(*cache);
}

void get_args(int argc, char *argv[], Args *args)
{
    int opt;
    char *s_val = NULL;
    char *e_val = NULL;
    char *b_val = NULL;
    char *filename = NULL;

    while ((opt = getopt(argc, argv, "hvs:E:b:t:")) != -1) {
        switch(opt) {
        case 'h':
            print_help_and_exit(argv);
            break;
        case 'v':
            args->v = 1;
            break;
        case 's':
            s_val = optarg;
            break;
        case 'E':
            e_val = optarg;
            break;
        case 'b':
            b_val = optarg;
            break;
        case 't':
            filename = optarg;
            break;
        default:
            print_help_and_exit(argv);
        }
    }

    if (s_val == NULL || e_val == NULL || b_val == NULL || filename == NULL)
        print_help_and_exit(argv);

    args->s = atoi(s_val);
    args->E = atoi(e_val);
    args->b = atoi(b_val);
    args->t = filename;
}

void process(Cache *cache, Args *args, unsigned address)
{
    int i, index, tag;
    CacheSet cs;
    int max_index, max_times;
    long mask = 0xffffffffffffffff >> (64 - args->s);

    address >>= args->b;
    index = mask & address;
    address >>= args->s;
    tag = address;

    cs = (*cache)[index];
    for (i = 0; i < args->E; i++) {
        if (cs[i].valid)
            cs[i].times++;
    }

    for (i = 0; i < args->E; i++) {
        if (cs[i].valid && cs[i].tag == tag) {
            if (args->v) printf(" hit");
            hits++;
            cs[i].times = 0;
            return;
        }
    }

    misses++;
    if (args->v) printf(" miss");
    for (i = 0; i < args->E; i++) {
        if (!cs[i].valid) {
            cs[i].valid = 1;
            cs[i].tag = tag;
            cs[i].times = 0;
            return;
        }
    }

    evictions++;
    if (args->v) printf(" eviction");
    max_index = 0;
    max_times = cs[0].times;
    for (i = 0; i < args->E; i++) {
        if (cs[i].times > max_times) {
            max_index = i;
            max_times = cs[i].times;
        }
    }
    cs[max_index].tag = tag;
    cs[max_index].times = 0;
}
