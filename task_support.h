/* task_support.h
 * Aarav Rekhi
 * aaravr
 * ECE 2230 Spring 2024
 * MP2
 *
 * Propose: A template for MP2 
 *
 * Assumptions: suggested structure definitions and public functions 
 *              as defined for assignmnet.  
 *
 *              You may change this file if needed for your design.
 *
 * Bugs:
 *
 */

#define MAXLINE 120

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
void task_list_stats(List *, List *);        /* prints size of each list */


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

/* commands for vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */