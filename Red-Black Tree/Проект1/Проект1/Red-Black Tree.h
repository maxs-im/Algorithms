#pragma once

class RBTree
{
	enum Paint {Red, Black};

public:
	struct RBnode {
		Paint color;
		int key;
		RBnode *parent,
			*left,
			*right;
	};

	RBnode *root, 
		*nil;

	RBTree();
	~RBTree();

	int count();
	bool search(int);
	void pop(int);
	void push(int);
	void clear();
	int maximum();
	int minimum();

private:
	void free(RBnode *);

	int RB_count(RBnode *);
	void left_rotate(RBnode *);
	void right_rotate(RBnode *);
	void RB_insert(RBnode *);
	void insert_fixup(RBnode *);
	void RB_delete(RBnode *);
	void delete_fixup(RBnode *);

	RBnode* RB_successor(RBnode *);
	RBnode* RB_maximum(RBnode *);
	RBnode* RB_minimum(RBnode *);
	RBnode* RB_search(RBnode *, int);

};