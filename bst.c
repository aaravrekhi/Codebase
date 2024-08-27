/* bst.c
 * Aarav Rekhi
 * aaravr@clemson.edu
 * ECE 2230 Spring 2024
 * MP 5
 *
 * Implementation of tree interface for basic binary tree
 */
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>

#include "bst.h"

#define MYMAX(a, b) (a > b ? a : b)

// counters for statistics
static BST_Stats stats;

/* definitions for use in bst.c only */
void ugly_print(BST_Node *N, int level, int policy);
int bst_debug_validate_rec(BST_Node *N, int min, int max, int *count);
int rec_height(BST_Node *N);
int children(BST_Node *N);
void pretty_print(BST *T);


// Helper functions
void bst_destruct_nodes(BST_Node *N);
int bst_path_len(BST_Node *N, int depth);
BST_Node *bst_insert_node(BST_Node *N, BST_Key key, Data elem_ptr, int policy);
int find_balance(BST_Node *No);
int find_height(BST_Node *No);
BST_Node *left_rotation(BST_Node *N);
BST_Node *right_rotation(BST_Node *N);
int max(int a, int b);

/* Finds the tree element with the matching key and returns the data that is
 * stored in this node in the tree
 *
 * T - tree to search in key - key to find inside T
 *
 * RETURNS pointer to the data stored in the found node or NULL if no match is
 * found
 */
Data bst_access(BST *T, BST_Key key)
{
    stats.CompCalls = 0;
    BST_Node *N = T->root;
    while (N != NULL){
        stats.CompCalls++;
        if (key == N->key)
            return N->data_ptr;
        stats.CompCalls++;
        if (key < N->key)
            N = N->left;
        else
            N = N->right;
    }

    T->last_key = stats.CompCalls;
    return NULL;
}

/* Creates the header block for the tree with the provided management policy,
 * and initializes it with default 'blank' data.
 *
 * tree_policy - tree management policy to use either AVL or BST.
 *
 * RETURNS pointer to the newly created tree
 */
BST *bst_construct(int tree_policy)
{
    assert(tree_policy == AVL || tree_policy == BS_TREE);
    BST *binarysearchtree = (BST *)malloc(sizeof(BST));
    binarysearchtree->root = NULL;
    binarysearchtree->policy = tree_policy;
    binarysearchtree->size = 0;
    binarysearchtree->rotations = 0;
    binarysearchtree->last_key = 0;
    return binarysearchtree;
}

/* Free all items stored in the tree including the memory block with the data
 * and the BST_Node structure.  Also frees the header block.
 *
 * T - tree to destroy
 */
void bst_destruct(BST *T)
{
    bst_destruct_nodes(T->root);
    free(T);
}

/* Insert Data into the tree with the associated key. Insertion MUST
 * follow the tree's property (i.e., AVL or BST)
 *
 * T - tree to insert into
 * key - search key to determine if key is in the tree
 * elem_ptr - data to be stored at tree node indicated by key
 *
 * RETURNS 0 if key is found and element is replaced, and 1 if key is not found
 * and element is inserted
 */
int bst_insert(BST *T, BST_Key key, Data elem_ptr)
{
    stats.CompCalls = 0;
    stats.NumRotations = 0;
    int entry = -1;
    BST_Node *N = T->root;
    while (N != NULL && entry == -1){
        stats.CompCalls++;
        if (key == N->key){
            free(N->data_ptr);
            N->data_ptr = elem_ptr;
            entry = 0;
        } else{
            stats.CompCalls++;
            N = (key < N->key) ? N->left : N->right;
        }
    }

    if (entry == -1){
        if (T->policy == AVL)
            entry = bst_avl_insert(T, key, elem_ptr);
        else{
            entry = 1;
            T->root = bst_insert_node(T->root, key, elem_ptr, T->policy);
        }
        
        (T->size)++;
    }

    N = NULL;
    T->last_key = stats.CompCalls;
    T->rotations = stats.NumRotations;
    return entry;
}

/* Insert Data into the tree with the associated key. Insertion MUST
 * follow the tree's property AVL. This function should be called from
 * bst_insert for AVL tree's inserts.
 *
 * T - tree to insert into
 * key - search key to determine if key is in the tree
 * elem_ptr - data to be stored at tree node indicated by key
 *
 * RETURNS 0 if key is found and element is replaced, and 1 if key is not found
 * and element is inserted
 */
int bst_avl_insert(BST *T, BST_Key key, Data elem_ptr)
{
    T->root = bst_insert_node(T->root, key, elem_ptr, T->policy);
    return 1;
}

/* DO NOT NEED TO IMPLEMENT FOR REGULAR ASSIGNMENT. ONLY FOR EXTRA ASSIGNMENT.
 *
 * Removes the item in the tree with the matching key.
 *
 * T - pointer to tree
 * key - search key for particular node in the tree 'T'
 *
 * RETURNS the pointer to the data memory block and free the BST_Node memory
 * block.  If the key is not found in the tree, return NULL.  If the tree's
 * policy is AVL, then ensure all nodes have the AVL property.
 *
 */

/*
This function is used to remove a node from the tree
T - the tree to remove the node from
*/
Data bst_remove(BST *T, BST_Key key)
{
    Data dat = NULL;
    stats.CompCalls = 0;
    stats.NumRotations = 0;
    BST_Node *N = T->root;
    BST_Node *parent = NULL;
    while (N != NULL && dat == NULL){
		stats.CompCalls++;
		if (key == N->key)
			dat = N->data_ptr;
		else{
			parent = N;
			stats.CompCalls++;
			if (key < N->key)
				N = N->left;
			else
				N = N->right;
		}
	}

    return NULL;
}

/* RETURNS the number of keys in the tree */
int bst_size(BST *T)
{
    return T->size;
}

/* RETURNS the number of key comparisons  */
int bst_key_comps(BST *T)
{
    return T->last_key;
}

/* RETURNS the computed internal path length of the tree T */
int bst_int_path_len(BST *T)
{
    return bst_path_len(T->root, 0);
}

/* RETURNS the number of rotations from the last remove*/
int bst_rotations(BST *T)
{
    return T->rotations;
}

/* prints the tree T */
void bst_debug_print_tree(BST *T)
{
    ugly_print(T->root, 0, T->policy);
    printf("\n");
    if (T->size < 64)
        pretty_print(T);
}

/* basic print function for a binary tree
 *
 * N - node of tree to print
 * level - level in which the node resides
 * policy - BST or AVL
 */
void ugly_print(BST_Node *N, int level, int policy)
{
    int i;
    if (N == NULL)
        return;
    ugly_print(N->right, level + 1, policy);
    
    if (policy == AVL){
        for (i = 0; i < level; i++)
            printf("       ");
        printf("%5d-%d\n", N->key, N->height);
    } else{
        for (i = 0; i < level; i++)
            printf("     ");
        printf("%5d\n", N->key);
    }

    ugly_print(N->left, level + 1, policy);
}

/* Basic validation function for tree T */
void bst_debug_validate(BST *T)
{
    int size = 0;
    assert(bst_debug_validate_rec(T->root, INT_MIN, INT_MAX, &size) == TRUE);
    assert(size == T->size);
    if (T->policy == AVL)
        rec_height(T->root);
}

/* A tree validation function based on node position
 *
 * N - node to validate tree
 * min - temp min for it's left subtree min
 * max - temp max for it's right subtree max
 */
int bst_debug_validate_rec(BST_Node *N, int min, int max, int *count)
{
    if (N == NULL)
        return TRUE;
    if (N->key <= min || N->key >= max)
        return FALSE;
    assert(N->data_ptr != NULL);
    *count += 1;
    return bst_debug_validate_rec(N->left, min, N->key, count) && bst_debug_validate_rec(N->right, N->key, max, count);
}

/* Verifies AVL tree properties */
int rec_height(BST_Node *N)
{
    if (N == NULL)
        return 0;
    int lefthand = rec_height(N->left);
    int righthand = rec_height(N->right);
    int unbalance = lefthand - righthand;
    assert(unbalance == 0 || unbalance == 1 || unbalance == -1);
    return 1 + MYMAX(lefthand, righthand);
}

/* Recursive function to count children */
int children(BST_Node *N)
{
    if (N == NULL)
        return 0;
    return 1 + children(N->left) + children(N->right);
}

/* Prints the tree to the terminal in ASCII art*/
void pretty_print(BST *T)
{
    typedef struct queue_tag{
        BST_Node *N;
        int level;
        int list_sum;
    } Queue;

    Queue Q[T->size];
    int q_head = 0;
    int q_tail = 0;
    int i, j;
    int current_level = 0;
    int col_cnt = 0;
    BST_Node *N;

    Q[q_tail].N = T->root;
    Q[q_tail].level = 0;
    Q[q_tail].list_sum = 0;
    q_tail++;
    for (i = 0; i < T->size; i++){
        assert(q_head < T->size);
        N = Q[q_head].N;

        if (Q[q_head].level > current_level){
            printf("\n");
            current_level++;
            col_cnt = 0;
        }
        
        int left_ch = children(N->left);
        int my_pos = 1 + left_ch + Q[q_head].list_sum;
        int left_child_pos = my_pos;
        if (N->left != NULL)
            left_child_pos = 1 + Q[q_head].list_sum + children(N->left->left);
        int right_child_pos = my_pos;
        if (N->right != NULL)
            right_child_pos = my_pos + 1 + children(N->right->left);
        
        for (j = col_cnt + 1; j <= right_child_pos; j++){
            if (j == my_pos)
                if (left_child_pos < my_pos)
                    if (N->key < 10)
                        printf("--%d", N->key);
                    else if (N->key < 100)
                        printf("-%d", N->key);
                    else
                        printf("%d", N->key);
                else
                    printf("%3d", N->key);
            else if (j == left_child_pos)
                // printf("  |");
                printf("  /");
            else if (j > left_child_pos && j < my_pos)
                printf("---");
            else if (j > my_pos && j < right_child_pos)
                printf("---");
            else if (j == right_child_pos)
                // printf("--|");
                printf("-\\ ");
            else
                printf("   ");
        }

        col_cnt = right_child_pos;

        if (N->left != NULL){
            Q[q_tail].N = N->left;
            Q[q_tail].level = Q[q_head].level + 1;
            Q[q_tail].list_sum = Q[q_head].list_sum;
            q_tail++;
        }

        if (N->right != NULL){
            Q[q_tail].N = N->right;
            Q[q_tail].level = Q[q_head].level + 1;
            Q[q_tail].list_sum = Q[q_head].list_sum + left_ch + 1;
            q_tail++;
        }

        q_head++;
    }

    printf("\n");
}

// Helper functions

void bst_destruct_nodes(BST_Node *toptop)
{
    if (toptop == NULL)
        return;
    bst_destruct_nodes(toptop->left);
    bst_destruct_nodes(toptop->right);
    free(toptop->data_ptr);
    toptop->data_ptr = NULL;
    toptop->left = NULL;
    toptop->right = NULL;
    free(toptop);
}

int bst_path_len(BST_Node *N, int depth)
{
    if (N == NULL)
        return 0;
    return depth + bst_path_len(N->left, depth + 1) + bst_path_len(N->right, depth + 1);
}

BST_Node *bst_insert_node(BST_Node *N, BST_Key key, Data elem_ptr, int policy)
{
    if (N == NULL)
    {
        BST_Node *NN = (BST_Node *)malloc(sizeof(BST_Node));
        NN->data_ptr = elem_ptr;
        NN->key = key;
        NN->left = NULL;
        NN->right = NULL;
        NN->height = 0;
        return NN;
    }
    (key < N->key) ? (N->left = bst_insert_node(N->left, key, elem_ptr, policy)) : (N->right = bst_insert_node(N->right, key, elem_ptr, policy));
    
    N->height = max(find_height(N->left), find_height(N->right)) + 1;
    
    if (policy == AVL){
        int avlbalance = find_balance(N);
        return (avlbalance > 1 && key < N->left->key) ? right_rotation(N) : (avlbalance < -1 && key > N->right->key) ? left_rotation(N)
                : (avlbalance > 1 && key > N->left->key)     ? (N->left = left_rotation(N->left), right_rotation(N))
                : (avlbalance < -1 && key < N->right->key)   ? (N->right = right_rotation(N->right), left_rotation(N))
                : N;
    }

    return N;
}

int find_balance(BST_Node *No)
{
    if (No == NULL)
        return 0;
    return find_height(No->left) - find_height(No->right);
}

int find_height(BST_Node *No)
{
    if (No == NULL)
        return -1;
    return No->height;
}
BST_Node *left_rotation(BST_Node *x)
{
    stats.NumRotations++;
    BST_Node *y = x->right;
    BST_Node *temporary = y->left;
    y->left = x;
    x->right = temporary;
    int lh, rh;
    lh = (x->left == NULL) ? -1 : x->left->height;
    rh = (x->right == NULL) ? -1 : x->right->height;
    x->height = 1 + max(lh, rh);
    lh = (y->left == NULL) ? -1 : y->left->height;
    rh = (y->right == NULL) ? -1 : y->right->height;
    y->height = 1 + max(lh, rh);
    return y;
}

BST_Node *right_rotation(BST_Node *x)
{
    stats.NumRotations++;
    BST_Node *y = x->left;
    BST_Node *temporary = y->right;
    y->right = x;
    x->left = temporary;
    int lh, rh;
    lh = (x->left == NULL) ? -1 : x->left->height;
    rh = (x->right == NULL) ? -1 : x->right->height;
    x->height = 1 + max(lh, rh);
    lh = (y->left == NULL) ? -1 : y->left->height;
    rh = (y->right == NULL) ? -1 : y->right->height;
    y->height = 1 + max(lh, rh);
    return y;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}