// COMP2521 23T2 Assignment 1

#ifndef MSET_STRUCTS_H
#define MSET_STRUCTS_H

// DO NOT CHANGE THE NAME OF THIS STRUCT
struct node {
	int item;           // DO NOT CHANGE/REMOVE THIS FIELD
	int count;          // DO NOT CHANGE/REMOVE THIS FIELD
	struct node *left;  // DO NOT CHANGE/REMOVE THIS FIELD
	struct node *right; // DO NOT CHANGE/REMOVE THIS FIELD
	int height;
	struct node *next;
	struct node *prev;
	// You may add more fields here if needed
};

// DO NOT CHANGE THE NAME OF THIS STRUCT
struct mset {
	struct node *tree;  // DO NOT CHANGE/REMOVE THIS FIELD
	int count;
	int size;
	struct node *first;
	struct node *last;
	// You may add more fields here if needed
};

// You may define more structs here if needed

struct linkedNode {
	int count;
	int item;
	struct linkedNode *next;
	struct linkedNode *prev;
};

struct linkedList {
	struct linkedNode *head;
	struct linkedNode *tail;
};

////////////////////////////////////////////////////////////////////////
// Cursors

struct cursor {
	struct node *selected;
	struct node *start;
	struct node *end;
};

////////////////////////////////////////////////////////////////////////

#endif
