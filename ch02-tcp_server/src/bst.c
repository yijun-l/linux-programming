#include <stdio.h>
#include "btree.h"

/*
 * Binary Search Tree (BST)
 * Binary search tree is a data structure that quickly allows us to maintain a
 * sorted list of numbers.
 *
 * The properties that separate a binary search tree from a regular binary tree
 * 1. All nodes of left subtree are less than the root node
 * 2. All nodes of right subtree are more than the root node
 * 3. Both subtrees of each node are also BSTs i.e. they have the above two
 * properties
 *
 */

/* insert a node into Binary Search Tree (BST) */
void node_insert(Tree* tree, int val){
    Node* node = node_create(val, NULL, NULL);
    if(tree->root == NULL){
	tree->root = node;
	return;
    }
    Node* tmp = tree->root;
    while(1){
	if(val > tmp->data){
	    if(tmp->right == NULL){
		tmp->right = node;
		return;
	    }else{
		tmp = tmp->right;
		continue;
	    }
	} else{
	    if(tmp->left == NULL){
		tmp->left = node;
		return;
	    }else{
		tmp = tmp->left;
		continue;
	    }
	}
    }
}

/* get the maximum number in a BST */
int maxValue(Tree* bst){
    Node* tmp = bst->root;
    while(1){
	if(tmp->right == NULL){
	    return tmp->data;
	}else{
	    tmp = tmp->right;
	    continue;
	}
    }
}

/* get the minimum number in a BST */
int minValue(Tree* bst){
    Node* tmp = bst->root;
    while(1){
	if(tmp->left == NULL){
	    return tmp->data;
	}else{
	    tmp = tmp->left;
	    continue;
	}
    }
}


int main(){
    int test[7] = {6, 3, 8, 2, 5, 1, 7};
    Tree bst = { NULL };
    for(int i = 0; i < 7; i++){
	node_insert(&bst, test[i]);
    }
    print_trav(bst.root);
    printf("max value: %d\n", maxValue(&bst));
    printf("min value: %d\n", minValue(&bst));
    printf("height: %d\n", getHeight(bst.root));
    return 0;
}
