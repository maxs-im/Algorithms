#include "Red-Black Tree.h"

RBTree::RBTree()
{
	nil = new RBnode;
	nil->color = Black;
	root = nil;
};
RBTree::~RBTree()
{
	free(root);
	if(nil != nullptr)
		delete nil;
	nil = nullptr;
};
int RBTree::count()
{
	return RB_count(root);
};
bool RBTree::search(int _key)
{
	if(RB_search(root, _key) == nil)
		return false;
	return true;
};
void RBTree::pop(int _key)
{
	RBnode *temp = RB_search(root, _key);
	if(temp != nil)
		RB_delete(temp);
};
void RBTree::push(int _key)
{
	RBnode *new_node = new RBnode;
	new_node->key = _key;
	new_node->left = nil;
	new_node->right = nil;
	new_node->color = Red;
		RB_insert(new_node);
};
void RBTree::clear()
{
	free(root);
	root = nil;
};
int RBTree::maximum()
{
	return RB_maximum(root)->key;
};
int RBTree::minimum()
{
	return RB_minimum(root)->key;
};

void RBTree::free(RBnode *node)
{
	if(node == nil || nil == nullptr)
		return;

	if(node->left != nil)
		free(node->left);
	if(node->right != nil)
		free(node->right);
	
	/*if(node->parent != nil) {
		RBnode *temp;
		if(node == node->parent->right)
			temp = node->parent->right;
		else
			temp = node->parent->left;
		temp = nil;
	}*/
	delete node;
};

int RBTree::RB_count(RBnode *node)
{
	if(node == nil)
		return 0; 
	return 1 + RB_count(node->left) + RB_count(node->right);
};
void RBTree::left_rotate(RBnode *x)
{
	RBnode *y = x->right;
		x->right = y->left;
	if(y->left != nil)
		y->left->parent = x;
	y->left = x;
		y->parent = x->parent;
	if(x->parent == nil)
		root = y;
	else {
		if(x == x->parent->left)
			x->parent->left = y;
		else
			x->parent->right = y;
	}
	x->parent = y;
};
void RBTree::right_rotate(RBnode *y)
{
	RBnode *x = y->left;
		y->left = x->right;
	if(x->right != nil)
		x->right->parent = y;
	x->right = y;
		x->parent = y->parent;
	if(y->parent == nil)
		root = x;
	else {
		if(y == y->parent->right)
			y->parent->right = x;
		else
			y->parent->left = x;
	}
	y->parent = x;
};
void RBTree::RB_insert(RBnode *node)
{
	RBnode *prev = nil,
		*next = root;
	while(next != nil) {
		prev = next;
		if(node->key < next->key)
			next = next->left;
		else
			next = next->right;
	}
	node->parent = prev;
	if(prev == nil)
		root = node;
	else {
		if(node->key < prev->key)
			prev->left = node;
		else
			prev->right = node;
	}
		
	insert_fixup(node);
};
void RBTree::insert_fixup(RBnode *node)
{
	while(node->parent->color == Red) {
		RBnode *great_parent;
		if(node->parent == node->parent->parent->left) {
			great_parent = node->parent->parent->right;
			if(great_parent->color == Red) {
				node->parent->color = Black;
				great_parent->color = Black;
				node->parent->parent->color = Red;
				node = node->parent->parent;
			}
			else {
				if(node == node->parent->right) {
					node = node->parent;
					left_rotate(node);
				}
				node->parent->color = Black;
				node->parent->parent->color = Red;
				right_rotate(node->parent->parent);
			}
		}
		else {
			great_parent = node->parent->parent->left;
			if(great_parent->color == Red) {
				node->parent->color = Black;
				great_parent->color = Black;
				node->parent->parent->color = Red;
				node = node->parent->parent;
			}
			else {
				if(node == node->parent->left) {
					node = node->parent;
					right_rotate(node);
				}
				node->parent->color = Black;
				node->parent->parent->color = Red;
				left_rotate(node->parent->parent);
			}
		}
	}
	root->color = Black;
};
void RBTree::RB_delete(RBnode *node)
{
	RBnode *replace, 
		*x;
	
	if(node->left == nil || node->right == nil)
		replace = node;
	else
		replace = RB_successor(node);
	
	if(replace->left != nil)
		x = replace->left;
	else
		x = replace->right;
	
	x->parent = replace->parent;
	if(replace->parent == nil)
		root = x;
	else {
		if(replace == replace->parent->left)
			replace->parent->left = x;
		else
			replace->parent->right = x;
	}
		if(replace != node)
			node->key = replace->key;
		if(replace->color == Black)
			delete_fixup(x);		
};
void RBTree::delete_fixup(RBnode * node) 
{
	while(node != root && node->color == Black){
		RBnode *parallel;
		if(node == node->parent->left) {
			parallel = node->parent->right;
			if(parallel->color == Red) {
				parallel->color = Black;
				node->parent->color = Red;
				left_rotate(node->parent);
				parallel = node->parent->right;
			}
			if(parallel->left->color == Black && parallel->right->color == Black) {
				parallel->color = Red;
				node = node->parent;
			}
			else {
				if(parallel->right->color == Black) {
					parallel->left->color = Black;
					parallel->color = Red;
					right_rotate(parallel);
					parallel = node->parent->right;
				}
				parallel->color = node->parent->color;
				node->parent->color = Black;
				parallel->right->color = Black;
				left_rotate(node->parent);
				node = root;
			}
		}
		else {
			parallel = node->parent->left;
			if(parallel->color == Red) {
				parallel->color = Black;
				node->parent->color = Red;
				right_rotate(node->parent);
				parallel = node->parent->left;
			}
			if(parallel->left->color == Black && parallel->right->color == Black) {
				parallel->color = Red;
				node = node->parent;
			}
			else {
				if(parallel->left->color == Black) {
					parallel->right->color = Black;
					parallel->color = Red;
					left_rotate(parallel);
					parallel = node->parent->left;
				}
				parallel->color = node->parent->color;
				node->parent->color = Black;
				parallel->left->color = Black;
				right_rotate(node->parent);
				node = root;
			}
		}
	}
	node->color = Black;
};

RBTree::RBnode* RBTree::RB_successor(RBnode *node)
{
	if(node->right != nil)
		return RB_minimum(node->right);
	RBnode *answer = node->parent;
	while(answer != nil && node == answer->right) {
		node = answer;
		answer = answer->parent;
	}
	return answer;
};
RBTree::RBnode* RBTree::RB_maximum(RBnode *node)
{
	while(node->right != nil)
		node = node->right;
	return node;
};
RBTree::RBnode* RBTree::RB_minimum(RBnode *node)
{
	while(node->left != nil)
		node = node->left;
	return node;
};
RBTree::RBnode* RBTree::RB_search(RBnode *node, int s_key)
{
	if(node == nil || node->key == s_key)
		return node;
		if(s_key < node->key)
		return RB_search(node->left, s_key);
	return RB_search(node->right, s_key);
};