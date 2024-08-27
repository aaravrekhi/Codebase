/* list.c                  <<-- A template to update and 
 * Aarav Rekhi           <<-- change
 * aaravr
 * ECE 2230 Spring 2024
 * MP3
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
void list_bubble_sort(List** L, int sort_order);
void list_insertion_sort(List** L, int sort_order);
void list_recursive_selection_sort(List** L, int sort_order);
void list_selection_sort(List** L, int sort_order);
void list_merge_sort(List** L, int sort_order);

/* sorting helper functions) */
void swap(ListNode *A, ListNode *B);
void SelectionSort(List** L, ListNode* start, ListNode* end, int sort_order);
ListNode* find_max(List* L, ListNode* start, ListNode* end);
ListNode* find_min(List* L, ListNode* start, ListNode* end);
void halve_list(List **L, List** left, List** right);
void merge_lists(List **L, List **left, List **right, int sort_order);

/* Sorts a list used a specific sorting algorithm and direction.
 * 
 * list_ptr: pointer to list-of-interest.  A pointer to an empty list is
 *           obtained from list_construct.
 *
 * sort_type: sorting algorithm used during sorting: 1) Bubble Sort; 
 *            2)Insertion Sort; 3) Recursive Selection Sort; 
 *            4) Iterative Selection Sort; and 5) Merge Sort
 *
 * sort_order: if 1 then the items are to be sorted in descending order.
 *             if 2 then the items are to be sorted in ascending order.
 *
 * return value: None.
 */
void list_sort(List **list_ptr, int sort_type, int sort_order)
{
	assert(list_ptr != NULL && *list_ptr != NULL); // Make sure pointer to list or list pointer itself are NULL

	if ((*list_ptr)->current_list_size > 1) // Sort list if there is more than one element
	{
		switch (sort_type) // Selection for the appropriate sort function
		{
			case 1:
				list_bubble_sort(list_ptr, sort_order);
				break;
			case 2:
				list_insertion_sort(list_ptr, sort_order);
				break;
			case 3:
				list_recursive_selection_sort(list_ptr, sort_order);
				break;
			case 4:
				list_selection_sort(list_ptr, sort_order);
				break;
			case 5:
				list_merge_sort(list_ptr, sort_order);
				break;
		}
	}

	if (sort_order == 1) // Set list to given order
		(*list_ptr)->list_sorted_state = LIST_SORTED_DESCENDING;
	else
		(*list_ptr)->list_sorted_state = LIST_SORTED_ASCENDING;
    list_debug_validate(*list_ptr);
}

/* Sorting the list via the insertion sort algorithm
 * 
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order 
 */
void list_bubble_sort(List** L, int sort_order)
{
	assert((*L)->current_list_size > 1); // Precondition
	int compare; // Compare elements based on given sort order
	if (sort_order == 1)
		compare = 1;
	else
		compare = -1;
	int length = (*L)->current_list_size;
	int i, j;
	ListNode *node = (*L)->head; // Start with head node
	
	for (i = 0; i < length-1; i++)
	{
		for (j = 0; j < length-i-1; j++)
		{
			if ((*L)->comp_proc(node->data_ptr,node->next->data_ptr) == compare) // Returns 1/-1 if elements are in ascending/descending order
				swap(node,node->next); // Swap node and node->next
			node = node->next; // Move onto the next node
		}
		node = (*L)->head; // Return to head node
	}
}

/* Sorting the list via the insertion sort algorithm
 * 
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order 
 */
void list_insertion_sort(List** L, int sort_order)
{
	assert((*L)->current_list_size > 1); // Precondition

	List *new_list = list_construct((*L)->comp_proc, (*L)->data_clean); // New list
	if (sort_order == 1) // Set order of new_list
		new_list->list_sorted_state = LIST_SORTED_DESCENDING;
	else
		new_list->list_sorted_state = LIST_SORTED_ASCENDING;
	int length = (*L)->current_list_size; // Pointer to hold the data
	int i;
	for (i = 0; i < length; i++)
		list_insert_sorted(new_list, list_remove(*L,0)); // Remove head of old list and insert into new sorted list
	list_destruct(*L); // Destruct the old list and point to new list
	*L = new_list;
}

/* Sorting the list via the recursive selection sort algorithm
 * 
 * L: pointer to list-of-interest.
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order 
 */
void list_recursive_selection_sort(List** L, int sort_order)
{
	assert((*L)->current_list_size > 1); // Precondition
	assert(L != NULL && (*L) != NULL); // Make sure list point is not NULL
	SelectionSort(L, (*L)->head, (*L)->tail, sort_order);
}

/* Sorting the list via the selection sort algorithm
 * 
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order 
 */
void list_selection_sort(List** L, int sort_order)
{
	assert((*L)->current_list_size > 1); // Precondition
	int compare; // Comparison direction setup
	if (sort_order == 1)
		compare = 1;
	else
		compare = -1;
	int i;
	int start = 0;
	int end = (*L)->current_list_size - 1;
	ListNode *start_node, *swap_node, *search_node;
	while (start < end)
	{
		start_node = (*L)->head; // Locate start_node for iteration
		for (i = 0; i < start; i++)
			start_node = start_node->next;

		assert(i == start); // Locate swap node
		i++;
		swap_node = start_node;
		search_node = swap_node->next;

		while (i <= end)
		{
			if ((*L)->comp_proc(swap_node->data_ptr, search_node->data_ptr) == compare)
				swap_node = search_node;
			search_node = search_node->next;
			i++;
		}

		swap(start_node, swap_node); // Swap swap_node and start_node
		start++;
	}
}

/* Sorting the list via the merge sort algorithm
 * 
 * L: pointer to list-of-interest.  
 *
 * sort_order: 1 sort list in descending order 2 sort in ascending order 
 */
void list_merge_sort(List** L, int sort_order)
{
	int length = (*L)->current_list_size;

	if (length > 1) // If there is more than one element in the list
	{
		List *left = list_construct((*L)->comp_proc, (*L)->data_clean); // List holding first half
		List *right = list_construct((*L)->comp_proc, (*L)->data_clean); // List holding second half
		halve_list(L, &left, &right); // Divide int list into two halves
		list_merge_sort(&left, sort_order); // Sort smaller list holding first half
		list_merge_sort(&right, sort_order); // Sort smaller list holding second half
		merge_lists(L, &left, &right, sort_order); // Merge lists back together
	}
}




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
    ListNode *L;

    assert(list_ptr != NULL);

    /* debugging function to verify that the structure of the list is valid */
    list_debug_validate(list_ptr);

    /* TODO: handle four special cases.
     *   1.  The list is empty
     *   2.  Asking for the head
     *   3.  Asking for the tail
     *   4.  Asking for invalid pos
     */

   	if (list_ptr->head == NULL) // Case 1
   	 	return NULL;
   	
   	else if (pos_index == 0 || pos_index == LISTPOS_HEAD) // Case 2
   		L = list_ptr->head;
   	
   	else if (pos_index == LISTPOS_TAIL || pos_index == (list_ptr->current_list_size)-1) // Case 3
   		L = list_ptr->tail;
   	
   	else if (pos_index < 0 || pos_index >= list_ptr->current_list_size) // Case 4
   		return NULL;

    /* we now know pos_index is for an interal element */
    /* TODO: loop through the list until find correct position index */
    else{
    	int i;
    	L = list_ptr->head;
    	for (i = 0; i < pos_index; i++)
    		L = L->next;
    }

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
    L = (List *) malloc(sizeof(List));
    L->head = NULL;
    L->tail = NULL;
    L->current_list_size = 0;
    L->list_sorted_state = LIST_SORTED_ASCENDING;
    L->comp_proc = fcomp;
    L->data_clean = dataclean;

    /* the last line of this function must call validate */
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
	Data* d = NULL;

    /*TODO: */
    /* fix the return value */

	if (list_ptr == NULL || list_ptr->head == NULL){ // If list is empty/uninitialized, reutnr NULL/-1
		*pos_index = -1;
		return NULL;
	}
	
	else{ // Locate matching element
		*pos_index = 0;
		ListNode* N = list_ptr->head;
		while (*pos_index<(list_ptr->current_list_size) && list_ptr->comp_proc(N->data_ptr, elem_ptr) == 0 ){
			N = N->next;
			(*pos_index)++;
		}
		
		if (*pos_index == list_ptr->current_list_size) // Set pos_index to go past list end
			*pos_index = -1;

		else // Set d if pos_index is already past list end
			d = N->data_ptr;
	}

    return d;
}

