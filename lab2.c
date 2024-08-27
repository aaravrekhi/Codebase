/* lab2.c
 * Aarav Rekhi
 * aaravr                  
 * ECE 2230 Spring 2024
 * MP2
 *
 * 
 *
 * Propose: A driver for MP2. DO NOT MODIFY!
 *
 * Assumptions: This file is simple.  Input is collected and the
 * appropriate wifi function is called.
 *
 * An outline for the interactive menu input is provided.  Details need to be
 * completed but format of the commands and the output generated by the
 * print statements must not be changed. You have to call the corrent 
 * print statement in response to the input
 *
 * Bugs: Many details have not been implemented.
 *
 * See the ECE 2230 programming guide
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "datatypes.h"
#include "list.h"
#include "task_support.h"

int main(int argc, char * argv[])
{
    List * sorted_list;
    List * unsorted_list;
    char line[MAXLINE];
    char command[MAXLINE];
    char junk[MAXLINE];
    int list_size = -1;
    int num_items;
    int id, arg2;

    if (argc != 2) {
        printf("Usage: ./lab2 list_size\n");
        exit(1);
    }
    list_size = atoi(argv[1]);
    if (list_size < 1) {
        printf("Invalid list size %d\n", list_size);
        exit(2);
    } 
    printf("Lab2 list size is %d. Possible commands:\n", list_size);
    printf("List: INSERT\nFIND id\nREMOVE id\nUPDATE id state\nSCHEDULE id priority\nDETERMINE\nCLEAN\nREVERSE\nPRINT\n");

    printf("Queue      : ADDTAIL; RMHEAD; PRINTHEAD; PRINTQ\n");
    printf("           : STATS; QUIT\n\n\n");

    // Create two List objects
    /* this list is sorted */
    sorted_list = task_list_create();

    /* this list is unsorted and has no duplicates */
    unsorted_list = task_list_create();

    /* remember fgets includes newline \n unless line too long */
    while (fgets(line, MAXLINE, stdin) != NULL) {
        num_items = sscanf(line, "%s %d %d %s", command, &id, &arg2, junk);
        if (num_items == 1 && strcmp(command, "INSERT") == 0) {
            task_list_add(sorted_list, list_size);
        } else if (num_items == 2 && strcmp(command, "FIND") == 0) {
            task_list_lookup_id(sorted_list, id);
        } else if (num_items == 2 && strcmp(command, "REMOVE") == 0) {
            task_list_remove_id(sorted_list, id);
        } 
        // MP1 ops
        else if (num_items == 3 && strcmp(command, "UPDATE") == 0) {
            task_list_update(sorted_list, id, arg2); // arg2 is state 
        } else if (num_items == 3 && strcmp(command, "SCHEDULE") == 0) {
            task_list_schedule(sorted_list, id, arg2); // arg2 is priority 
        } else if (num_items == 1 && strcmp(command, "DETERMINE") == 0) {
            task_list_determine(sorted_list); 
        } else if (num_items == 1 && strcmp(command, "CLEAN") == 0) {
            task_list_remove_finished(sorted_list); 
        }

        //new in MP2
        else if (num_items == 1 && strcmp(command, "REVERSE") ==0) {
            task_list_reverse(sorted_list);
        } else if (num_items == 1 && strcmp(command, "PRINT") == 0) {
            task_list_print(sorted_list, "List");
        }
        // unsorted list commands
        else if (num_items == 1 && strcmp(command, "ADDTAIL") == 0) {
            task_list_add_tail(unsorted_list);
        } else if (num_items == 1 && strcmp(command, "RMHEAD") == 0) {
            task_list_remove_head(unsorted_list);
        } else if (num_items == 1 && strcmp(command, "PRINTHEAD") == 0) {
            task_list_print_head(unsorted_list);
        } else if (num_items == 1 && strcmp(command, "PRINTQ") == 0) {
            task_list_print(unsorted_list, "Queue");
        } 
        //general
        else if (num_items == 1 && strcmp(command, "STATS") == 0) {
            task_list_stats(sorted_list, unsorted_list);
        } else if (num_items == 1 && strcmp(command, "QUIT") == 0) {
            task_list_cleanup(sorted_list);
            task_list_cleanup(unsorted_list);
            printf("Goodbye\n");
            break;
        } else {
            printf("UNKNOWN COMMAND: %s", line);
        }
    }
    return 0;
}
/* commands specified to vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */