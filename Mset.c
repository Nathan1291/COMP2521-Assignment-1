// Implementation of the Multiset ADT using a balanced BST
// COMP2521 23T2 Assignment 1

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Mset.h"
#include "MsetStructs.h"

////////////////////////////////////////////////////////////////////////
// Helper functions


// deletes all nodees within a given tree
void deleteTree(struct node *node) {
	// if the node doesnt exist, do nothing
	if (node == NULL) {
		// Nothing
	}
	// if the node doesn't have any children, just free the node
	else if (node->left == NULL && node->right == NULL) {
		free(node);
	}
	// if the node has children, branch it off to the children then free the node
	else {
		deleteTree(node->right);
		deleteTree(node->left);
		free(node);
	}
}


// obtains the height of a given node
int getHeight(struct node *node) {
	if (node == NULL) {
		return 0;
	}
	return node->height;
}


// creates a new node
struct node *newNode(int item, int count) {
	struct node *node = malloc(sizeof(struct node));
	node->item = item;
	node->count = count;
	node->left = NULL;
	node->right = NULL;
	node->height = 0;
	node->next = NULL;
	node->prev = NULL;
	return node;
}


// gets a maximum of 2 numbers
int max(int a, int b) {
	return (a > b) ? a : b;
}


// gets a minumum of 2 numbers
int min(int a, int b) {
	return (a < b) ? a : b;
}

// Performs a right rotate on the given tree
// 	    y                               x
//     / \     Right Rotation          /  \
//    x   B3   – - – - – - – >        B1   y
//   / \                                  / \
//  B1  B2                               B2  B3

struct node *rightRotate(struct node *y) {
	struct node *x = y->left;
	struct node *B2 = x->right;

	y->left = B2;
	x->right = y;

	y->height = max(getHeight(y->left), getHeight(y->right))+1;
	x->height = max(getHeight(x->left), getHeight(x->right))+1;

	// returns the new root
	return x;
}


// Performs a left rotate on the given tree
//      x							     y
//     / \        Left Rotation         / \
//    B1  y     – - – - – - – >        x   B3
//   	 / \                          / \
//      B2  B3                      B1  B2
struct node *leftRotate(struct node *x) {
	struct node *y = x->right;
	struct node *B2 = y->left;

	y->left = x;
	x->right = B2;

	y->height = max(getHeight(y->left), getHeight(y->right))+1;
	x->height = max(getHeight(x->left), getHeight(x->right))+1;

	// returns the new root
	return y;
}


// Get balance of node
int getBalance(struct node *node) {
    if (node == NULL) {
        return 0;
	}
    return getHeight(node->left) - getHeight(node->right);
}


// inserts a node into the tree given its root
struct node *insertNode(Mset s, struct node *node, int item, int amount) {
	// if there exists no node in the current location, add the new node
	if (node == NULL) {
		struct node *new = newNode(item, amount);
		s->size++;
		s->count += amount;

		if (s->tree == NULL) {
			s->first = new;
			s->last = new;
		}
		else if (s->first->item > item) {
			s->first = new;
		}
		else if (s->last->item < item) {
			s->last = new;
		}
		return new;
	}
	// else we branch into the left branch
	else if (item < node->item) {
		// if we aren't inserting in the next call of insertNode
		if (node->left != NULL) {
			node->left = insertNode(s, node->left, item, amount);
		}
		// if we are inserting in the next call of insertNode
		else {
			node->left = insertNode(s, node->left, item, amount);
			if (node->prev == NULL) {
				node->prev = node->left;
				node->left->next = node;
			}
			else if (node->prev->item < item) {
				node->prev->next = node->left;
				node->left->prev = node->prev;
				node->prev = node->left;
				node->left->next = node;
			}
		}
	}
	// else we branch into the right branch
	else if (item > node->item) {
		if (node->right != NULL) {
			node->right = insertNode(s, node->right, item, amount);
		}
		// if we are inserting in the next call of insert node
		else {
			node->right = insertNode(s, node->right, item, amount);
			if (node->next == NULL) {
				node->next = node->right;
				node->right->prev = node;
			}
			else if (node->next->item > item) {
				node->next->prev = node->right;
				node->right->next = node->next;
				node->next = node->right;
				node->right->prev = node;
			}
		}
	}
	// if it matches, simply add to the count
	else {
		s->count += amount;
		node->count += amount;
	}

	// Updating heights of the nodes
	node->height = max(getHeight(node->left), getHeight(node->right))+1;

	// get the balance of this node
	int balance = getBalance(node);

	// if the node is unbalanced, we deal with 4 cases
	if (balance > 1) {
		// left left rotate
		if (item < node->left->item) {
        	return rightRotate(node);
		}
		// left right rotate
		if (item > node->left->item) {
			node->left = leftRotate(node->left);
        	return rightRotate(node);
		}
	}
	else if (balance < -1) {
		// right right rotate
		if (item > node->right->item) {
        	return leftRotate(node);
		}
		// right left rotate
		if (item < node->right->item) {
			node->right = rightRotate(node->right);
        	return leftRotate(node);
		}
	}
	return node;
}


// searches for a node with a given value in the tree
struct node *searchNode(struct node *node, int item) {
	// case where the node does not exist in the tree, return NULL
	if (node == NULL) {
		return NULL;
	}
	// if the number is larger than the current node number, branch to the right
	else if (item > node->item) {
		return searchNode(node->right, item);
	}
	// if the number is smaller than the current node number, branch to the left
	else if (item < node->item) {
		return searchNode(node->left, item);
	}
	// if the number matches the nodes number, return the node
	return node;
}


// search the largest node in the tree
struct node *searchMax(struct node *node) {
	if (node->right == NULL) {
		return node;
	}
	else {
		return searchMax(node->right);
	}
}


// printes the nodes in inorder traversal
void printNodeInorderTraversal(struct node *node, int max) {
	if (node == NULL) {
		// do nothing
	}
	else {
		// prints left branch
		printNodeInorderTraversal(node->left, max);
		// prints current node
		printf("(%d, %d)", node->item, node->count);
		// we need to leave out the last comma if it is the largest value
		if (node->item != max) {
			printf(", ");
		}
		// prints right branch
		printNodeInorderTraversal(node->right, max);
	}

}

// exact same logic as insertNode but instead of adding to the count, we replace with the highest count
struct node *insertNodeUnion(Mset s, struct node *node, int item, int amount) {
	// if there exists no node in the current location, add the new node
	if (node == NULL) {
		struct node *new = newNode(item, amount);
		s->size++;
		s->count += amount;

		if (s->tree == NULL) {
			s->first = new;
			s->last = new;
		}
		else if (s->first->item > item) {
			s->first = new;
		}
		else if (s->last->item < item) {
			s->last = new;
		}
		return new;
	}
	// else we branch into the left branch
	else if (item < node->item) {
		// if we aren't inserting in the next call of insertNode
		if (node->left != NULL) {
			node->left = insertNodeUnion(s, node->left, item, amount);
		}
		// if we are inserting in the next call of insertNode
		else {
			node->left = insertNodeUnion(s, node->left, item, amount);
			if (node->prev == NULL) {
				node->prev = node->left;
				node->left->next = node;
			}
			else if (node->prev->item < item) {
				node->prev->next = node->left;
				node->left->prev = node->prev;
				node->prev = node->left;
				node->left->next = node;
			}
		}
	}
	// else we branch into the right branch
	else if (item > node->item) {
		if (node->right != NULL) {
			node->right = insertNodeUnion(s, node->right, item, amount);
		}
		// if we are inserting in the next call of insert node
		else {
			node->right = insertNodeUnion(s, node->right, item, amount);
			if (node->next == NULL) {
				node->next = node->right;
				node->right->prev = node;
			}
			else if (node->next->item > item) {
				node->next->prev = node->right;
				node->right->next = node->next;
				node->next = node->right;
				node->right->prev = node;
			}
		}
	}
	// if it matches, simply add to the count
	else {
		s->count += max(node->count, amount) - node->count;
		node->count = max(node->count, amount);
	}

	// Updating heights of the nodes
	node->height = max(getHeight(node->left), getHeight(node->right))+1;

	// get the balance of this node
	int balance = getBalance(node);

