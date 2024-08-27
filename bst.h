/* bst.h 
 * Aarav Rekhi
 * aaravr@clemson.edu
 * ECE 2230 Spring 2024
 * MP 5
 *
 * Interface and tree definition for basic binary tree
 */

enum balanceoptions {BS_TREE, AVL};

#define TRUE 1
#define FALSE 0

typedef void *Data;
typedef int BST_Key;

typedef struct bst_node_tag {
    Data data_ptr;
    BST_Key key;
    int height;
    struct bst_node_tag *left;
    struct bst_node_tag *right;
} BST_Node;

typedef struct tree_stats {
    int CompCalls;
    int NumRotations;
} BST_Stats;

typedef struct bst_tag {
    BST_Node *root;
    int size;       // number of keys in tree
    int rotations;
    int policy;     // must be BS_TREE or AVL
    int last_key;
    BST_Stats stats; 
} BST;


/* prototype definitions for functions in bst.c */
Data bst_access(BST *bst_ptr, BST_Key key);
BST *bst_construct(int);
void bst_destruct(BST *bst_ptr);
int bst_insert(BST *bst_ptr, BST_Key key, Data elem_ptr);
int bst_avl_insert(BST *bst_ptr, BST_Key key, Data elem_ptr);
Data bst_remove(BST *bst_ptr, BST_Key key);

int bst_size(BST *bst_ptr);
int bst_key_comps(BST *bst_ptr);
int bst_int_path_len(BST *);
int bst_rotations(BST *bst_ptr);
void bst_debug_print_tree(BST *bst_ptr);
void bst_debug_validate(BST *T);


/* vi:set ts=8 sts=4 sw=4: */