/* Deallocates the contents of the specified list, releasing associated memory
 * resources for other purposes.
 *
 * Free all elements in the list, and the header block.  Use the data_clean
 * function point to free the Data items in the list.
 */
void list_destruct(List *list_ptr)
{
    /* the first line must validate the list */
    list_debug_validate(list_ptr);
	assert(list_ptr != NULL);

	ListNode *A, *B; // Create loop variables, rover and tail

	if (list_ptr->head == NULL && list_ptr->tail == NULL) // Skip if list is empty
		assert(list_ptr->current_list_size == 0);
	
	else if (list_ptr->current_list_size == 1) // If list has just one element
		free(list_ptr->head);
	
	else{ // If there are multiple elements
		A = list_ptr->head; // Initialize rover
		B = A->next; // Initialize tail

		while (B != NULL){ // Free data from each element before freeing the element itself
			list_ptr->data_clean(A->data_ptr);
			free(A);
			A = B;
			B = B->next;
		}

		list_ptr->data_clean(A->data_ptr);
		free(A);
	}

	free(list_ptr); // Free final element and header block
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

	ListNode *new_node = (ListNode*)malloc(sizeof(ListNode)); // Create and allocate memory for new list element
	new_node->data_ptr = elem_ptr;
	// If pos_index is an invalid negative value, exit the function
	if (pos_index < 0 && pos_index != LISTPOS_HEAD && pos_index != LISTPOS_TAIL) // If pos_index is invalid, exit
		return;

	else if (list_ptr->head == NULL && list_ptr->tail == NULL){ // Insert into empty list
		new_node->prev = NULL;
		new_node->next = NULL;
		list_ptr->head = new_node;
		list_ptr->tail = new_node;
	}
	
	else if (pos_index == 0 || pos_index == LISTPOS_HEAD){ // Insert new element at list head
		new_node->prev = NULL;
		new_node->next = list_ptr->head;
		list_ptr->head->prev = new_node;
		list_ptr->head = new_node;
	}
	
	else if (pos_index >= list_ptr->current_list_size || pos_index == LISTPOS_TAIL){ // Insert new element at list tail
		new_node->prev = list_ptr->tail;
		new_node->next = NULL;
		list_ptr->tail->next = new_node;
		list_ptr->tail = new_node;
	}

	else{ // Insert new element at any list location
		ListNode *front = list_ptr->head;
		ListNode *back;
		int i;
		
		for (i = 0; i < pos_index; i++) // Find node currently as pos_index
			front = front->next;
		back = front->prev;
		new_node->prev = back;
		back->next = new_node;
		new_node->next = front;
		front->prev = new_node;
	}

	(list_ptr->current_list_size)++; // Increase list size by one

    /* the last two lines of this function must be the following */
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

    /* insert your code here */
    /*TODO*/
	int index;
	int current_state = list_ptr->list_sorted_state;

	if (list_ptr->head == NULL && list_ptr->tail == NULL) // If list is empty, add data
		index = 0;
	
	index = 0; // Locate index to add data
	ListNode *node = list_ptr->head;
	
	if (list_ptr->list_sorted_state == LIST_SORTED_ASCENDING){ // Locate index for ascending sort
		while (node != NULL && list_ptr->comp_proc(node->data_ptr,elem_ptr) != -1){
			index++;
			node = node->next;
		}
	}
	
	else if (list_ptr->list_sorted_state == LIST_SORTED_DESCENDING){ // Locate index for descending sort
		while (node != NULL && list_ptr->comp_proc(node->data_ptr,elem_ptr) != 1){
			index++;
			node = node->next;
		}
	}

	list_insert(list_ptr, elem_ptr, index); // Insert at located index
	list_ptr->list_sorted_state = current_state; // Reset list state
    /* the last line of this function must be the following */
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
	ListNode *lead, *follow, *removed;
	Data *data;

	if (list_ptr == NULL || (list_ptr->head == NULL && list_ptr->tail == NULL) 
		|| (pos_index < 0 && pos_index != LISTPOS_HEAD && pos_index != LISTPOS_TAIL)
		|| pos_index >= list_ptr->current_list_size) // If list does not exist, is empty, or given index does not exist, return NULL
		return NULL;
	
	else if ((pos_index == 0 ||pos_index == LISTPOS_HEAD || 
	pos_index == LISTPOS_TAIL) 
	&& list_ptr->current_list_size == 1){ // If there is only one element, and head/tail is chosen, remove it
		removed = list_ptr->head;
		data = removed->data_ptr;
		list_ptr->head = NULL;
		list_ptr->tail = NULL;
	}
	
	else if (pos_index == 0 || pos_index == LISTPOS_HEAD){ // If requested, remove list head
		follow = list_ptr->head;
		removed = follow;
		data = follow->data_ptr;
		lead = follow->next;
		lead->prev = NULL;
		list_ptr->head = lead;
	}
	// Remove the list tail if requested
	else if (pos_index == (list_ptr->current_list_size)-1 || pos_index == LISTPOS_TAIL){ // If requested, remove list taik
		lead = list_ptr->tail;
		removed = lead;
		data = lead->data_ptr;
		follow = lead->prev;
		follow->next = NULL;
		list_ptr->tail = follow;
	}

	else{ // If requested, remove any specifiec node
		removed = list_ptr->head;
		int i;
		for (i = 0; i < pos_index; i++)
			removed = removed->next;
		data = removed->data_ptr;
		follow = removed->prev;
		lead = removed->next;
		follow->next = lead;
		lead->prev = follow;
	}

	(list_ptr->current_list_size)--; // Decrease list size by one
	free(removed); // Free removed node memory
    assert(data != NULL); // Prove that data has been located
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
	// If the list does not exist, is empty, or only has 1 element, we don't
	// need to do anything
	if (list_ptr == NULL || (list_ptr->head == NULL && list_ptr->tail == NULL)
		|| list_ptr->head == list_ptr->tail){ // If list does not exist, is empty, or has just one element, do nothing
	}

	else if (list_ptr->head != list_ptr->tail){ // Elsewise, swap prev and next
		ListNode *node, *temp;
		node = list_ptr->head;
		list_ptr->tail = node; // Set first node as new tail

		while (node != NULL){ // Swapping prev and next
			temp = node->prev;
			node->prev = node->next;
			node->next = temp;
			node = node->prev;
		}

		list_ptr->head = temp->prev; // Set last node as new head
	}
	
	if (list_ptr->list_sorted_state == LIST_SORTED_ASCENDING){
		list_ptr->list_sorted_state = LIST_SORTED_DESCENDING; // Reverse list_sorted_state
	}
	else if (list_ptr->list_sorted_state == LIST_SORTED_DESCENDING){
		list_ptr->list_sorted_state = LIST_SORTED_ASCENDING;
	}

    list_debug_validate(list_ptr); // Verify list after reversal
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
        exit(5);  // this should not happen
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
#ifdef VALIDATE
    ListNode *N;
    int count = 0;
    assert(L != NULL);
    if (L->head == NULL)
        assert(L->tail == NULL && L->current_list_size == 0);
    if (L->tail == NULL)
        assert(L->head == NULL && L->current_list_size == 0);
    if (L->current_list_size == 0)
        assert(L->head == NULL && L->tail == NULL);
    assert(L->list_sorted_state == LIST_SORTED_ASCENDING
            || L->list_sorted_state == LIST_SORTED_DESCENDING
            || L->list_sorted_state == LIST_UNSORTED);

    if (L->current_list_size == 1) {
        assert(L->head == L->tail && L->head != NULL);
        assert(L->head->next == NULL && L->head->prev == NULL);
        assert(L->head->data_ptr != NULL);
    }
    if (L->head == L->tail && L->head != NULL)
        assert(L->current_list_size == 1);
    if (L->current_list_size > 1) {
        assert(L->head != L->tail && L->head != NULL && L->tail != NULL);
        N = L->head;
        assert(N->prev == NULL);
        while (N != NULL) {
            assert(N->data_ptr != NULL);
            if (N->next != NULL)
                assert(N->next->prev == N);
            else
                assert(N == L->tail);
            count++;
            N = N->next;
        }
        assert(count == L->current_list_size);
    }
    if (L->list_sorted_state != LIST_UNSORTED && L->head != NULL) {
        N = L->head;
        int comp_val = -1 * list_order(L);
        while (N->next != NULL) {
            assert((L->comp_proc)(N->data_ptr, N->next->data_ptr) != comp_val);
            N = N->next;
        }
    }
#endif
}

void swap(ListNode *A, ListNode *B)
{
	Data *temp = A->data_ptr;	// Swap the data pointers
	A->data_ptr = B->data_ptr;	// between the two nodes
	B->data_ptr = temp;

	temp = NULL; // Clear the dangling pointer
}

void SelectionSort(List** L, ListNode* start, ListNode* end, int sort_order)
{
	ListNode* swap_node;

	if (start != end){ // If there is no node in the list
		if (sort_order == 1) // Locate node for swapping
			swap_node = find_max(*L, start, end);
		else
			swap_node = find_min(*L, start, end);
	
		swap(start, swap_node); // Swap located node with header node
		SelectionSort(L, start->next, end, sort_order); // Function call for smaller list
	}
	swap_node = NULL;
}

ListNode* find_max(List* L, ListNode* start, ListNode* end)
{
	ListNode* max = start; // Set max equal to start of list
	ListNode* search_node = max->next; // Locate max node
	while (search_node != NULL){
		if (L->comp_proc(max->data_ptr, search_node->data_ptr) == 1) // If node data is greater than max data
			max = search_node; // Set max to the node
		search_node = search_node->next;
	}
	assert(search_node == NULL); // Now reached end of list
	return max;
}

ListNode* find_min(List* L, ListNode* start, ListNode* end)
{
	ListNode* min = start; // Set max equal to node at start of list
	ListNode* search_node = min->next; // Locate max node
	while (search_node != NULL){
		if (L->comp_proc(min->data_ptr, search_node->data_ptr) == -1) // If node data is less than minimum data
			min = search_node; // Set min to node
		search_node = search_node->next;
	}
	assert(search_node == NULL); // Now reached end of list
	return min;
}

void halve_list(List **L, List** left, List** right)
{
	int half_length = (*L)->current_list_size / 2; // Determine the length of half the list
	ListNode *midpoint = (*L)->head; // Locate midpoint
	int i;
	for (i = 1; i < half_length; i++)
		midpoint = midpoint->next;
	(*left)->head = (*L)->head; // Assign a left list
	(*left)->tail = midpoint;
	(*left)->current_list_size = half_length;
	(*right)->head = midpoint->next; // Assign a right list
	(*right)->tail = (*L)->tail;
	(*left)->tail->next = NULL; // Set ends of each list to NULL
	(*right)->head->prev = NULL;
	// Make right an element shorter if L had an odd number of elements
	if ((*L)->current_list_size % 2 == 0) // If left list has an odd amount of elements
		(*right)->current_list_size = half_length; // Make right list an element shorter
	else
		(*right)->current_list_size = half_length+1;
	free(*L); // Deallocate left list
	*L = NULL; // Remove dangling pointer
}

void merge_lists(List **L, List **left, List **right, int sort_order)
{
	int compare;
	if (sort_order == 1) // Determine how to compare elements given a sort order
		compare = -1;
	else
		compare = 1;
	*L = list_construct((*left)->comp_proc, (*left)->data_clean); // Allocate new merged list
	int num_elements =(*left)->current_list_size + (*right)->current_list_size;
	int i;
	Data* data;

	for (i = 0; i < num_elements; i++){
		if ((*left)->current_list_size == 0){ // If left list if empty
			data = list_remove(*right, 0); // Use elements from the right list
			list_insert(*L, data, LISTPOS_TAIL);
		}

		// If the right list is empty, take from the left
		else if ((*right)->current_list_size == 0){ // If right list is empty
			data = list_remove(*left, 0); // Use elements from the left list
			list_insert(*L, data, LISTPOS_TAIL);
		}
		else{ // If neither list is empty, compare list heads and choose
			if ((*L)->comp_proc((*left)->head->data_ptr,
				(*right)->head->data_ptr) == compare){
				data = list_remove(*left, 0);
				list_insert(*L, data, LISTPOS_TAIL);
			}
			else{
				data = list_remove(*right, 0);
				list_insert(*L, data, LISTPOS_TAIL);
			}
		}
	}
	
	list_destruct(*left); // Destroy smaller left list
	list_destruct(*right); // Destroy smaller right list
	*left = NULL; // Remove dangling pointer
	*right = NULL; // Remove dangling pointer
}