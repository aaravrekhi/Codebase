/* task_list.h 
 * Prof. Calhoun 
 * ECE 2230 Spring 2024
 * MP1
 *
 * Purpose: The interface definition for MP1.  DO NOT CHANGE THIS FILE
 *
 * Assumptions: structure definitions and public functions as defined for
 * assignmnet.  There are no extensions.
 *
 * Bugs:
 */

enum state {QUEUED, RUNNING, BLOCKED, FINISHED};

struct task_t {
    int task_id;            // unique task id
    int priority;           // scheduling priority
    enum state state;       // scheduling state 
    
    
    double  wallclocktime;  // task runtime in seconds
   
     
    int nargs;              // number of input arguments
    int* args;              // input arguments
    int output;             // task result
};


// Header block for the list
struct task_list_t {
    int task_count;            // current number of records in list
    int task_size;             // size of the list
    struct task_t **task_ptr;  // array of task pointers
};

//  The sequential list ADT must have the following interface:

struct  task_list_t*    task_list_construct(int size);
void                    task_list_destruct(struct task_list_t *);
int                     task_list_number_entries(struct task_list_t *);
int                     task_list_add(struct task_list_t *, struct task_t *);
struct  task_t *        task_list_access(struct task_list_t *, int idx);
struct  task_t *        task_list_remove(struct task_list_t *, int idx);


int                     task_list_lookup_first_priority(struct task_list_t *list_ptr, int priority);
int                     task_list_lookup_id(struct task_list_t *, int id);
struct task_t *         task_list_access_priority(struct task_list_t *list_ptr, int priority);
struct task_t *         task_list_remove_priority(struct task_list_t *list_ptr, int priority);
struct task_t *         task_list_access_id(struct task_list_t *list_ptr, int id);
struct task_t *         task_list_remove_id(struct task_list_t *list_ptr, int id);



//operations
void                    task_list_set_state(struct task_list_t *, int id, enum state state);
struct task_t*          task_list_schedule(struct task_list_t *, int priority, int id);
int                     task_list_determine_runable(struct task_list_t *, int nargs, int*args);
struct task_list_t*     task_list_remove_finished(struct task_list_t *);
