/* task_support.h       <<-- A template
 * Aarav Rekhi        <<-- some changes needed
 * aaravr
 * ECE 2230 Spring 2024
 * MP3
 *
 * Propose: A template for MP3
 *
 * Assumptions: suggested structure definitions and public functions 
 *              as defined for assignment.  
 *
 *              You may change this file if needed for your design.
 *
 * Bugs:
 *
 */

#define MAXLINE 120

/* new functions for MP3 sortings */
void task_list_add_tail_mp3(List *, int id);
void task_list_sort(List **, int sort_type, int sort_order);
/* prototype function definitions */

/* function to compare task records */
int task_compare(const Task *rec_a, const Task *rec_b);

/* function to free memory in an task record */
void task_rec_cleanup(Task *rec);

/* functions to create and cleanup a task record list */
List * task_list_create(void);
void task_list_cleanup(List *);

/* Functions to get and print task information */
void task_list_print(List * list_ptr, char *); /* print list of records */
void task_list_print_head(List *list_ptr);     /* print record at head */
void task_list_stats(List *, List *, List *);        /* prints size of each list */


/* functions for task list that has a maximum size for the list */
void task_list_add(List *, int max_list_size);
void task_list_lookup_id(List * , int id);
void task_list_remove_id(List * , int id);

/* MP1 ops */
void task_list_update(List *, int id, int state); 
void task_list_schedule(List *, int id, int priority);
void task_list_determine(List *); 
void task_list_remove_finished(List *); 

/* New in MP2 */
void task_list_reverse(List *); 



/* functions for unsorted FIFO list that has no limit of the size of the list,
 * inserts at the tail, removes at the head, and does not check for duplicate
 * elements in the list */
void task_list_add_tail(List *);
void task_list_remove_head(List * );

Task* id_lookup(List* list_ptr, int id, int *index);
// void chomp(char *str);
// void fill_task_record(Task *new);
// int* getDetermineArgs(int* num_args);
// char* state2str(enum state state);

// void print_task_rec_short(Task *rec);
// void print_task_rec_long(Task *rec);

/* commands for vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */