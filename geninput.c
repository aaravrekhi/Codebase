/* geninput.c
 * author: Aarav Rekhi
 * email: aaravr@clemson.edu
 * Spring 2024 ECE 2230
 *  
 *  Purpose: generates random input data for MP3
 *  
 *  Assumptions: MP3 is completed
 *               num_elements - number of elements to insert
 *               gen_method   - method of data generation:
 *                               3) random
 *                               2) ascending
 *                               1) descending
 *               sort_type    - number for sorting algorithm to use
 *               sort_order   - direction of sorted data
 *                
 *  Not for use as testlog
 *  
 * INSTRUCTIONS: gcc -Wall -g geninput.c -o geninput
 *               ./geninput command_num | valgrind ./lab3 list_size
 *               (valgrind is optional)
 *
 *
 *
 */
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<limits.h>


int main(int argc, char * argv[]) {

    int i, id;
    int num_elements, gen_method, sort_type, sort_order;
    time_t seed;
    seed = time(NULL);
    srand48(seed);

    // read the command line arguments
    if (argc != 5) {
        printf("Usage: ./geninput num_elements generation_method sort_type sort_order\n");
        exit(1);
    }
    num_elements = atoi(argv[1]);
    gen_method   = atoi(argv[2]);
    sort_type    = atoi(argv[3]);
    sort_order   = atoi(argv[4]);



    //print the commands to add "num_elements" to the list 
    for (i = 0; i < num_elements; i++) {

        if (gen_method == 3)      /* random */
            id = (int)(INT_MAX*drand48());
        else if (gen_method == 2) /* ascending */
            id = i;
        else if (gen_method == 1) /* decending */
            id = num_elements - i;
        
        printf("ADDTAIL %d\n", id);

    }

    //show stats, sort the list, and then end
    //printf("STATS\n");
    printf("SORT %d %d\n", sort_type, sort_order);
    printf("QUIT\n");

    return 0;
}