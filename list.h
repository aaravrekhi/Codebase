/* list.h 
 *
 * Public functions for two-way linked list
 *
 * You should not need to change any of the code this file.  If you do, you
 * must get permission from the instructor.
 */

/* public constants used as parameters by most functions */

#define LISTPOS_HEAD -1001
#define LISTPOS_TAIL -0110

typedef struct list_node_tag {
    // Private members for list.c only
    Data *data_ptr;
    struct list_node_tag *prev;
    struct list_node_tag *next;
} ListNode;

typedef struct list_tag {
    // Private members for list.c only
    ListNode *head;
    ListNode *tail;
    int current_list_size;
    int list_sorted_state;

    // Private method for list.c only
    int (*comp_proc)(const Data *, const Data *);
    void (*data_clean)(Data *);
} List;
/* public prototypes defintions for MP3 */
void list_sort(List **list_ptr, int sort_type, int sort_order);

/* public prototype definitions for list.c */
Data * list_access(List *list_ptr, int pos_index);
List * list_construct(int (*fcomp)(const Data *, const Data *),
                        void (*dataclean)(Data *));
Data * list_elem_find(List *list_ptr, Data *elem_ptr, int *pos_index);
void     list_destruct(List *list_ptr);
void     list_insert(List *list_ptr, Data *elem_ptr, int pos_index);
void     list_insert_sorted(List *list_ptr, Data *elem_ptr);
Data * list_remove(List *list_ptr, int pos_index);
void     list_reverse(List *list_ptr);
int      list_size(List *list_ptr);
int      list_order(List *list_ptr);

void swap(ListNode *A, ListNode *B);
void SelectionSort(List** L, ListNode* start, ListNode* end, int sort_order);
ListNode* find_max(List* L, ListNode* start, ListNode* end);
ListNode* find_min(List* L, ListNode* start, ListNode* end);
void halve_list(List **L, List** left, List** right);
void merge_lists(List **L, List **left, List **right, int sort_order);

/* commands for vim. ts: tabstop, sts: soft tabstop sw: shiftwidth */
/* vi:set ts=8 sts=4 sw=4 et: */