/* task_support.c         <<-- A template 
 * Aarav Rekhi          <<-- many updates required
 * aaravr 
 * ECE 2230 Spring 2024
 * MP2
 *
 * Propose: A template for MP2 
 *
 * Assumptions: 
 *
 * Bugs:
 *
 * You can modify the interface for any of the task_ functions if you like
 * But you must clearly document your changes.
 *
 * (You CANNOT modify any of the details of the list.h interface, or use any 
 *  of the private variables outside of list.c.)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "datatypes.h"
#include "list.h"
#include "task_support.h"

#define FALSE 0
#define TRUE  1

// private functions for use in task_support.c only
void fill_task_record(Task *new);          // collect input
int* getDetermineArgs(int* num_args);        // get input for "DETERMINE" cmd
char* state2str(enum state state);           // Converts enum to ASCII
void print_task_rec_short(Task *rec);      // print one record
void print_task_rec_long(Task *rec) ;      // print one record
Task *task_id(List *list_ptr, int id, int *index); // use id to find task

/* task_compare is required by the list ADT for sorted lists. 
 *
 * This function returns 
 *     1 if rec_a should be closer to the head than rec_b,
 *    -1 if rec_b is to be considered closer to the head, and 
 *     0 if the records are equal.
 *
 * For the task data we want to sort from lowest task ID up, so
 * closer to the front means a smaller task ID.
 *
 * The function expects pointers to two record structures, and it is an error
 * if either is NULL
 *
 * THIS FUNCTION SHOULD NOT BE CHANGED
 */
int task_compare(const Task *record_a, const Task *record_b)
{
    assert(record_a != NULL && record_b != NULL);
    if (record_a->task_id < record_b->task_id)
        return 1;
    else if (record_a->task_id > record_b->task_id)
        return -1;
    else
        return 0;

}

/* Function to pass into List ADT during construction to free the memory 
 * associate with a task record.  This hides the details of how the record
 * is defined from the List ADT.
 */
void task_rec_cleanup(Task *rec)
{
    if (rec != NULL) {
        free(rec->args);
        free(rec);
    }
}

/* This creates a list and it can be either a sorted or unsorted list
 *
 * This function is provided to you as an example of how to use 
 * function pointers.  
 */
List * task_list_create(void)
{
    return list_construct(task_compare, task_rec_cleanup);
}

/* this function frees the memory for either a sorted or unsorted list.
 */
void task_list_cleanup(List * list_ptr)
{
    list_destruct(list_ptr);
}

/* print one of the task record lists
 *
 * inputs: 
 *    list_ptr: a List * pointer to either sorted or unsorted list
 *
 *    type_of_list: a charter string that must be either "List" or "Queue"
 *
 * output: prints the list in the format required for grading.
 *         Do not change any of these lines (i.e. printfs && print_task_rec_short()
 */
void task_list_print(List * list_ptr, char *list_type)
{
    assert(strcmp(list_type, "List") == 0 || strcmp(list_type, "Queue") == 0);

    int num_in_list = list_size(list_ptr);
    Task *rec_ptr = NULL;

    if (num_in_list == 0)
        printf("%s empty\n", list_type);
    else{
        printf("%s has %d record%s\n", list_type, num_in_list, num_in_list == 1 ? "." : "s");
        for (int i = 0; i < num_in_list; i++){
            printf("%4d: ", i);
            rec_ptr = list_access(list_ptr, i);
            print_task_rec_short(rec_ptr);
        }
    }
}

/* the function takes a pointer to each list and prints the
 * number of items in each list
 */
void task_list_stats(List * sorted, List * unsorted)
{
	// TODO: get the number in list and size of the list
    int num_in_sorted_list = list_size(sorted);
    int num_in_unsorted_list = list_size(unsorted);
    int order_of_list = list_order(sorted); // Get sorted list order using list_order()
    
    printf("Number records: %d, Order: %s\n", num_in_sorted_list, 
            order_of_list == 1 ? "Ascending" : "Descending");
    printf("Number records in queue: %d\n", num_in_unsorted_list);
}








/* This function adds an task to the list.  
 *
 * Otherwise, if the list is full the task is rejected.
 */