	// if the node is unbalanced, we deal with 4 cases
	if (balance > 1) {
		// left left rotate
		if (item < node->left->item) {
        	return rightRotate(node);
		}
		// left right rotate
		if (item > node->left->item) {
			node->left = leftRotate(node->left);
        	return rightRotate(node);
		}
	}
	else if (balance < -1) {
		// right right rotate
		if (item > node->right->item) {
        	return leftRotate(node);
		}
		// right left rotate
		if (item < node->right->item) {
			node->right = rightRotate(node->right);
        	return leftRotate(node);
		}
	}
	return node;
}

// inserting a multiset into another multiset under a union
void insertUnion(Mset s, struct node *node) {
	if (node == NULL) {
		// nothing
	}
	else {
		s->tree = insertNodeUnion(s, s->tree, node->item, node->count);
		insertUnion(s, node->left);
		insertUnion(s, node->right);
	}
}


// create the sum of 2 multisets
void insertIntersection(Mset s, struct node *node, struct node *root) {
	if (node == NULL) {
		// nothing
	}
	else {
		// check if a node exists in the second tree
		struct node *search = searchNode(root, node->item);
		// if the node does exist, add it back in
		if (search != NULL) {
			s->tree = insertNode(s, s->tree, node->item, min(search->count, node->count));
		}
		insertIntersection(s, node->left, root);
		insertIntersection(s, node->right, root);
	}
}


// create the sum of 2 multisets
void insertSum(Mset s, struct node *node) {
	if (node == NULL) {
		// nothing
	}
	else {
		s->tree = insertNode(s, s->tree, node->item, node->count);
		insertSum(s, node->left);
		insertSum(s, node->right);
	}
}


// creates a new multiset based on the difference of 2 multisets
void insertDifference(Mset s, struct node *node, struct node *root) {
	if (node == NULL) {
		// nothing
	}
	else {
		// search for a node in the second tree
		struct node *search = searchNode(root, node->item);
		// if a node was not found, we can go and add the whole node
		if (search == NULL) {
			s->tree = insertNode(s, s->tree, node->item, node->count);
		}
		// if the node was found, check if we can include it, then include it
		else if (node->count > search->count) {
			s->tree = insertNode(s, s->tree, node->item, node->count - search->count);
		}
		insertDifference(s, node->left, root);
		insertDifference(s, node->right, root);
	}
}


// Checks if the first set is included in the second set
bool checkIncluded(struct node *node, struct node *root) {
	if (node == NULL) {
		return true;
	}
	else {
		struct node *search = searchNode(root, node->item);
		if (search == NULL) {
			return false;
		}
		else if (node->count > search->count) {
			return false;
		}
	}
	return checkIncluded(node->left, root) && checkIncluded(node->right, root);
}


// creates a new doubly linked list
struct linkedList *linkedListNew( void) {
	struct linkedList *new = malloc(sizeof(struct linkedList));
	new->head = NULL;
	new->tail = NULL;
	return new;
}


// creates a new node for the doubly linked list
struct linkedNode *linkedNodeNew(int item, int count) {
	struct linkedNode *new = malloc(sizeof(struct linkedNode));
	new->next = NULL;
	new->prev = NULL;
	new->count = count;
	new->item = item;
	return new;
}


// inserts a node into the list ordered by their count
void linkedListInsert(struct linkedList *list, int item, int count) {
	struct linkedNode *head = list->head;
	struct linkedNode *tail = list->tail;
	struct linkedNode *new = linkedNodeNew(item, count);
	// base case when the list is empty
	if (head == NULL) {
		list->head = new;
		list->tail = new;
		return;
	}
	// base case where we insert into the head
	if (count < head->count) {
		new->next = head;
		head->prev = new;
		list->head = new;
		return;
	}
	// All other cases where we insert somewhere in the middle
	struct linkedNode *curr = list->head;
	while (curr != NULL) {
		// case where they have the same count, insert it in descending order
		if (count == curr->count) {
			if (item > curr->item) {
				struct linkedNode *prev = curr->prev;
				new->next = curr;
				new->prev = curr->prev;
				curr->prev = new;
				prev->next = new;
				return;
			}
		}
		// case where the count is lower than the current nodes count
		if (count < curr->count) {
			struct linkedNode *prev = curr->prev;
			new->next = curr;
			new->prev = curr->prev;
			curr->prev = new;
			prev->next = new;
			return;
		}
		curr = curr->next;
	}
	// if it passes through the whole while loop without returning
	// we need to insert at end
	tail->next = new;
	new->prev = tail;
	list->tail = new;
	return;
}


// inserts a bst into a linked list
void MsetInsertList(struct node *node, struct linkedList *list) {
	if (node == NULL) {
		return;
	}
	linkedListInsert(list, node->item, node->count);
	MsetInsertList(node->left, list);
	MsetInsertList(node->right, list);
	return;
}


// gets the k most common elements then inserts it into items array
int getCommon(struct linkedList *list, int k, struct item items[]) {
	struct linkedNode *curr = list->tail;
	int i = 0;
	while (curr != NULL && i < k) {
		items[i].item = curr->item;
		items[i].count = curr->count;
		i++;
		curr = curr->prev;
	}
	return i;
}


// frees the created linked list
void freeList(struct linkedList *list) {
	struct linkedNode *curr = list->head;
	while (curr != NULL) {
		struct linkedNode *temp = curr;
		curr = curr->next;
		free(temp);
	}
	free(list);
}


////////////////////////////////////////////////////////////////////////
// Basic Operations

/**
 * Creates a new empty mset
 */
Mset MsetNew(void) {
	Mset new = malloc(sizeof(struct mset));
	new->tree = NULL;
	new->size = 0;
	new->count = 0;
	new->first = NULL;
	new->last = NULL;
	return new;
}


/**
 * Frees all memory allocated to the given mset
 */
void MsetFree(Mset s) {
	struct node *root = s->tree;
	deleteTree(root);
	free(s);
}


/**
 * Inserts one of an item into the mset. Does nothing if the item is
 * equal to UNDEFINED.
 */
void MsetInsert(Mset s, int item) {
	struct node *root = s->tree;
	if (item == UNDEFINED) {
		// nothing
	}
	else {
		int amount = 1;
		s->tree = insertNode(s, root, item, amount);
	}
}


/**
 * Inserts the given amount of an item into the mset. Does nothing if
 * the item is equal to UNDEFINED or the given amount is 0 or less.
 */
void MsetInsertMany(Mset s, int item, int amount) {
		struct node *root = s->tree;
	if (item == UNDEFINED) {
		// nothing
	}
	else if (amount <= 0) {
		// nothing
	}
	else {
		s->tree = insertNode(s, root, item, amount);
	}
}


/**
 * Returns the number of distinct elements in the mset
 */
int MsetSize(Mset s) {
	return s->size;
}


/**
 * Returns the sum of counts of all elements in the mset
 */
int MsetTotalCount(Mset s) {
	return s->count;
}


/**
 * Returns the count of an item in the mset, or 0 if it doesn't occur in
 * the mset
 */
int MsetGetCount(Mset s, int item) {
	struct node *node = searchNode(s->tree, item);
	if (node != NULL) {
		return node->count;
	}
	return 0;
}


/**
 * Prints the given mset in the format
 * {(elem1, count1), (elem2, count2), (elem3, count3)}
 * where the elements are in increasing order
 */
// solution to remove the comma from the last print
// search for the right most node, then check if they are equal
// if they are then dont print, so we get commas on everything but the last entry
// probably faster than doing a queue/linked list solution or something
void MsetShow(Mset s) {
	struct node *max = searchMax(s->tree);
	printf("{");
	printNodeInorderTraversal(s->tree, max->item);
	printf("}");
}

////////////////////////////////////////////////////////////////////////
// Advanced Operations

/**
 * Returns a new mset representing the union of the two msets
 */
Mset MsetUnion(Mset s1, Mset s2) {
	Mset s3 = MsetNew();
	insertUnion(s3, s1->tree);
	insertUnion(s3, s2->tree);
	return s3;
}

/**
 * Returns a new mset representing the intersection of two msets
 */
Mset MsetIntersection(Mset s1, Mset s2) {
	Mset s3 = MsetNew();
	insertIntersection(s3, s1->tree, s2->tree);
	return s3;
}

/**
 * Returns a new mset representing the addition of two msets
 */
Mset MsetSum(Mset s1, Mset s2) {
	Mset s3 = MsetNew();
	insertSum(s3, s1->tree);
	insertSum(s3, s2->tree);
	return s3;
}

/**
 * Returns a new mset representing the difference s1 - s2
 */
Mset MsetDifference(Mset s1, Mset s2) {
	Mset s3 = MsetNew();
	insertDifference(s3, s1->tree, s2->tree);
	return s3;
}

/**
 * Returns true if the mset s1 is included in the mset s2, and false
 * otherwise
 */
bool MsetIncluded(Mset s1, Mset s2) {
	return checkIncluded(s1->tree, s2->tree);
}

/**
 * Returns true if the two given msets are equal, and false otherwise
 */
bool MsetEquals(Mset s1, Mset s2) {
	return MsetIncluded(s1, s2) && MsetIncluded(s2, s1);
}

/**
 * Stores the k most common elements in the mset into the given items
 * array in decreasing order of count and returns the number of elements
 * stored. Elements with the same count should be stored in increasing
 * order.
 */

// creates a linked list based on the count (sorted in ascending order)
// we can then access the last element of the linked list and go backwards to find
// MsetMostCommon
int MsetMostCommon(Mset s, int k, struct item items[]) {
	if (k <= 0) {
		return 0;
	}
	struct linkedList *newList = linkedListNew();
	MsetInsertList(s->tree, newList);
	int num = getCommon(newList, k, items);
	freeList(newList);
	return num;
}

////////////////////////////////////////////////////////////////////////
// Cursor Operations

typedef struct cursor *MsetCursor;

/**
 * Creates a new cursor positioned at the start of the mset
 */
MsetCursor MsetCursorNew(Mset s) {
	struct node *start = malloc(sizeof(struct node));
	start->left = NULL;
	start->right = NULL;
	start->item = -1;
	start->count = 0;
	start->next = s->first;
	start->prev = NULL;
	s->first->prev = start;

	struct node *end = malloc(sizeof(struct node));
	end->left = NULL;
	end->right = NULL;
	end->item = 0;
	end->count = -1;
	end->prev = s->last;
	end->next = NULL;
	s->last->next = end;

	struct cursor *cursor = malloc(sizeof(struct cursor));
	cursor->selected = start;
	cursor->start = start;
	cursor->end = end;
	return cursor;
}

/**
 * Frees all memory allocated to the given cursor
 */
void MsetCursorFree(MsetCursor cur) {
	struct node *start = cur->start;
	struct node *end = cur->end;
	start->next->prev = NULL;
	end->prev->next = NULL;
	free(start);
	free(end);
	free(cur);
}

/**
 * Returns the element at the cursor's position and its count, or
 * {UNDEFINED, 0} if the cursor is positioned at the start or end of
 * the mset.
 */
struct item MsetCursorGet(MsetCursor cur) {
	struct node *curr = cur->selected;
	if (curr->item == -1) {
		return (struct item){UNDEFINED, 0};
	}
	return (struct item){curr->item, curr->count};
}

/**
 * Moves the cursor to the next greatest element, or to the end of the
 * mset if there is no next greatest element. Returns false if the
 * cursor is now at the end of the mset, and true otherwise.
 */
bool MsetCursorNext(MsetCursor cur) {
	struct node *curr = cur->selected;
	// case where it is end of set
	if (curr->next == NULL) {
		return true;
	}
	cur->selected = curr->next;
	if (cur->selected->count == -1) {
		return false;
	}
	return true;
}

/**
 * Moves the cursor to the next smallest element, or to the start of the
 * mset if there is no next smallest element. Returns false if the
 * cursor is now at the start of the mset, and true otherwise.
 */
bool MsetCursorPrev(MsetCursor cur) {
	struct node *curr = cur->selected;
	// case where it is start of set
	if (curr->prev == NULL) {
		return true;
	}
	cur->selected = curr->prev;
	if (cur->selected->item == -1) {
		return false;
	}
	cur->selected = curr->prev;
	return true;
}

////////////////////////////////////////////////////////////////////////
