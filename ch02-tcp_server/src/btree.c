#include <stdio.h>
#include <stdlib.h>
#include "btree.h"

/* create a node */
Node* node_create(int data, Node* left, Node* right){
    Node* npt = (Node*)malloc(sizeof(Node));
    npt->data = data;
    npt->left = left;
    npt->right = right;
}

/* Pre-order traversal: Node > Left > Right */
void preorder(Node* node){
    if(node){
	printf(">> %d ", node->data);
	preorder(node->left);
	preorder(node->right);
    }	
}

/* Post-order traversal: Left > Right > Node */
void postorder(Node* node){
    if(node){
	postorder(node->left);
	postorder(node->right);
	printf(">> %d ", node->data);
    }	
}

/* In-order traversal: Left > Node > Right */
void inorder(Node* node){
    if(node){
	inorder(node->left);
	printf(">> %d ", node->data);
	inorder(node->right);
    }	
}

/* print traversal result */
void print_trav(Node* root){
    printf("\nPre-order traversal:\n");
    preorder(root);
    printf("\n\nPost-order traversal:\n");
    postorder(root);
    printf("\n\nIn-order traversal:\n");
    inorder(root);
    printf("\n\n");
}

/* get the height of a node */
int getHeight(Node* node){
    if(node == NULL){
	return 0;
    }
    int lval = getHeight(node->left);
    int rval = getHeight(node->right);
    int max = (lval > rval) ? lval : rval;
    return max + 1;
}
