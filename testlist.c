/*

   Writing and debugging MP2 is challenging, and it is particularly hard if you
   try and write all the code before you do any testing. It is much easier to
   break the problem into small pieces and test each piece.

For MP2, one approach is to write list_insert first, since it might be the
easiest algorithm and it depends only on list_construct.

   For testing, we use a simple main function in this file.  This file is for
   testing only, and you will NOT submit it as it is for your personal testing
   during development.

Below are a few simple examples that allocates a few structures and inserts
them into a list.

With this file containing a temporary main section, you can compile and run
list.c on its own using
   gcc -Wall -g testlist.c list.c task_list_support.c -o testlist

The compling command is also found in the provided makefile.  Do
   make testlist

Do this testing before you make ANY changes to task_list_support.c.  The only
reason we need to compile task_list_support.c is for the task_list_compare function.
The linker needs to find the comp_proc function and it is found in the template
   task_list_support.c file (with no changes)

*/

#include <stdio.h>
#include <stdlib.h>
#include "datatypes.h"
#include "list.h"
#include "task_support.h"

void list_debug_print(List *list_ptr);

int main(void)
{
    List *list_ptr = list_construct(task_compare, task_rec_cleanup);
    printf("Test Case 1: List Construction\n");
    if (list_ptr != NULL && list_ptr->current_list_size == 0)
        printf("PASS: List constructed successfully with size 0.\n");
    else
        printf("FAIL: List construction failed.\n");

    printf("\nTest Case 2: Insert Tasks into the List\n");

    for (int i = 0; i < 10; i++){
        Task *new_task = (Task *)malloc(sizeof(Task));
        new_task->task_id = i;
        new_task->priority = i % 5;
        list_insert(list_ptr, new_task, LISTPOS_TAIL);
        printf("Inserted task with ID %d and priority %d.\n", new_task->task_id, new_task->priority);
    }
    
    list_debug_print(list_ptr);
    printf("\nTest Case 3: Access Tasks by Position\n");

    for (int i = 0; i < list_size(list_ptr); i++){
        Task *task = list_access(list_ptr, i);
        printf("Accessed task at position %d has ID %d and priority %d.\n", i, task->task_id, task->priority);
    }

    printf("\nTest Case 4: Find Tasks by ID\n");

    for (int i = 0; i < 10; i++){
        int index = -1;
        Task template = {.task_id = i};
        Task *found = list_elem_find(list_ptr, &template, &index);
        if (found != NULL)
            printf("Found task ID %d at index %d.\n", found->task_id, index);
        else
            printf("Did not find task ID %d.\n", i);
    }

    printf("\nTest Case 5: List Cleanup\n");
    list_destruct(list_ptr);
    printf("List cleaned up.\n");
    return 0;
}

void list_debug_print(List *list_ptr)
{
    if (list_ptr == NULL){
        printf("List is NULL\n");
        return;
    }

    ListNode *L = list_ptr->head;
    printf("List size is %d\n", list_ptr->current_list_size);
    int count = 0;

    while (L != NULL){
        printf("Pos: %d contains task id: %d, priority: %d\n", count, L->data_ptr->task_id, L->data_ptr->priority);
        L = L->next;
        count++;
    }
    if (count != list_ptr->current_list_size)
        printf("Error: The counted list size is %d, but it should be %d\n", count, list_ptr->current_list_size);
}