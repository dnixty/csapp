/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 *
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

#define DEBUG 1

team_t team = {
    "selfstudy",
    "Dominik Stodolny",
    "dominik@stodolny.org",
    "",
    ""
};

static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void *find_fit(size_t asize);
static void place(void *bp, size_t asize);
static void insertfb(void *bp);
static void removefb(void *bp);
#ifdef DEBUG
static void print_fblk_list();
static int mm_checkheap(void);
static int find_fbp(void *fbp);
#endif

static char *heap_listp; /* Prologue block pointer */
static char *fblk_listp; /* Free blocks pointer */

/* Basic constants and macros */
#define WSIZE 4           /* Word and header/footer size (bytes) */
#define DSIZE 8           /* Double word size (bytes) */
#define CHUNKSIZE (1<<12) /* Extend heap by this amount (bytes) */

#define ALIGN(size) (((size) + (DSIZE-1)) & ~0x7)

#define MAX(x, y) ((x) > (y)? (x) : (y))

/* Pack a size and allocated bit into a word */
#define PACK(size, alloc) ((size) | (alloc))

/* Read and write a word at address p */
#define GET(p)      (*(unsigned int *)(p))
#define PUT(p, val) (*(unsigned int *)(p) = (val))

/* Read the size and allocated fields from address p */
#define GET_SIZE(p)  (GET(p) & ~0x7)
#define GET_ALLOC(p) (GET(p) & 0x1)

/* Given block ptr bp, compute address of its header and footer */
#define HDRP(bp) ((char *)(bp) - WSIZE)
#define FTRP(bp) ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE)
#define NXFB(bp) (bp)
#define PVFB(bp) ((char *)bp + WSIZE)

/* Given block ptr bp, compute address of next and previous blocks */
#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) - WSIZE)))
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE)))

/*
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    /* Create the initial empty heap */
    if ((heap_listp = mem_sbrk(8*WSIZE)) == (void *)-1)
        return -1;
    PUT(heap_listp, 0);                            /* Alignment padding */
    PUT(heap_listp + (1*WSIZE), PACK(2*DSIZE, 0)); /* Root header */
    PUT(heap_listp + (2*WSIZE), 0);                /* Root next free block */
    PUT(heap_listp + (3*WSIZE), 0);                /* Root prev free block */
    PUT(heap_listp + (4*WSIZE), PACK(2*DSIZE, 0)); /* Root footer */

    PUT(heap_listp + (5*WSIZE), PACK(DSIZE, 1)); /* Prologue header */
    PUT(heap_listp + (6*WSIZE), PACK(DSIZE, 1)); /* Prologue footer */
    PUT(heap_listp + (7*WSIZE), PACK(0, 1));     /* Epilogue header */
    fblk_listp = heap_listp + (2*WSIZE);
    heap_listp += (6*WSIZE);

#ifdef DEBUG
    printf("** mm_init() **\n");
#endif

    /* Extend the empty heap with a free block of CHUNKSIZE bytes */
    if ((extend_heap(CHUNKSIZE/WSIZE)) == NULL)
        return -1;

    return 0;
}

/*
 * extend_heap - Extend heap by words * WSIZE by creating new block. The
 * new block coalesces any preceding free block and then is being
 * inserted into block into the list of free blocks
 */
static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

#ifdef DEBUG
    printf("** extend_heap(%d) **\n", words);
#endif

    /* Allocate an even number of words to maintain alignment */
    size = (words % 2) ? (words+1) * WSIZE : words * WSIZE;
    if ((long)(bp = mem_sbrk(size)) == -1)
        return NULL;

    /* Initialize free block header/footer and the epilogue header */
    PUT(HDRP(bp), PACK(size, 0));         /* Free block header */
    PUT(FTRP(bp), PACK(size, 0));         /* Free block footer */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* New epilogue header */

    /* Coalesce if the previous block was free */
    return coalesce(bp);
}


/*
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t asize;               /* Adjusted block size */
    size_t extendsize;          /* Amount to extend heap if no fit */
    char *bp;

#ifdef DEBUG
    printf("** mm_malloc(%d) **\n", size);
#endif

    /* Ignore spurious requests */
    if (size == 0)
        return NULL;

    /* Adjust block size to include overhead and alignment reqs. */
    if (size <= 2*DSIZE)
        asize = 3*DSIZE;
    else
        asize = DSIZE * ((size + (DSIZE) + (DSIZE-1)) / DSIZE);

    /* Search the free list for a fit */
    if ((bp = find_fit(asize)) != NULL) {
        place(bp, asize);
        return bp;
    }

    /* No fit found. Get more memory and place the block */
    extendsize = MAX(asize, CHUNKSIZE);
    if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
        return NULL;
    place(bp, asize);
#ifdef DEBUG
    mm_checkheap();
#endif
    return bp;
}

/*
 * mm_free - Free an alocated block and coalesce other adjacent free
 * blocks
 */
void mm_free(void *bp)
{
    size_t size = GET_SIZE(HDRP(bp));

#ifdef DEBUG
    printf("** mm_free(%x) **\n", (unsigned int)bp);
#endif

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));

    coalesce(bp);
}

/*
 * coalesce - join other adjacent free blocks into the bp
 *
 */
static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

#ifdef DEBUG
    printf("** coalesce(%x) **\n", (unsigned int)bp);
#endif

    if (prev_alloc && next_alloc) { /* Case 1 */
        insertfb(bp);
    }

    else if (prev_alloc && !next_alloc) { /* Case 2 */
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));

        removefb(NEXT_BLKP(bp));
        insertfb(bp);

        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    }

    else if (!prev_alloc && next_alloc) { /* Case 3 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));

        removefb(PREV_BLKP(bp));
        insertfb(PREV_BLKP(bp));

        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));

        bp = PREV_BLKP(bp);
    }

    else {                      /* Case 4 */
        size += GET_SIZE(HDRP(PREV_BLKP(bp))) +
            GET_SIZE(FTRP(NEXT_BLKP(bp)));

        removefb(PREV_BLKP(bp));
        removefb(NEXT_BLKP(bp));
        insertfb(PREV_BLKP(bp));

        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));

        bp = PREV_BLKP(bp);
    }
#ifdef DEBUG
    print_fblk_list();
#endif

    return bp;
}

/*
 * find_fit - Find first free block with size larger or equal to asize.
 */
static void *find_fit(size_t asize)
{
    void *bp = fblk_listp;

#ifdef DEBUG
    printf("** find_fit(%d) **\n", asize);
#endif

    while (1) {
#ifdef DEBUG
        printf("Checking: %x\n", (unsigned int)bp);
#endif
        if (asize <= GET_SIZE(HDRP(bp)))
            return bp;
        if (GET(NXFB(bp)) == 0)
            break;
        bp = (void *)GET(NXFB(bp));
    }

    return NULL;                /* No fit */
}

/*
 * place - Alocate free block and remove it from fblk_listp. If the
 * remainer is smaller than 3*DSIZE then allocate the whole block.
 */
static void place(void *bp, size_t asize)
{

    size_t csize = GET_SIZE(HDRP(bp));

#ifdef DEBUG
    printf("** place(%x, %d) **\n", (unsigned int)bp, asize);
#endif

    removefb(bp);

    if ((csize - asize) >= (3*DSIZE)) {
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));

        bp = NEXT_BLKP(bp);

        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));
        insertfb(bp);
    } else {
        PUT(HDRP(bp), PACK(csize, 1));
        PUT(FTRP(bp), PACK(csize, 1));
    }
}

/*
 * insertfb - Insert given block into the beginning of fblk_listp
 */
static void insertfb(void *bp)
{
#ifdef DEBUG
    printf("** insertfb(%x) **\n", (unsigned int)bp);
#endif

    PUT(NXFB(bp), GET(NXFB(fblk_listp)));
    if (GET(NXFB(fblk_listp)) != 0)
        PUT(PVFB(GET(NXFB(fblk_listp))), (unsigned int)bp);
    PUT(NXFB(fblk_listp), (unsigned int)bp);
    PUT(PVFB(bp), (unsigned int)fblk_listp);
}

static void removefb(void *bp)
{
    unsigned int next_fblk = GET(NXFB(bp));
    unsigned int prev_fblk = GET(PVFB(bp));

#ifdef DEBUG
    printf("** removefb(%x) **\n", (unsigned int)bp);
#endif

    if (prev_fblk != 0)
        PUT(NXFB(prev_fblk), next_fblk);
    if (next_fblk != 0)
        PUT(PVFB(next_fblk), prev_fblk);

#ifdef DEBUG
    print_fblk_list();
#endif

}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *bp, size_t size)
{
    void *oldptr = bp;
    void *newptr;
    size_t copySize;

#ifdef DEBUG
    printf("** mm_realloc(%x, %d) **\n", (unsigned int)bp, size);
#endif

    newptr = mm_malloc(size);
    if (newptr == NULL)
      return NULL;
    copySize = *(size_t *)((char *)oldptr - WSIZE);
    if (size < copySize)
      copySize = size;
    memcpy(newptr, oldptr, copySize);
    mm_free(oldptr);
    return newptr;

}

#ifdef DEBUG
/*
 * print_fblk_list - Print all free blocks
 */
static void print_fblk_list(void)
{
    void *bp = fblk_listp;

    while (1) {
        printf("\t[%x]\tH:%d F:%d N:%x P:%x\n",
               (unsigned int)bp, GET(HDRP(bp)), GET(FTRP(bp)), GET(NXFB(bp)), GET(PVFB(bp)));
        if (GET(NXFB(bp)) == 0)
            break;
        bp = (void *)GET(NXFB(bp));
    }
    printf("\n");
}

static int mm_checkheap(void)
{
    void *bp, *fbp;

    /* Check if the prologue block is correct */
    if (heap_listp != (char *)mem_heap_lo() + (6*WSIZE)) {
        printf("heap error: the prologue block is not in the correct position.\n");
        exit(1);
    }

    /* Check if the epilogue block is correct */
    if (GET(HDRP(mem_heap_hi()+1)) != 1) {
        printf("heap error: the epilogue block is not in the correct position.\n");
        exit(1);
    }

    for (bp = heap_listp; GET_SIZE(HDRP(bp)) != 0; bp = NEXT_BLKP(bp)) {
        /* Check if the header matches the footer  */
        if (GET(HDRP(bp)) != GET(FTRP(bp))) {
            printf("heap error: %x header and footer are different\n", (unsigned int)bp);
            exit(1);
        }
        /* Check if payload is aligned properly */
        if (((bp - mem_heap_lo()) % DSIZE) != 0) {
            printf("heap error: block %x is not aligned to %d bytes\n", (unsigned int)bp, DSIZE);
            exit(1);
        }

        if (GET_ALLOC(HDRP(bp)) == 0) {
            /* Check if all free blocks are in the fblk_listp */
            if(find_fbp(bp) != 0) {
                printf("heap error: free block %x is not in the fblk_listp.\n", (unsigned int)bp);
                exit(1);
            }
            /* Check that there are no contiguous free blocks in the
           memory */
            if (GET_ALLOC(HDRP(NEXT_BLKP(bp))) == 0) {
                printf("heap error: contiguous blocks %x and %x are free.\n", (unsigned int)bp, (unsigned int)NEXT_BLKP(bp));
                exit(1);
            }

        }

    }

    bp = fblk_listp;
    while (1) {
        if ((fbp = (void *)GET(NXFB(bp))) == 0)
            break;
        /* Check that next/prev pointers to consecutive free blocks are
           consistent */
        if (GET(PVFB(fbp)) != (unsigned int)bp) {
            printf("heap error: %x and %x next/prev pointers are inconsistent.\n", (unsigned int)bp, (unsigned int)fbp);
            exit(1);
        }
        /* Check that the free block is not allocated */
        if (GET_ALLOC(HDRP(bp)) == 1) {
            printf("heap error: free block %x is allocated.\n", (unsigned int)bp);
            exit(1);
        }
        bp = (void *)GET(NXFB(bp));
    }


    return 1;
}

/*
 * find_fbp - Find fbp in fblk_listp. Return 0 if exists, otherwise 1.
 */
static int find_fbp(void *fbp)
{
    void *bp = fblk_listp;

    while (1) {
        if ((unsigned int)fbp == (unsigned int)bp)
            return 0;
        if (GET(NXFB(bp)) == 0)
            return 1;
        bp = (void *)GET(NXFB(bp));
    }
}
#endif
