/* list.c
 * Aarav Rekhi
 * aaravr
 * ECE 2230 Spring 2024
 * MP2
 *
 * Propose: A template for list.c. You will make many changes.
 *
 * Assumptions: Many details are incomplete.  
 *
 * Bugs: Many details have not been implemented.
 *
 * The interface definition for the two-way linked list ADT is based on one 
 * defined by OPNET, Inc. http://www.opnet.com/
 */

#include <stdlib.h>
#include <assert.h>

#include "datatypes.h"   /* defines Data */
#include "list.h"        /* defines public functions for list ADT */

/* definitions for private constants used in list.c only */

#define LIST_SORTED_ASCENDING   -1234567
#define LIST_SORTED_DESCENDING  -7654321
#define LIST_UNSORTED           -4444444

/* prototypes for private functions used in list.c only */
void list_debug_validate(List *L);


/* ----- below are the functions  ----- */

/* Obtains a pointer to an element stored in the specified list, at the
 * specified list position
 * 
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from list_construct.
 *
 * pos_index: position of the element to be accessed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LISTPOS_HEAD and LISTPOS_TAIL
 *
 * return value: pointer to element accessed within the specified list.  A
 * value NULL is returned if the pos_index does not correspond to an element in
 * the list.  For example, if the list is empty, NULL is returned.
 */
Data * list_access(List *list_ptr, int pos_index)
{
    ListNode *L = NULL;
    assert(list_ptr != NULL);
    list_debug_validate(list_ptr);

    if (list_ptr->head == NULL)
        return NULL;
    else if (pos_index == LISTPOS_HEAD)
        return list_ptr->head->data_ptr;
    else if (pos_index == LISTPOS_TAIL)
        return list_ptr->tail->data_ptr;
    else if (pos_index < 0 || pos_index >= list_ptr->current_list_size)
        return NULL;

    L = list_ptr->head;
    int i;
    for (i = 0; i < pos_index; i++)
        L = L->next;
    assert(L != NULL);
    return L->data_ptr;
}

/* Allocates a new, empty list 
 *
 * By convention, the list is initially assumed to be sorted.  The field sorted
 * can only take values LIST_SORTED_ASCENDING LIST_SORTED_DESCENDING or 
 * LIST_UNSORTED
 *
 * Use list_free to remove and deallocate all elements on a list (retaining the
 * list itself).
 *
 * comp_proc = pointer to comparison function
 *
 * Use the standard function free() to deallocate a list which is no longer
 * useful (after freeing its elements).
 */
List * list_construct(int (*fcomp)(const Data *, const Data *),
        void (*dataclean)(Data *))
{
    List *L;
    L = (List *)malloc(sizeof(List));
    L->head = NULL;
    L->tail = NULL;
    L->current_list_size = 0;
    L->list_sorted_state = LIST_SORTED_ASCENDING;
    L->comp_proc = fcomp;
    L->data_clean = dataclean;
    list_debug_validate(L);
    return L;
}

/* Finds an element in a list and returns a pointer to it.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: element against which other elements in the list are compared.
 *           Note: use the comp_proc function pointer found in the List 
 *           header block. 
 *
 * The function returns a pointer to the matching element with lowest index if
 * a match if found.  If a match is not found the return value is NULL.
 *
 * The function also returns the integer position of matching element with the
 *           lowest index.  If a matching element is not found, the position
 *           index that is returned should be -1. 
 *
 * pos_index: used as a return value for the position index of matching element
 *
 *
 */
Data * list_elem_find(List *list_ptr, Data *elem_ptr, int *pos_index)
{
    list_debug_validate(list_ptr);
    Data *X = NULL;
    if (list_ptr == NULL || list_ptr->head == NULL){
        *pos_index = -1;
        return NULL;
    }

    *pos_index = 0;
    ListNode *N = list_ptr->head;
    while (N != NULL && list_ptr->comp_proc(N->data_ptr, elem_ptr) != 0){
        N = N->next;
        (*pos_index)++;
    }

    if (N == NULL){
        *pos_index = -1;
        return NULL;
    }

    X = N->data_ptr;
    return X;
}

