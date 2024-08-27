/* datatypes.h 
 * Aarav Rekhi
 * aaravr
 * ECE 2230 Spring 2024
 * MP3
 *
 * Propose: The data type that is stored in the list ADT is defined here.  We
 *          define a single mapping that allows the list ADT to be defined in
 *          terms of a generic Data.
 *
 * Assumptions: 
 *
 *      Data: The type of data that we want to store in the list
 *
 * Bugs:
 * 
 */

enum state {QUEUED, RUNNING, BLOCKED, FINISHED};

typedef struct task_info {
    int task_id;            // unique task id
    int priority;           // scheduling priority
    enum state state;       // scheduling state 
    
    
    double  wallclocktime;  // task runtime in seconds
   
     
    int nargs;              // number of input arguments
    int* args;              // input arguments
    int output;             // task result
} Task;

/* the list ADT works on task data of this type */
typedef Task Data;