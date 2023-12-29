#ifndef _BTREE_H
#define _BTREE_H

/*
 * Binary Tree
 * A binary tree is a tree data structure in which each parent node can have at
 * most two children. Each node of a binary tree consists of three items:
 * 1. data item
 * 2. address of left child
 * 3. address of right child
 */

typedef struct node{
    int data;
    struct node* left;
    struct node* right;
} Node;

typedef struct tree{
    Node* root;
} Tree;


/* create a node */
extern Node* node_create(int data, Node* left, Node* right);

/* Pre-order traversal: Node > Left > Right */
extern void preorder(Node* node);

/* Post-order traversal: Left > Right > Node */
extern void postorder(Node* node);

/* In-order traversal: Left > Node > Right */
extern void inorder(Node* node);

/* print traversal result */
extern void print_trav(Node* root);

/* get the height of a node */
int getHeight(Node* node);

#endif