void task_list_add(List * list_ptr, int max_list_size)
{
    Task *rec_ptr = (Task *)malloc(sizeof(Task));
    if (rec_ptr == NULL){
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    fill_task_record(rec_ptr);
    if (list_size(list_ptr) >= max_list_size){
        printf("Rejected: %d\n", rec_ptr->task_id);
        task_rec_cleanup(rec_ptr);
        return;
    }

#define UNORDERED 0
    if (list_order(list_ptr) == UNORDERED)
        list_insert(list_ptr, rec_ptr, list_size(list_ptr));
    else
        list_insert_sorted(list_ptr, rec_ptr);
    printf("Inserted: %d\n", rec_ptr->task_id);
}

/* This function prints the first task with the matching id in the
 * list.  
*/
void task_list_lookup_id(List * list_ptr, int id)
{
    int index = -1;
    Task *rec_ptr = task_id(list_ptr, id, &index);

    if (rec_ptr == NULL)
        printf("Did not find: %d\n", id);
    else{
        printf("Found: %d at index %d\n", id, index);
        print_task_rec_long(rec_ptr);
    }
}

/* This function removes the first task from the list with the matching
 * task id
 */
void task_list_remove_id(List * list_ptr, int id)
{
    int index = -1;
    Task *rec_ptr = task_id(list_ptr, id, &index);

    if (rec_ptr != NULL){
        list_remove(list_ptr, index);
        printf("Removed: %d\n", id);
        print_task_rec_long(rec_ptr);
        task_rec_cleanup(rec_ptr);
    }
    else
        printf("Did not remove: %d\n", id);
}

/* This function updates the state of the task identified by id if possible */
void task_list_update(List * list_ptr, int id, int state)
{
    int index = -1;
    Task *rec_ptr = task_id(list_ptr, id, &index);

    if (rec_ptr != NULL){
        enum state old_state = rec_ptr->state;
        if (old_state == FINISHED)
            assert(old_state == FINISHED);
        else if (old_state == QUEUED && state == RUNNING)
            rec_ptr->state = RUNNING;
        else if (old_state == RUNNING && (state == FINISHED || state == BLOCKED))
            rec_ptr->state = state;
        else if (old_state == BLOCKED && state == RUNNING)
            rec_ptr->state = RUNNING;
        printf("Task %d has state of %s\n", id, state2str(rec_ptr->state));
    } else
        printf("Task %d not found.\n", id);
    rec_ptr = NULL;
} 

/* This function schedules based on id and priority */
void task_list_schedule(List * list_ptr, int id, int priority)
{
    Task *rec_ptr = NULL;
    int length = list_size(list_ptr);
    int found = 0;

    for (int i = 0; i < length; i++){
        rec_ptr = (Task *)list_access(list_ptr, i);
        if (rec_ptr->task_id == id){
            found = 1;
            break;
        }
    }

    if (!found){
        for (int i = 0; i < length; i++){
            rec_ptr = (Task *)list_access(list_ptr, i);
            if (rec_ptr->priority == priority){
                found = 1;
                break;
            }
        }
    }


    if (found){
        if (rec_ptr->state == QUEUED){
            rec_ptr->state = RUNNING;
            printf("Now running task:\n");
            print_task_rec_short(rec_ptr);
        } else
            printf("Task is already running or finished.\n");   
    } else
        printf("Did not find task to schedule...\n");
}

/* This function determins if a task is runable */
void task_list_determine(List * list_ptr)
{
    int num_args;
    int *args = getDetermineArgs(&num_args);
    Task *task = NULL;
    int length = list_size(list_ptr);
    int found = 0;

    for (int i = 0; i < length && !found; i++){
        task = list_access(list_ptr, i);
        for (int j = 0; j < num_args && task != NULL; j++){
            if (task->args[j] != args[j])
                task = NULL;
        }
        
        if (task != NULL){
            printf("Task %d is runable.\n", task->task_id);
            found = 1;
        }
    }

    if (!found)
        printf("No runable tasks.\n");

    free(args);
}

void task_list_remove_finished(List * list_ptr)
{
    int deletions = 0;
    for (int i = 0; i < list_size(list_ptr);){
        Task *task = list_access(list_ptr, i);
        if (task->state == FINISHED){
            task_rec_cleanup(list_remove(list_ptr, i));
            deletions++;
        } else
            i++;
    }

    printf("Removing %d finished tasks\n", deletions);
}

/* This function reverses the order of the items in the list */
void task_list_reverse(List * list_ptr)
{
    list_reverse(list_ptr);
    int order_of_list = list_order(list_ptr);
    printf("List reversed, new order: %s\n",
           order_of_list == 1 ? "Ascending" : "Descending");
}




/****************************************************************************/
/****************************************************************************/

/*       Queue Functions                                                    */

/****************************************************************************/
/****************************************************************************/




/* This functions adds an task record to the tail of a list.  The list is
 * unsorted, and it does not have any limits on the maximum size of the list.
 */
void task_list_add_tail(List * list_ptr)
{
    Task *rec_ptr = (Task*) malloc(sizeof(Task));
    fill_task_record(rec_ptr); 
	list_insert(list_ptr, rec_ptr, list_size(list_ptr));
    printf("Appended %d onto queue\n", rec_ptr->task_id);
}

/* This function removes the task record at the head of the list.  The
 * list is unsorted, and there is no limit on the maximum size of the list.
 *
 * The task should be recycled so there are no memory leaks.
 */
void task_list_remove_head(List * list_ptr)
{
    Task *task_ptr = list_remove(list_ptr, 0);

    if (task_ptr == NULL){
        printf("Deleted head with task id and priority: %d %d\n", task_ptr->task_id, task_ptr->priority);
        task_rec_cleanup(task_ptr);
    } else
        printf("Queue empty, did not remove\n");
}

/* This function prints the task at the head of the queue.
 * Print all fields of the task record. 
 */
void task_list_print_head(List * list_ptr)
{
    Task *rec_ptr = list_access(list_ptr, 0);
    if (rec_ptr == NULL)
        printf("Queue is empty\n");
    else{
        printf("Found at head of queue:\n");
        print_task_rec_long(rec_ptr);
    }
}




/****************************************************************************/
/****************************************************************************/

/*       Private functions for the task list you declare                    */

/****************************************************************************/
/****************************************************************************/





Task* task_id(List* list_ptr, int id, int *location)
{
    Task *rec_ptr = NULL;
    int length = list_size(list_ptr);
    int i;
    for (i = 0; i < length; i++){
        rec_ptr = (Task *)list_access(list_ptr, i);
        if (rec_ptr->task_id == id){
            *location = i;
            return rec_ptr;
        }
    }

    *location = -1;
    return NULL;
}

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

/*       Functions from here to end of file should not be changed           */

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

/* If a string ends with an end-of-line \n, remove it. */

void chomp(char *str) 
{
    int lastchar = strlen(str) - 1;
    if (lastchar >= 0 && str[lastchar] == '\n') {
        str[lastchar] = '\0';
    }
}

/* Prompts user for task record input.
 * The input is not checked for errors but will default to an acceptable value
 * if the input is incorrect or missing.
 *
 * The input to the function assumes that the structure has already been
 * created.  The contents of the structure are filled in.
 *
 * There is no output.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void fill_task_record(Task *new)
{
    static int task_id = 0;
    char line[MAXLINE];
    assert(new != NULL);

    new->task_id = task_id;
    task_id++;
    
    printf("Priority:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%u", &new->priority);

    //printf("State:");
    //fgets(line, MAXLINE, stdin);
    //sscanf(line, "%u", &new->state);
    new->state = QUEUED;
    
    printf("Wallclocktime:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%le", &new->wallclocktime);
    

    printf("Number of Args:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%u", &new->nargs);

    assert(new->nargs >= 0 && "# args negative\n");
    new->args = (int*) malloc(sizeof(int) * new->nargs);
    int i=0;
    for (i=0; i<new->nargs; i++) {
        printf("Arg %d:", i);
        fgets(line, MAXLINE, stdin);
        sscanf(line, "%d", &new->args[i]);
    }

    printf("\n");
}

/* Gets values (from user) to use as arguments to determine if a task is runnable
 *
 * Input/Output argument is the number of args the user provides
 * Returns pointer to array of args
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
int* getDetermineArgs(int* num_args)
{
    char line[MAXLINE];
    int i;

    printf("Number of Args:");
    fgets(line, MAXLINE, stdin);
    sscanf(line, "%u", num_args);

    assert(*num_args >= 0 && "# args negative\n");
    int* args = (int*) malloc(sizeof(int) * (*num_args));
    for (i=0; i<*num_args; i++) {
        printf("Arg %d:", i);
        fgets(line, MAXLINE, stdin);
        sscanf(line, "%d", &args[i]);
    }

    return args;
}

/* Converts state enum value to a printable string
 *
 * Input state enum value
 *
 * DO NOT CHANGE THIS FUNCTION
 */
char* state2str(enum state state)
{
    if (state == QUEUED)
        return "QUEUED";
    else if (state == RUNNING)
        return "RUNNING";
    else if (state == BLOCKED)
        return "BLOCKED";
    else if (state == FINISHED)
        return "FINISHED";
    else
        return "UNKNOWN";
}




/* print the information for a particular task record in short form
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void print_task_rec_short(Task *rec)
{
    assert(rec != NULL);
    printf("Task ID: %d priority = %d state = %s\n",
            rec->task_id, rec->priority, state2str(rec->state));
}

/* Long form to print a particular task record 
 *
 * Input is a pointer to a record, and no entries are changed.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void print_task_rec_long(Task *rec)
{
    assert(rec != NULL);
    int i;
    
    printf("Task ID: %d\n", rec->task_id);
    printf("     priority = %d\n", rec->priority);
    printf("     state     = %s\n", state2str(rec->state));
    printf("     time = %e\n", rec->wallclocktime);
    printf("     nargs = %d\n", rec->nargs);
    printf("     args = {");
    for (i=0; i<rec->nargs; i++)
        printf(" %d, ", rec->args[i]);
    printf("}\n");
}

/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */