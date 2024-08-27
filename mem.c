/* mem.c         <<-- A template 
 * Aarav Rekhi        <<-- many updates required
 * aaravr
 * ECE 2230 Spring 2024
 * MP4
 *
 * Propose: A template for MP4
 *
 * Assumptions: 
 *
 * Bugs:
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <math.h>

#include "mem.h"

/* Chunk definition (do not modify).
 * sizeof(Chunk) must equal SIZEOF_CHUNK_T in mem.h.
 */
typedef struct chunk_tag {
    struct chunk_tag *next;  /* pointer to the next free block */
    unsigned long size;               /* size of memory block in units of
                                sizeof(Chunk) */
} Chunk;


typedef struct heap_stats {

    /* do not include the dummy block when computing the next 4 */
    int numItems;    /* number of chunks in the free list   */
    int min;         /* size of the smallest chunk, in bytes */
    int max;         /* size of the largest chunk, in bytes  */
    int average;     /* average size of the chunks, in bytes */
    int totalBytes;  /* total size of all chunks, in bytes   */

    /* the following two fields are updated in morecore() */
    int numSbrkCalls;  /* number of successful calls to sbrk()  */
    int numPages;      /* number of pages allocated with sbrk() */
} HeapStats;

/* global variables exported via mem.h */
int SearchPolicy = FIRST_FIT;
int SearchLoc = HEAD_FIRST;
int Coalescing = FALSE;

/* global variables restricted to mem.c only */
static Chunk Dummy = {
    &Dummy,  /* next */
    0        /* size */
};
static Chunk * Rover = &Dummy;
static HeapStats stats;  /* initialized by the O/S to all 0s */


/* prototypes for functions private to mem.c */
void mem_validate(void);
Chunk *morecore(int);
void test1(void);
void test1()
{
	void *mem;
	mem = Mem_alloc(1);
	printf("\nsize = %d\n\n", Dummy.next->size);
}

/* function to request 1 or more pages from the operating system.
 *
 * new_bytes must be the number of bytes that are being requested from
 *           the OS with the sbrk command.  It must be an integer 
 *           multiple of the PAGESIZE
 *
 * returns a pointer to the new memory location.  If the request for
 * new memory fails this function simply returns NULL, and assumes some
 * calling function will handle the error condition.  Since the error
 * condition is catastrophic, nothing can be done but to terminate 
 * the program.
 */
Chunk *morecore(int new_bytes) 
{
    char *cp;
    Chunk *new_p;

    /* assert preconditions */
    assert(new_bytes % PAGESIZE == 0 && new_bytes > 0);
    assert(PAGESIZE % sizeof(Chunk) == 0);

    /* allocate memory using sbrk() */
    cp = sbrk(new_bytes);
    if (cp == (char *) -1)  /* no space available */
        return NULL;
    new_p = (Chunk *) cp;
    
    /* update statistics */
    stats.numSbrkCalls++;
    stats.numPages += new_bytes/PAGESIZE;

    return new_p;
}

/* TODO update this documentation according to the programming guide */
void Mem_free(void *return_ptr)
{
    /* TODO obviously the next line is WRONG!!!!  You must fix it. */
    if (return_ptr == NULL)
    {
        return;
    }
    else if (Coalescing == FALSE)
    {
  	    // Set return_pos to point to the header
	    Chunk *header = (Chunk*) (return_ptr - SIZEOF_CHUNK_T);
        Chunk *temp = Rover->next;
        Rover->next = header;
        header->next = temp;
        Rover = temp;
        temp = NULL;
    }
    else
    {
	    // Find the header of the node being freed
	    Chunk *header = (Chunk*)(return_ptr - SIZEOF_CHUNK_T);
	    // Find the node before header
    	Chunk *finder = &Dummy;
	    while ((finder->next < header + header->size + 1) && finder->next != &Dummy)
	    {
		    finder = finder->next;
	    }
	    // Insert header into the free list
	    header->next = finder->next;
	    finder->next = header;
	    // Check if node at end can be coalesced
	    if ((header + header->size + 1) == header->next && header->next != &Dummy)
	    {
		    Chunk *temp = header->next;
		    header->size += temp->size + 1;
		    header->next = temp->next;
		    temp->next = NULL;
    		temp = NULL;
	    }
	    // Check if node in front can be coalesced
	    if ((finder + finder->size + 1) == header && finder != &Dummy)
	    {
		    finder->size += header->size + 1;
		    finder->next = header->next;
		    header->next = NULL;
	    }
	    Rover = finder;
	    header = NULL;
    }
}

/* TODO update this documentation */
void *Mem_alloc(int nbytes) {
    /* assert preconditions */
    assert(nbytes > 0);
    // Determine how many chunk sized units are needed
    long nunits = (nbytes / SIZEOF_CHUNK_T) + 1;
    if (nbytes % SIZEOF_CHUNK_T != 0)
    {
        nunits++;
    }
    // Save search start location
    Chunk *start;
    if (SearchLoc == HEAD_FIRST)
    {
        Rover = &Dummy;
        start = Rover;
    }
    else if (SearchLoc == ROVER_FIRST)
    {
        start = Rover;
    }
    else
    {
        printf("Invalid search position.\n");
        exit(1);
    }
    // Search for a big enough block according to search policy
    Chunk *removalSpot = NULL;
    long removalSize = Rover->size;
    // First fit
    if (SearchPolicy == FIRST_FIT)
    {
        do {
            // Keep the first chunk that is big enough
            if (Rover->size >= nunits-1)
            {
                removalSize = Rover->size;
                removalSpot = Rover;
            }
            Rover = Rover->next;
        } while (removalSpot == NULL && Rover != start);
    }
    // Best fit
    else if (SearchPolicy == BEST_FIT)
    {
        removalSpot = Rover;
        do {
            if (Rover->size >= nunits-1)
            {
		        if (removalSize < nunits-1 || Rover->size < removalSize)
		        {
          	        removalSize = Rover->size;
          	        removalSpot = Rover;
		        }
            }
            Rover = Rover->next;
        } while (removalSize != nunits-1 && Rover != start);
        // If best fit is still too small, set removalSpot to NULL
        if (removalSize < nunits-1)
        {
            removalSpot = NULL;
        }
    }
    // Worst fit
    else if (SearchPolicy == WORST_FIT)
    {
        long max_size = removalSize;
        removalSpot = Rover;
        // Find the biggest free chunk
        do {
            // If a bigger chunk is found, save that size and position
            if (Rover->size > max_size)
            {
            removalSize = Rover->size;
            removalSpot = Rover;
            }
            Rover = Rover->next;
        } while (Rover != start);
        // If max size is still too small, set removalSpot to NULL
        if (removalSize < nunits-1)
        {
            removalSpot = NULL;
        }
    }
    // Error
    else
    {
        printf("Not a valid search policy.\n");
        exit(1);
    }
    // If a large enough chunk was not found, add memory to the
    // free list using morecore
    if (removalSpot == NULL)
    {
        int new_page = nbytes;
        // Increase removalSize to be a multiple of PAGESIZE
        if (new_page % PAGESIZE != 0)
        {
            new_page += PAGESIZE - (new_page % PAGESIZE);
        }
        Chunk *temp = Rover->next;
        removalSpot = morecore(new_page);
        removalSize = (new_page/SIZEOF_CHUNK_T) - 1;
        removalSpot->size = removalSize;
        Rover->next = removalSpot;
        removalSpot->next = temp;
	    removalSpot->size = removalSize;
        temp = NULL;
    }
    // If morecore returned NULL, return NULL
    if (removalSpot == NULL)
    {
        return NULL;
    }
    // User Rover to find chunk before removalSpot
    while (Rover->next != removalSpot)
    {
        Rover = Rover->next;
    }
    // Adjust header block for chunk that will remain
    if (removalSize > nunits-1)
    {
        // Skip over chunk being removed
        Rover->next = removalSpot + nunits;
        Rover = Rover->next;
        // Connect new header to the free list
        Rover->next = removalSpot->next;
        Rover->size = removalSize - nunits;
        // Remove old header to correct size
	    removalSpot->size = nunits-1;
    }
    // Skip that if chunk was a perfect fit
    else{
        Rover->next = removalSpot->next;
    }
    // Return pointer to free Memory
    return (removalSpot + 1);
}

/* prints stats about the current free list
 *
 * -- number of items in the linked list including dummy item
 * -- min, max, and average size of each item (in bytes)
 * -- total memory in list (in bytes)
 * -- number of calls to sbrk and number of pages requested
 *
 * A message is printed if all the memory is in the free list
 */
void Mem_stats(void)
{
    /* TODO calculate the latest stats and put them in the stats struct */
    // Set all values according to the first node
    Chunk *search_pointer = Dummy.next;
    stats.numItems = 1;
    stats.min = search_pointer->size;
    stats.max = search_pointer->size;
    stats.totalBytes = 0;
    // Iterate through all nodes and find values in units of chunks
    while (search_pointer != &Dummy)
    {
        stats.numItems++;
        if (search_pointer->size < stats.min)
        {
            stats.min = search_pointer->size;
        }
        else if (search_pointer->size > stats.max)
        {
            stats.max = search_pointer->size;
        }
        stats.totalBytes += (search_pointer->size + 1);
        search_pointer = search_pointer->next;
    }
    // Convert from chunks to bytes
    stats.min *= SIZEOF_CHUNK_T;
    stats.max *= SIZEOF_CHUNK_T;
    stats.totalBytes *= SIZEOF_CHUNK_T;
    // Calculate average size
    stats.average = stats.totalBytes / stats.numItems;
    /* ======= DO NOT MODIFY FROM HERE TO END OF Mem_stats() ======= */
    printf("\n\t\tMP4 Heap Memory Statistics\n"
            "----------------------------------------------------\n\n"
            "\tNumber of blocks in free list: %d\n"
            "\tMin: %d\n"
            "\tMax: %d\n"
            "\tAverage: %d\n"
            "\tTotal bytes in free list: %d\n"
            "\tNumber of sbrk calls: %d\n"
            "\tNumber of requested pages: %d\n"
            "\tHeap status:  %s\n",
            stats.numItems, stats.min, stats.max, stats.average, stats.totalBytes,
            stats.numSbrkCalls, stats.numPages,
            stats.totalBytes == stats.numPages*PAGESIZE ? \
            "all memory is in the heap -- no leaks are possible\n"\
            : "heap is in-use -- leaks are possible\n");
}

/* print table of memory in free list 
 *
 * The print should include the dummy item in the list 
 */
void Mem_print(void)
{
    assert(Rover != NULL && Rover->next != NULL);
    Chunk *p = Rover;
    Chunk *start = p; /* save my starting block */
    do {
        printf("p=%p, size=%ld, end=%p, next=%p %s\n", 
                p, p->size, p + p->size, p->next, p->size!=0?"":"<-- dummy");
        p = p->next;
    } while (p != start); /* only 1 time through the list */
    // mem_validate();
}

/* This is an experimental function to attempt to validate the free
 * list when coalescing is used.  It is not clear that these tests
 * will be appropriate for all designs.  If your design utilizes a different
 * approach, that is fine.  You do not need to use this function and you
 * are not required to write your own validate function.
 */
void mem_validate(void)
{
    assert(Rover != NULL && Rover->next != NULL);
    assert(Rover->size >= 0);
    int wrapped = FALSE;
    int found_dummy = FALSE;
    int found_rover = FALSE;
    Chunk *p, *largest, *smallest;

    /* for validate begin at Dummy */
    p = &Dummy;
    do {
        if (p->size == 0) {
            assert(found_dummy == FALSE);
            found_dummy = TRUE;
        } else {
            assert(p->size > 0);
        }
        if (p == Rover) {
            assert(found_rover == FALSE);
            found_rover = TRUE;
        }
        p = p->next;
    } while (p != &Dummy);
    assert(found_dummy == TRUE);
    assert(found_rover == TRUE);

    if (Coalescing) {
        do {
            if (p >= p->next) {
                /* this is not good unless at the one wrap */
                if (wrapped == TRUE) {
                    printf("validate: List is out of order, already found wrap\n");
                    printf("first largest %p, smallest %p\n", largest, smallest);
                    printf("second largest %p, smallest %p\n", p, p->next);
                    assert(0);   /* stop and use gdb */
                } else {
                    wrapped = TRUE;
                    largest = p;
                    smallest = p->next;
                }
            } else {
                assert(p + p->size < p->next);
            }
            p = p->next;
        } while (p != &Dummy);
        assert(wrapped == TRUE);
    }
}