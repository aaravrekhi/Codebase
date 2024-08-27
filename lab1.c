/* lab1.c
 * Prof. Jon Calhoun 
 * jonccal                              <--- replace with your user name!
 * ECE 2230 Spring 2024
 * MP1
 *
 *
 * Purpose: Driver for MP1 
 *
 * Assumptions: The functions to collect input
 * for a record and to print a record specify the format that is required for
 * grading.
 *
 * The program accepts one command line arguement that is the size of the list.
 *
 *
 * Bugs: None that I know.
 *
 * See the ECE 2230 programming guide
 *
 * NOTE: One of the requirements is to verify you program does not have any 
 * memory leaks or other errors that can be detected by valgrind.  Run with
 * your test script(s):
 *      valgrind --leak-check=full ./lab1 5 < your_test_script
 *  and replace "5" with the value that makes sense for your test script
 * 
 * Are you unhappy with the way this code is formatted?  You can easily
 * reformat (and automatically indent) your code using the astyle 
 * command.  If it is not installed use the Ubuntu Software Center to 
 * install astyle.  Then in a terminal on the command line do
 *     astyle --style=kr lab1.c
 *
 * See "man astyle" for different styles.  Replace "kr" with one of
 * ansi, java, gnu, linux, or google to see different options.  Or, set up 
 * your own style.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "task_list.h"

#define MAXLINE 256
#define FALSE 0
#define TRUE 1

// function prototypes for lab1.c
void fill_task_record(struct task_t *new, int task_id);
void print_task_rec_short(struct task_t *rec);
void print_task_rec_long(struct task_t *rec);
int* getDetermineArgs(int* num_args);
char* state2str(enum state state);

int main(int argc, char *argv[])
{
    char line[MAXLINE];
    char command[MAXLINE];
    char junk[MAXLINE];
    int list_size = -1;
    int num_items = 0;
    int task_id_counter = 0;
    int arg1 = 0;
    int arg2 = 0;
    int i = 0;
    struct task_t *rec_ptr = NULL;

    if (argc != 2) {
        printf("Usage: ./lab1 list_size\n");
        exit(1);
    }
    list_size = atoi(argv[1]);
    if (list_size < 1) {
        printf("Invalid list size %d\n", list_size);
        exit(2);
    } 
    printf("The list size is %d. Possible commands:\n", list_size);
    printf("INSERT\nFIND id\nREMOVE id\nUPDATE id state\nPRINT\nSTATS\nSCHEDULE id priority\nDETERMINE\nCLEAN\nQUIT\n");

    struct task_list_t *my_list = task_list_construct(list_size);

    // remember fgets includes newline \n unless line too long 
    while (fgets(line, MAXLINE, stdin) != NULL) {
        num_items = sscanf(line, "%s %d %d %s", command, &arg1, &arg2, junk);
        

        if (num_items == 1 && strcmp(command, "QUIT") == 0) {
            printf("Goodbye\n");

            /* remove list from back to front to advoid shifting */
            int num_in_list = task_list_number_entries(my_list);
            for (i = num_in_list-1; i>=0; i--) {
                rec_ptr = task_list_remove(my_list, i);
                free(rec_ptr->args);
                free(rec_ptr);
            }
            
            task_list_destruct(my_list);
            break;
        }


        
        else if (num_items == 1 && strcmp(command, "INSERT") == 0) {
            rec_ptr = NULL;   
            rec_ptr = (struct task_t *) malloc(sizeof(struct task_t));
            fill_task_record(rec_ptr, task_id_counter);
            task_id_counter++;

            int added_return = -2;
            added_return = task_list_add(my_list, rec_ptr);

            if (added_return == 1) {
                printf("Inserted: %d\n", rec_ptr->task_id);
            } 
            else if (added_return == -1) {
                printf("Rejected: %d\n", rec_ptr->task_id);
                free(rec_ptr->args);
                free(rec_ptr);
            } else {
                printf("Error with return value! Fix your code.\n");
            }
            rec_ptr = NULL;
        }

        
        else if (num_items == 2 && strcmp(command, "FIND") == 0) {
            int index = -1;
            rec_ptr = NULL;  

            // arg1 is task_id    
            index = task_list_lookup_id(my_list, arg1);
            rec_ptr = task_list_access(my_list, index);

            if (rec_ptr == NULL) {
                printf("Did not find: %d\n", arg1);
            } else {
                printf("Found: %d at index %d\n", arg1, index);
                print_task_rec_long(rec_ptr);
            }
            rec_ptr = NULL;
        }




        else if (num_items == 3 && strcmp(command, "UPDATE") == 0) {
            
            // arg1 is task_id; arg2 is state
            rec_ptr = task_list_access_id(my_list, arg1);
            if (rec_ptr != NULL){
                task_list_set_state(my_list, arg1, (enum state)arg2);
                printf("Task %d's state is now %s\n", arg1, state2str(rec_ptr->state));
            }
            rec_ptr = NULL;
        }


        else if (num_items == 2 && strcmp(command, "REMOVE") == 0) {
            int index = -1;
            rec_ptr = NULL;   

            // arg1 is task_id    
            index = task_list_lookup_id(my_list, arg1);
 
            if (index == -1) {
                printf("Did not remove: %d\n", arg1);
            } else {
                rec_ptr = task_list_remove(my_list, index);
                printf("Removed: %d\n", arg1);
                //print_task_rec_short(rec_ptr);
                print_task_rec_long(rec_ptr);
                
                free(rec_ptr->args);
                free(rec_ptr);
            }
            rec_ptr = NULL;
        }

        else if (num_items == 3 && strcmp(command, "SCHEDULE") == 0) {
        
            // arg1 is task_id; arg2 is priority
            rec_ptr = task_list_schedule(my_list, arg2, arg1);
            if (rec_ptr != NULL)
            {
                printf("Now running task:\n");
                print_task_rec_short(rec_ptr);
                
            }
            else {
                printf("Did not find task to schedule...\n");
            }
        }


        else if (num_items == 1 && strcmp(command, "DETERMINE") == 0) {
            int num_args;
            int* args = getDetermineArgs(&num_args);
            int idx = task_list_determine_runable(my_list, num_args, args);
            free(args);

            if (idx != -1) {
                rec_ptr = task_list_access(my_list, idx);
                printf("Task %d is runable.\n", rec_ptr->task_id);
            }
            else {
                printf("No runable tasks.\n");
            }

            
        }
        else if (num_items == 1 && strcmp(command, "CLEAN") == 0) {
            struct task_list_t* removed = NULL;
            removed = task_list_remove_finished(my_list);
            
            /* remove list from back to front to advoid shifting */
            int num_in_list = task_list_number_entries(removed);
            printf("Removing %d finshed tasks\n", num_in_list);
            
            for (i = num_in_list-1; i>=0; i--) {
                rec_ptr = task_list_remove(removed, i);
                print_task_rec_short(rec_ptr);
                free(rec_ptr->args);
                free(rec_ptr);
            }
            
            task_list_destruct(removed);
        }


        else if (num_items == 1 && strcmp(command, "STATS") == 0) {
            // get the number in list and order of the list
            int num_in_list = 0;   
            num_in_list = task_list_number_entries(my_list);
            printf("Number records: %d\n", num_in_list);
        }

        
        else if (num_items == 1 && strcmp(command, "PRINT") == 0) {
            int num_in_list = 0;   
            num_in_list = task_list_number_entries(my_list);
            
            if (num_in_list == 0) {
                printf("List empty\n");
            } else {
                printf("List has %d records\n", num_in_list);
                for (i = 0; i < num_in_list; i++) {
                    printf("%4d: ", i);
                    rec_ptr = NULL;    
                    rec_ptr = task_list_access(my_list, i);
                    print_task_rec_short(rec_ptr);
                }
            }
            rec_ptr = NULL;
        } else {
            printf("UNKNOWN COMMAND: %s", line);
        }
    }
    return 0;
}

/* If a string ends with an end-of-line \n, remove it.
 */
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
void fill_task_record(struct task_t *new, int task_id)
{
    char line[MAXLINE];
    assert(new != NULL);

    new->task_id = task_id;

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


/* print the information for a particular task record in short form
 *
 * rec: Input is a pointer to a record, and no entries are changed.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void print_task_rec_short(struct task_t *rec)
{
    assert(rec != NULL);
    printf("Task ID: %d priority = %d state = %s\n",
            rec->task_id, rec->priority, state2str(rec->state));
}

/* Long form to print a particular task record 
 *
 * rec: Input is a pointer to a record, and no entries are changed.
 *
 * DO NOT CHANGE THIS FUNCTION!
 */
void print_task_rec_long(struct task_t *rec)
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