/* Deallocates the contents of the specified list, releasing associated memory
 * resources for other purposes.
 *
 * Free all elements in the list, and the header block.  Use the data_clean
 * function point to free the Data items in the list.
 */
void list_destruct(List *list_ptr)
{
    list_debug_validate(list_ptr);
    assert(list_ptr != NULL);
    ListNode *current_node = list_ptr->head;
    ListNode *next_node;

    while (current_node != NULL){
        next_node = current_node->next;
        list_ptr->data_clean(current_node->data_ptr);
        free(current_node);
        current_node = next_node;
    }

    free(list_ptr);
}

/* Inserts the specified data element into the specified list at the specified
 * position.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * pos_index: numeric position index of the element to be inserted into the 
 *            list.  Index starts at 0 at head of the list, and incremented by 
 *            one until the tail is reached.  The index can also be equal
 *            to LISTPOS_HEAD or LISTPOS_TAIL (these are special negative 
 *            values use to provide a short cut for adding to the head
 *            or tail of the list).
 *
 * If pos_index is greater than the number of elements currently in the list, 
 * the element is simply appended to the end of the list (no additional elements
 * are inserted).
 *
 * Note that use of this function results in the list to be marked as unsorted,
 * even if the element has been inserted in the correct position.  That is, on
 * completion of this subroutine the list_ptr->list_sorted_state must be equal 
 * to LIST_UNSORTED.
 */
void list_insert(List *list_ptr, Data *elem_ptr, int pos_index)
{
    assert(list_ptr != NULL);
    ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
    new_node->data_ptr = elem_ptr;

    if (pos_index < 0 && pos_index != LISTPOS_HEAD && pos_index != LISTPOS_TAIL)
        return;
    
    if (list_ptr->head == NULL && list_ptr->tail == NULL){
        new_node->prev = NULL;
        new_node->next = NULL;
        list_ptr->head = new_node;
        list_ptr->tail = new_node;
    }
    else if (pos_index == 0 || pos_index == LISTPOS_HEAD){
        new_node->prev = NULL;
        new_node->next = list_ptr->head;
        if (list_ptr->head != NULL)
            list_ptr->head->prev = new_node;
        list_ptr->head = new_node;
    }

    else if (pos_index >= list_ptr->current_list_size || pos_index == LISTPOS_TAIL){
        new_node->prev = list_ptr->tail;
        new_node->next = NULL;
        if (list_ptr->tail != NULL)
            list_ptr->tail->next = new_node;
        list_ptr->tail = new_node;
    } else{
        ListNode *current = list_ptr->head;
        int count = 0;
        while (current != NULL && count < pos_index){
            current = current->next;
            count++;
        }

        if (current == NULL){
            new_node->prev = list_ptr->tail;
            new_node->next = NULL;
            list_ptr->tail->next = new_node;
            list_ptr->tail = new_node;
        } else{
            new_node->prev = current->prev;
            new_node->next = current;
            if (current->prev != NULL)
                current->prev->next = new_node;
            else
                list_ptr->head = new_node;
            current->prev = new_node;
        }
    }

    list_ptr->current_list_size++;
    if (list_ptr->list_sorted_state != LIST_UNSORTED)
        list_ptr->list_sorted_state = LIST_UNSORTED;
    list_debug_validate(list_ptr);
}

/* Inserts the element into the specified sorted list at the proper position,
 * as defined by the comp_proc function pointer found in the header block.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * elem_ptr: pointer to the element to be inserted into list.
 *
 * If you use list_insert_sorted, the list preserves its sorted nature.
 *
 * If you use list_insert, the list will be considered to be unsorted, even
 * if the element has been inserted in the correct position.
 *
 * If the list is not sorted and you call list_insert_sorted, this subroutine
 * should generate a system error and the program should immediately stop.
 *
 * The comparison procedure must accept two arguments (A and B) which are both
 * pointers to elements of type Data.  The comparison procedure returns an
 * integer code which indicates the precedence relationship between the two
 * elements.  The integer code takes on the following values:
 *    1: A should be closer to the list head than B
 *   -1: B should be closer to the list head than A
 *    0: A and B are equal in rank
 * This definition results in the list being in ascending order.  To insert
 * in decending order, change the sign of the value that is returned.
 *
 * Note: if the element to be inserted is equal in rank to an element already
 * in the list, the newly inserted element will be placed after all the
 * elements of equal rank that are already in the list.
 */
