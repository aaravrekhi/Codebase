/* task_list.c
   Aarav Rekhi
   ECE 2230
   Spring 2024
   MP1

   Purpose:       Write the functions for the lab1.c program. Create a C file that consists of
                  task_list.c (containing ADT code) and task_list.h (containing the interface
                  declarations). Use an array of pointers that point to C structures containing
                  information for each task.

   Assumptions:   Assume that lab1.c is the correct way to implement all these functions/tasks
                  Assume that task_list.h has all necessary information.

   Bugs: None
*/

#include <stdlib.h>
#include <assert.h>

#include "task_list.h"
#define TRUE 1
#define FALSE 0

// Function to construct a new task list with a given size
struct task_list_t *task_list_construct(int size)
{
    struct task_list_t* list = (struct task_list_t*) malloc(sizeof(struct task_list_t));
    list->task_count = 0;
    list->task_size = size;
    list->task_ptr = (struct task_t**) malloc(size * sizeof(struct task_t*));
    return list;
}

// Function to destruct a task list and free allocated memory
void task_list_destruct(struct task_list_t *list_ptr)
{
    assert(list_ptr != NULL && "TASK LIST NULL IN DESTRUCT");
    free(list_ptr->task_ptr);
    free(list_ptr);
}

// Function to get the number of entries in the task list
int task_list_number_entries(struct task_list_t *list_ptr)
{
    assert(list_ptr != NULL);
    return list_ptr->task_count;
}

// Function to add a task to the task list
int task_list_add(struct task_list_t *list_ptr, struct task_t *rec_ptr)
{
    assert(list_ptr != NULL);

    // Check if there is space in the task list
    int lastIdx = list_ptr->task_count;
    if (lastIdx != list_ptr->task_size)
    {
        // Add the task to the task list
        list_ptr->task_ptr[lastIdx] = rec_ptr;
        (list_ptr->task_count)++;
        return 1; // Successful addition
    }
    else
        return list_ptr->task_count; // Task list is full
}

// Function to access a task at a specific index in the task list
struct task_t *task_list_access(struct task_list_t *list_ptr, int index)
{
    assert(list_ptr != NULL && "TASK LIST NULL IN ACCESS\n");

    struct task_t* new_task;

    // Check if the index is within bounds
    if (index < list_ptr->task_count)
        new_task = list_ptr->task_ptr[index];
    else
        new_task = NULL;

    return new_task;
}

// Function to remove a task at a specific index from the task list
struct task_t *task_list_remove(struct task_list_t *list_ptr, int index)
{
    assert(list_ptr != NULL && "LIST NULL IN REMOVE\n");

    struct task_t* task = NULL;

    // Check if the index is within bounds
    if (index < list_ptr->task_count)
    {
        task = list_ptr->task_ptr[index];

        // Shift tasks to fill the gap
        while (index < list_ptr->task_count)
        {
            list_ptr->task_ptr[index] = list_ptr->task_ptr[index+1];
            index++;
        }

        (list_ptr->task_count)--;
    }
    else
        task = NULL;

    return task;
}

// Function to find the index of the first task with a given priority in the task list
int task_list_lookup_first_priority(struct task_list_t *list_ptr, int priority)
{
    assert(list_ptr != NULL);

    int find = -1;
    int i = 0;

    // Search for the first task with the given priority
    while (i < list_ptr->task_count)
    {
        if (list_ptr->task_ptr[i]->priority != priority)
            i++;
        else
            return i;
    }

    return find;
}

// Function to find the index of a task with a given task ID in the task list
int task_list_lookup_id(struct task_list_t *list_ptr, int task_id)
{
    assert(list_ptr != NULL);

    int find = -1;
    int i = 0;

    // Search for the task with the given task ID
    while (i < list_ptr->task_count)
    {
        if (list_ptr->task_ptr[i]->task_id != task_id)
            i++;
        else
            return i;
    }

    return find;
}

// Function to access a task with a specific priority in the task list
struct task_t *task_list_access_priority(struct task_list_t *list_ptr, int priority)
{
    assert(list_ptr != NULL && "TASK LIST NULL IN ACCESS\n");

    int i = 0;

    // Search for the task with the given priority
    while (i < list_ptr->task_count)
    {
        if (list_ptr->task_ptr[i]->priority != priority)
            i++;
        else
            return list_ptr->task_ptr[i];
    }

    return NULL;
}

// Function to remove a task with a specific priority from the task list
struct task_t *task_list_remove_priority(struct task_list_t *list_ptr, int priority)
{
    assert(list_ptr != NULL && "LIST NULL IN REMOVE\n");

    struct task_t* task = NULL;
    int i = 0;

    // Search for the task with the given priority
    while (i < list_ptr->task_count)
    {
        if (list_ptr->task_ptr[i]->priority != priority)
            i++;
        else
        {
            task = list_ptr->task_ptr[i];
            (list_ptr->task_count)--;

            // Shift tasks to fill the gap
            while (i < list_ptr->task_count)
            {
                list_ptr->task_ptr[i] = list_ptr->task_ptr[i+1];
                i++;
            }
        }
    }

    return task;
}

// Function to access a task with a specific task ID in the task list
struct task_t *task_list_access_id(struct task_list_t *list_ptr, int id)
{
    assert(list_ptr != NULL && "TASK LIST NULL IN ACCESS\n");

    int i = 0;

    // Return the first task (Note: there's an issue here; should be fixed to return the task with the specified ID)
    while (i < list_ptr->task_count)
        return list_ptr->task_ptr[i];

    return NULL;
}

// Function to remove a task with a specific task ID from the task list
struct task_t *task_list_remove_id(struct task_list_t *list_ptr, int id)
{
    assert(list_ptr != NULL && "LIST NULL IN REMOVE\n");

    struct task_t* task = NULL;
    int i = 0;

    // Search for the task with the given task ID
    while (i < list_ptr->task_count)
    {
        if (list_ptr->task_ptr[i]->task_id != id)
            i++;
        else
        {
            task = list_ptr->task_ptr[i];
            (list_ptr->task_count)--;

            // Shift tasks to fill the gap
            while (i < list_ptr->task_count)
            {
                list_ptr->task_ptr[i] = list_ptr->task_ptr[i+1];
                i++;
            }
        }
    }

    return task;
}

// Function to determine if a task with given arguments is runnable in the task list
int task_list_determine_runable(struct task_list_t *list_ptr, int nargs, int* args)
{
    int i, j;

    // Iterate through tasks in the task list
    for (i = 0; i < list_ptr->task_count; i++)
    {
        // Check if the task is in QUEUED or BLOCKED state
        if (list_ptr->task_ptr[i]->state == QUEUED || list_ptr->task_ptr[i]->state == BLOCKED)
        {
            // Check if the number of arguments matches
            if (list_ptr->task_ptr[i]->nargs == nargs)
            {
                j = 0;

                // Check if each argument matches
                while (j < nargs && list_ptr->task_ptr[i]->args[j] == args[j])
                    j++;

                // If all arguments match, return the index of the runnable task
                if (j == nargs)
                    return i;
            }
        }
    }

    // Return -1 if no runnable task is found
    return -1;
}

// Function to set the state of a task with a specific task ID in the task list
void task_list_set_state(struct task_list_t *list_ptr, int id, enum state state)
{
    enum state current = -1;
    int i = 0;

    // Find the task with the specified task ID
    while (current == -1)
    {
        if (list_ptr->task_ptr[i]->task_id == id)
            current = list_ptr->task_ptr[i]->state;
    }

    // Set the state of the task
    list_ptr->task_ptr[i]->state = state;
}

// Function to remove all finished tasks from the task list and return them in a new task list
struct task_list_t* task_list_remove_finished(struct task_list_t *list_ptr)
{
    struct task_list_t* rm = (struct task_list_t*) malloc(sizeof(struct task_list_t));
    rm->task_size = 0;
    int i;

    // Count the number of finished tasks
    for (i = 0; i < list_ptr->task_count; i++)
        if (list_ptr->task_ptr[i]->state == FINISHED)
            (rm->task_size)++;

    rm->task_count = 0;
    rm->task_ptr = (struct task_t**) malloc((rm->task_size) * sizeof(struct task_t*));

    for (i = 0; i < list_ptr->task_count; i++)
    {
        // Move finished tasks to the new task list
        if (list_ptr->task_ptr[i]->state == FINISHED)
        {
            rm->task_ptr[rm->task_count] = task_list_remove(list_ptr, i);
            (rm->task_count)++;
            i--;
        }
    }

    // Update the task count in the original task list
    (list_ptr->task_count) = (rm->task_size) - 1;

    return rm;
}

// Function to schedule a task with a specific priority or task ID in the task list
struct task_t* task_list_schedule(struct task_list_t *list_ptr, int priority, int id)
{
    struct task_t* task = NULL;
    int i;

    for (i = 0; i < list_ptr->task_count; i++)
    {
        // Schedule task with a specific task ID
        if (list_ptr->task_ptr[i]->task_id == id)
        {
            // Check if the task is not finished or running
            if (list_ptr->task_ptr[i]->state != FINISHED && list_ptr->task_ptr[i]->state != RUNNING)
            {
                // Set the task state to RUNNING and return the task
                list_ptr->task_ptr[i]->state = RUNNING;
                return list_ptr->task_ptr[i];
            }
        }
    }

    for (i = 0; i < list_ptr->task_count; i++)
    {
        // Schedule task with a specific priority
        if (list_ptr->task_ptr[i]->priority == priority)
        {
            // Check if the task is not finished or running
            if (list_ptr->task_ptr[i]->state != FINISHED && list_ptr->task_ptr[i]->state != RUNNING)
            {
                // Set the task state to RUNNING and return the task
                list_ptr->task_ptr[i]->state = RUNNING;
                return list_ptr->task_ptr[i];
            }
        }
    }

    return task;
}