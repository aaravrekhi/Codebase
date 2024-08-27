/* lab4.c
 * Aarav Rekhi          <<-- updates required
 * aaravr
 * ECE 2230 Spring 2024
 * MP4
 *
 * Purpose:
 * --------
 *
 * This file contains drivers to test the mem.c package for dynamic memory
 * allocation.
 * 
 * The following parameters can be set on the command line.  If not set,
 * default values are used.  
 *
 * If different options are implemented for the memory package, this provides a
 * simple mechanism to change the options.  
 *
 * -f best|first|worst  search policy to find memory block (first by default)
 * -c                   turn on coalescing (off by default)
 *
 * General options for all test drivers
 * -s 19283  random number generator seed [ NOT USED ]
 *
 * The Unit test drivers.  Two example drivers are provided.  You MUST
 *           add one or more additional unit drivers for more detailed testing
 * 
 * -u 0      Tests one allocation for 16 bytes
 * -u 1      Tests 4 allocations including a new page
 *           The student must update this driver to match the details of
 *           his or her design.
 *
 * -u ?      The student is REQUIRED to add additional drivers
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <ctype.h>

#include "mem.h"

/* prototypes for functions in this file only */
void getCommandLine(int, char**, int*, int*);

int main(int argc, char **argv)
{
    int unit_driver;  /* the unit test number to run */
    int seed; //= 72467;         /* random number seed */
    
    getCommandLine(argc, argv, &unit_driver, &seed);
    printf("Seed: %d\n", seed);
    drand48(seed);

    // The major choices: search policy and coalescing option 
    if (SearchPolicy == BEST_FIT) printf("Best-fit search policy");
    else if (SearchPolicy == FIRST_FIT) printf("First-fit search policy");
    else if (SearchPolicy == WORST_FIT) printf("Worst-fit search policy");
    else {
        fprintf(stderr, "Error with undefined search policy\n");
        exit(1);
    }
    if (SearchLoc == HEAD_FIRST) printf(" starting at head");
    else if (SearchLoc == ROVER_FIRST) printf( "starting at rover");
    else {
        fprintf(stderr, "Error with undefined search starting location\n");
        exit(1);

    }
    if (Coalescing == TRUE) printf(" using coalescing\n");
    else if (Coalescing == FALSE) printf(" without coalescing\n");
    else {
        fprintf(stderr, "Error specify coalescing policy\n");
        exit(1);
    }

    if (unit_driver == 0)
    {
        // unit driver 0: basic test with one allocation and free
        printf("\n----- Begin unit driver 0 -----\n");
        char *string;
        const char msg[] = "hello world 15c";
        int len = strlen(msg);
        // add one for null character at end of string 
        string = (char *) Mem_alloc((len+1) * sizeof(char));
        strcpy(string, msg);
        printf("string length=%d\n:%s:\n", len, string);
        printf("\nFree list after first allocation\n");
        Mem_stats();
        Mem_print();
        Mem_free(string);
        printf("\nFree list after first free\n");
        printf("unit driver 0 has returned all memory to free list\n");
        Mem_stats();
        Mem_print();
        string = NULL;
        printf("\n----- End unit driver 0 -----\n");
    }
    else if (unit_driver == 1)
    {
        printf("\n----- Begin unit driver 1 -----\n");
        /* You MUST create at least three new unit drivers.

         Here is an example of a unit test driver.

         This test makes four allocations from the free list with the goal
         of making the third allocation the correct size so that the free
         list is left empty. The fourth allocation should allocate a new
         page. To get this test to work with your code you need to 
         determine two values.
         */
        int unit_size = SIZEOF_CHUNK_T;
        int units_in_first_page = PAGESIZE/unit_size;
        assert(units_in_first_page * unit_size == PAGESIZE);
        printf("There are %d units per page, and the size of Chunk is %d bytes\n", 
                units_in_first_page, unit_size); 

        int *p1, *p2, *p3, *p4;
        int num_bytes_1, num_bytes_2, num_bytes_3;
        int num_bytes_4;

        // allocate 1st pointer to 1/8 of a page
        num_bytes_1 = (units_in_first_page/8 - 1)*unit_size;
        p1 = (int *) Mem_alloc(num_bytes_1);
        printf("first: %d bytes (%d units) p=%p \n", 
                num_bytes_1, num_bytes_1/unit_size, p1);
        Mem_print();
         
        // allocate for 2nd pointer to 1/2 of a page
        num_bytes_2 = (units_in_first_page/2 - 1)*unit_size;
        p2 = (int *) Mem_alloc(num_bytes_2);
        printf("second: %d bytes (%d units) p=%p \n", 
                num_bytes_2, num_bytes_2/unit_size, p2);
        Mem_print();

        // allocate remaining memory in free list
        num_bytes_3 = units_in_first_page - num_bytes_1/unit_size 
            - num_bytes_2/unit_size - 3;
        num_bytes_3 *= unit_size;
        p3 = (int *) Mem_alloc(num_bytes_3);
        printf("third: %d bytes (%d units) p=%p \n", 
                num_bytes_3, num_bytes_3/unit_size, p3);
        Mem_print();
        printf("unit driver 1: above Mem_print shows empty free list\n");

        // allocate for 4th pointer to 1/4 a page when free list is empty
        num_bytes_4 = (units_in_first_page/4 - 1)*unit_size;
        p4 = (int *) Mem_alloc(num_bytes_4);
        printf("fourth: %d bytes (%d units) p=%p \n", 
                num_bytes_4, num_bytes_4/unit_size, p4);
        Mem_print();

        // next put the memory back into the free list:

        printf("first free of 1/8 a page p=%p \n", p1);
        Mem_free(p1);
        Mem_print();

        printf("second free of 3/8 a page p=%p \n", p3);
        Mem_free(p3);
        Mem_print();

        printf("third free of 1/2 a page p=%p \n", p2);
        Mem_free(p2);
        Mem_print();
        printf("fourth free of 1/4 a page p=%p\n", p4);
        Mem_free(p4);
        printf("unit driver 1 has returned all memory to free list\n");
        Mem_print();
        Mem_stats();
        printf("\n----- End unit test driver 1 -----\n");
    }


    // add your unit test drivers here to test for special cases such as
    //   -- request the number of bytes that matches a whole page, and a 
    //      size that is one unit smaller than a page
    //   -- request more bytes than in one page
    //   -- combinations of requests and frees such that the free list is empty
    //   -- demonstrate all patterns in coalescing
    //   -- show that rover spreads allocatins in list and does not cluster
    //      fragments at head of the free list

	// Request a whole page and a size one unit smaller and bigger than a page
    else if (unit_driver == 2)
	{
        printf("\n----- Begin unit driver 2 -----\n");
        int unit_size = SIZEOF_CHUNK_T;
        int units_in_first_page = PAGESIZE/unit_size;
        assert(units_in_first_page * unit_size == PAGESIZE);
        printf("There are %d units per page, and the size of chunk_t is %d bytes\n",
                units_in_first_page, unit_size);

        int *p1, *p2, *p3;
        int num_bytes_1, num_bytes_2, num_bytes_3;
		

        // allocate 1st pointer to an entire  page
        num_bytes_1 = (units_in_first_page)*unit_size;
        p1 = (int *) Mem_alloc(num_bytes_1);
        printf("first: %d bytes (%d units) p=%p \n",
                num_bytes_1, num_bytes_1/unit_size, p1);
        Mem_print();

        // allocate for 2nd pointer to 1 less than a page
        num_bytes_2 = (units_in_first_page - 1)*unit_size;
        p2 = (int *) Mem_alloc(num_bytes_2);
        printf("second: %d bytes (%d units) p=%p \n",
                num_bytes_2, num_bytes_2/unit_size, p2);
        Mem_print();

        // allocate for 3rd pointer to 1 more than a page
        num_bytes_3 = (units_in_first_page + 1)*unit_size;
        p3 = (int *) Mem_alloc(num_bytes_3);
        printf("second: %d bytes (%d units) p=%p \n",
                num_bytes_3, num_bytes_3/unit_size, p3);
        Mem_print();

        // next put the memory back into the free list:

        printf("first free of entire page p=%p \n", p1);
        Mem_free(p1);
        Mem_print();

        printf("second free of 1 less than a page p=%p \n", p2);
        Mem_free(p2);
        Mem_print();

        printf("third free of 1 more than a page p=%p \n", p3);
        Mem_free(p3);
        Mem_print();

        printf("unit driver 2 has returned all memory to free list\n");
        Mem_print();
        Mem_stats();
        printf("\n----- End unit test driver 2 -----\n");
	}
	// Request sizes that require several pages
    else if (unit_driver == 3)
	{
        printf("\n----- Begin unit driver 3 -----\n");
        int unit_size = SIZEOF_CHUNK_T;
        int units_in_first_page = PAGESIZE/unit_size;
        assert(units_in_first_page * unit_size == PAGESIZE);
        printf("There are %d units per page, and the size of chunk_t is %d bytes\n",
                units_in_first_page, unit_size);

        int *p1, *p2, *p3;
        int num_bytes_1, num_bytes_2, num_bytes_3;


        // allocate 1st pointer to over one page
        num_bytes_1 = (units_in_first_page + 20)*unit_size;
        p1 = (int *) Mem_alloc(num_bytes_1);
        printf("first: %d bytes (%d units) p=%p \n",
                num_bytes_1, num_bytes_1/unit_size, p1);
        Mem_print();

        // allocate for 2nd pointer to exactly 2 pages
        num_bytes_2 = (2*units_in_first_page)*unit_size;
        p2 = (int *) Mem_alloc(num_bytes_2);
        printf("second: %d bytes (%d units) p=%p \n",
                num_bytes_2, num_bytes_2/unit_size, p2);
        Mem_print();

        // allocate for 3rd pointer to over 5 pages
        num_bytes_3 = (5*units_in_first_page + 1)*unit_size;
        p3 = (int *) Mem_alloc(num_bytes_3);
        printf("second: %d bytes (%d units) p=%p \n",
                num_bytes_3, num_bytes_3/unit_size, p3);
        Mem_print();

        // next put the memory back into the free list:

        printf("first free of over a page p=%p \n", p1);
        Mem_free(p1);
        Mem_print();

        printf("second free of exactly 2 pages p=%p \n", p2);
        Mem_free(p2);
        Mem_print();

        printf("third free of more than 5 pages p=%p \n", p3);
        Mem_free(p3);
        Mem_print();

        printf("unit driver 3 has returned all memory to free list\n");
        Mem_print();
        Mem_stats();
        printf("\n----- End unit test driver 3 -----\n");
	}
	// Combines requests and frees throughout execution
    else if (unit_driver == 4)
	{
        printf("\n----- Begin unit driver 4 -----\n");
        int unit_size = SIZEOF_CHUNK_T;
        int units_in_first_page = PAGESIZE/unit_size;
        assert(units_in_first_page * unit_size == PAGESIZE);
        printf("There are %d units per page, and the size of chunk_t is %d bytes\n",
                units_in_first_page, unit_size);

        int *p1, *p2, *p3, *p4, *p5;
        int num_bytes_1, num_bytes_2, num_bytes_3, num_bytes_4, num_bytes_5;

        // allocate 1st pointer to 1/4 of a page
        num_bytes_1 = (units_in_first_page/4)*unit_size;
        p1 = (int *) Mem_alloc(num_bytes_1);
        printf("first: %d bytes (%d units) p=%p \n",
                num_bytes_1, num_bytes_1/unit_size, p1);
        Mem_print();

        // allocate for 2nd pointer to 1/8 of a page
        num_bytes_2 = (units_in_first_page/8)*unit_size;
        p2 = (int *) Mem_alloc(num_bytes_2);
        printf("second: %d bytes (%d units) p=%p \n",
                num_bytes_2, num_bytes_2/unit_size, p2);
        Mem_print();

		// Free first allocated memory
        printf("first free of 1/4 of a page p=%p \n", p1);
        Mem_free(p1);
        Mem_print();

        // allocate for 3rd pointer to 1/8 of a page
        num_bytes_3 = (units_in_first_page/8)*unit_size;
        p3 = (int *) Mem_alloc(num_bytes_3);
        printf("second: %d bytes (%d units) p=%p \n",
                num_bytes_3, num_bytes_3/unit_size, p3);
        Mem_print();

        // allocate 4th pointer to half a page
        num_bytes_4 = (units_in_first_page/2)*unit_size;
        p4 = (int *) Mem_alloc(num_bytes_4);
        printf("first: %d bytes (%d units) p=%p \n",
                num_bytes_4, num_bytes_4/unit_size, p4);
        Mem_print();

		// Free third allocated memory
        printf("third free of 1/8 of a page p=%p \n", p3);
        Mem_free(p3);
        Mem_print();

		// Free second allocated memory
        printf("second free of 1/8 of a page p=%p \n", p2);
        Mem_free(p2);
        Mem_print();

        // allocate for 5th pointer to over 2 pages
        num_bytes_5 = (2*units_in_first_page + 1)*unit_size;
        p5 = (int *) Mem_alloc(num_bytes_5);
        printf("second: %d bytes (%d units) p=%p \n",
                num_bytes_5, num_bytes_5/unit_size, p5);
        Mem_print();

		// Free fifth allocated memory
        printf("fifth free of over 2 pages p=%p \n", p5);
        Mem_free(p5);
        Mem_print();

		// Free fourth allocated memory
        printf("fourth free of 1/2 of a page p=%p \n", p4);
        Mem_free(p4);
        Mem_print();

        printf("unit driver 4 has returned all memory to free list\n");
        Mem_print();
        Mem_stats();
        printf("\n----- End unit test driver 4 -----\n");
	}

    return 0;
}

/* read in command line arguments.  Note that Coalescing and SearchPolicy 
 * are stored in global variables for easy access by other 
 * functions.
 *
 * -u 0  is for the unit drivers, starting with driver 0
 *
 *  argc:           number of command line arguments
 *  argv:           array of the command line arguments
 *  unit_driver:    is set to the number for the unit test
 *  seed:           is set to the seed for the random number generator
 */
void getCommandLine(int argc, char **argv, int *unit_driver, int *seed)
{
    /* The geopt function creates three global variables:
     *    optopt--if an unknown option character is found
     *    optind--index of next element in argv 
     *    optarg--argument for option that requires argument 
     *
     * The third argument to getopt() specifies the possible argument flags
     *   If an argument flag is followed by a colon, then an argument is 
     *   expected. E.g., "x:y" means -x must have an argument but not -y
     */
    int c;
    int index;

    while ((c = getopt(argc, argv, "s:f:u:h:c")) != -1)
        switch(c) {
            case 'u': *unit_driver = atoi(optarg);     break;
            case 's': *seed = atoi(optarg);            break;
            case 'c': Coalescing = TRUE;               break;
            case 'f':
                if (strcmp(optarg, "best") == 0)
                    SearchPolicy = BEST_FIT;
                else if (strcmp(optarg, "first") == 0)
                    SearchPolicy = FIRST_FIT;
                else if (strcmp(optarg, "worst") == 0)
                    SearchPolicy = WORST_FIT;
                else {
                    fprintf(stderr, "invalid search policy: %s\n", optarg);
                    exit(1);
                }
                break;
            case 'h':
                if (strcmp(optarg, "rover") == 0)
                    SearchLoc = ROVER_FIRST;
                else if (strcmp(optarg, "head") == 0)
                    SearchLoc = HEAD_FIRST;
                else {
                    fprintf(stderr, "invalid search starting location: %s\n", optarg);
                    exit(1);
                }
                break;
            case '?':
                if (isprint(optopt))
                    fprintf(stderr, "Unknown option %c.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                /* fall through */
            default:
                printf("Lab4 command line options\n");
                printf("General options ---------\n");
                printf("  -c        turn on coalescing (default off)\n");
                printf("  -f best|first|worst\n");
                printf("            search policy to find memory block (first by default)\n");
                printf("  -h rover|head\n");
                printf("            starting location for search\n");
                printf("  -u 0      run unit test driver number 0\n");
                exit(1);
        }
    for (index = optind; index < argc; index++)
        printf("Non-option argument %s\n", argv[index]);
}