void list_insert_sorted(List *list_ptr, Data *elem_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->list_sorted_state != LIST_UNSORTED);
    int location = 0;
    int current_state = list_ptr->list_sorted_state;
    ListNode *node = list_ptr->head;
    if (list_ptr->list_sorted_state == LIST_SORTED_ASCENDING){
        while (node != NULL && list_ptr->comp_proc(node->data_ptr, elem_ptr) != -1){
            location++;
            node = node->next;
        }
    }
    else if (list_ptr->list_sorted_state == LIST_SORTED_DESCENDING){
        while (node != NULL && list_ptr->comp_proc(node->data_ptr, elem_ptr) != 1){
            location++;
            node = node->next;
        }
    }

    list_insert(list_ptr, elem_ptr, location);
    list_ptr->list_sorted_state = current_state;
    list_debug_validate(list_ptr);
}

/* Removes an element from the specified list, at the specified list position,
 * and returns a pointer to the element.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * pos_index: position of the element to be removed.  Index starts at 0 at
 *            head of the list, and incremented by one until the tail is
 *            reached.  Can also specify LISTPOS_HEAD and LISTPOS_TAIL
 *
 * Attempting to remove an element at a position index that is not contained in
 * the list will result in no element being removed, and a NULL pointer will be
 * returned.
 */
Data * list_remove(List *list_ptr, int pos_index)
{
    list_debug_validate(list_ptr);
    ListNode *delete;
    Data *data;
    if (list_ptr == NULL || (list_ptr->head == NULL && list_ptr->tail == NULL) || (pos_index < 0 && pos_index != LISTPOS_HEAD && pos_index != LISTPOS_TAIL) || pos_index >= list_ptr->current_list_size)
        return NULL;
    else if ((pos_index == 0 || pos_index == LISTPOS_HEAD || pos_index == LISTPOS_TAIL) && list_ptr->current_list_size == 1){
        delete = list_ptr->head;
        data = delete->data_ptr;
        list_ptr->head = NULL;
        list_ptr->tail = NULL;
    }
    else if (pos_index == 0 || pos_index == LISTPOS_HEAD){
        delete = list_ptr->head;
        data = delete->data_ptr;
        list_ptr->head = delete->next;
        if (list_ptr->head != NULL)
            list_ptr->head->prev = NULL;
    }
    else if (pos_index == (list_ptr->current_list_size) - 1 || pos_index == LISTPOS_TAIL){
        delete = list_ptr->tail;
        data = delete->data_ptr;
        list_ptr->tail = delete->prev;
        if (list_ptr->tail != NULL)
            list_ptr->tail->next = NULL;
    } else{
        delete = list_ptr->head;
        // Get to the index given
        for (int i = 0; i < pos_index; i++)
        {
            delete = delete->next;
        }
        data = delete->data_ptr;
        if (delete->prev != NULL)
        {
            delete->prev->next = delete->next;
        }
        if (delete->next != NULL)
        {
            delete->next->prev = delete->prev;
        }
    }
    (list_ptr->current_list_size)--;
    free(delete);
    return data;
}

/* Reverse the order of the elements in the list.  Also change the 
 * list_sorted_state flag.  This function can only be called on a list
 * that is sorted.
 *
 * list_ptr: pointer to list-of-interest.  
 */
void list_reverse(List *list_ptr)
{
    assert(list_order(list_ptr) != 0);
    if (list_ptr == NULL || list_ptr->head == NULL || list_ptr->head == list_ptr->tail)
        return;
	
    ListNode *node = list_ptr->head;
    ListNode *temp = NULL;
    list_ptr->tail = node;
    while (node != NULL){
        temp = node->prev;
        node->prev = node->next;
        node->next = temp;
        node = node->prev;
    }
    list_ptr->head = temp->prev;

    if (list_ptr->list_sorted_state == LIST_SORTED_ASCENDING)
        list_ptr->list_sorted_state = LIST_SORTED_DESCENDING;
    else if (list_ptr->list_sorted_state == LIST_SORTED_DESCENDING)
        list_ptr->list_sorted_state = LIST_SORTED_ASCENDING;
    list_debug_validate(list_ptr);
}

/* Obtains the length of the specified list, that is, the number of elements
 * that the list contains.
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * Returns an integer equal to the number of elements stored in the list.  An
 * empty list has a size of zero.
 */
int list_size(List *list_ptr)
{
    assert(list_ptr != NULL);
    assert(list_ptr->current_list_size >= 0);
    return list_ptr->current_list_size;
}

/* Obtains the sort status and order of the specified list. 
 *
 * list_ptr: pointer to list-of-interest.  
 *
 * Returns 
 *    1: the list is sorted in ascending order
 *   -1: descending order
 *    0: the list is not sorted but a queue
 */
int list_order(List *list_ptr)
{
    assert(list_ptr != NULL);
    if (list_ptr->list_sorted_state == LIST_SORTED_ASCENDING)
        return 1;
    else if (list_ptr->list_sorted_state == LIST_SORTED_DESCENDING)
        return -1;
    else if (list_ptr->list_sorted_state == LIST_UNSORTED)
        return 0;
    else
        exit(5);
}


/* This function verifies that the pointers for the two-way linked list are
 * valid, and that the list size matches the number of items in the list.
 *
 * If the linked list is sorted it also checks that the elements in the list
 * appear in the proper order.
 *
 * The function produces no output if the two-way linked list is correct.  It
 * causes the program to terminate and print a line beginning with "Assertion
 * failed:" if an error is detected.
 *
 * The checks are not exhaustive, so an error may still exist in the
 * list even if these checks pass.
 *
 * YOU MUST NOT CHANGE THIS FUNCTION.  WE USE IT DURING GRADING TO VERIFY THAT
 * YOUR LIST IS CONSISTENT.
 */
void list_debug_validate(List *L)
{
    ListNode *N;
    int count = 0;
    assert(L != NULL);
    if (L->head == NULL)
        assert(L->tail == NULL && L->current_list_size == 0);
    if (L->tail == NULL)
        assert(L->head == NULL && L->current_list_size == 0);
    if (L->current_list_size == 0)
        assert(L->head == NULL && L->tail == NULL);
    assert(L->list_sorted_state == LIST_SORTED_ASCENDING || L->list_sorted_state == LIST_SORTED_DESCENDING || L->list_sorted_state == LIST_UNSORTED);

    if (L->current_list_size == 1)
    {
        assert(L->head == L->tail && L->head != NULL);
        assert(L->head->next == NULL && L->head->prev == NULL);
        assert(L->head->data_ptr != NULL);
    }
    if (L->head == L->tail && L->head != NULL)
        assert(L->current_list_size == 1);
    if (L->current_list_size > 1)
    {
        assert(L->head != L->tail && L->head != NULL && L->tail != NULL);
        N = L->head;
        assert(N->prev == NULL);
        while (N != NULL)
        {
            assert(N->data_ptr != NULL);
            if (N->next != NULL)
                assert(N->next->prev == N);
            else
                assert(N == L->tail);
            count++;
            N = N->next;
        }
        // Find index for ascending sort
        assert(count == L->current_list_size);
    }
    if (L->list_sorted_state != LIST_UNSORTED && L->head != NULL)
    {
        N = L->head;
        int comp_val = -1 * list_order(L);
        while (N->next != NULL)
        {
            assert((L->comp_proc)(N->data_ptr, N->next->data_ptr) != comp_val);
            N = N->next;
        }
    }
}
/* commands for vim. ts: tabstop, sts: softtabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */