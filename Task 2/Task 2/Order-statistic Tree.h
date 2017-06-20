#pragma once

template <class Value>			
class OSTree
{
	enum Paint {Red, Black};
public:	
	struct OSnode {
		Paint color;
		int key;
		Value data;
		int size;
		OSnode *parent,
			*left,
			*right;
	};
	OSnode *root, 
		*nil;

	OSTree()
	{
		nil = new OSnode;
		nil->color = Black;
		nil->size = 0;
		root = nil;
	};
	~OSTree()
	{
		free(root);
		delete nil;
	};

	int select(int start_key, int i)
	{
		return OS_select( RB_search(root, start_key), i )->key;
	};
	int rank(int _key)
	{
		return OS_rank( RB_search(root, _key) );
	};
	int count()
	{
		return RB_count(root);
	};
	Value* search(int _key)
	{
		OSnode* temp = RB_search(root, _key);
		if(temp == nil)
			return nullptr;
		return &temp->data;
	};
	void pop(int _key)
	{
		OSnode *temp = RB_search(root, _key);
		if(temp != nil)
			OS_delete(temp);
	};
	void push( int _key, Value& _data)
	{
		OSnode *new_node = new OSnode;
		new_node->key = _key;
		new_node->data = _data;
		new_node->left = nil;
		new_node->right = nil;
		new_node->color = Red;
		new_node->size = 1;
		OS_insert(new_node);
	};
	void clear()
	{
		free(root);
		root = nil;
	};
	int maximum()
	{
		return RB_maximum(root)->key;
	};
	int minimum()
	{
		return RB_minimum(root)->key;
	};

private:

	void free(OSnode *node)
	{
		if(node == nil)
			return;

		if(node->left != nil)
			free(node->left);
		if(node->right != nil)
			free(node->right);
	
		/*if(node->parent != nil) {
			OSnode *temp;
			if(node == node->parent->right)
				temp = node->parent->right;
			else
				temp = node->parent->left;
			temp = nil;
		}*/
		delete node;
	};

	OSnode* OS_select(OSnode *start, int i)
	{
		int temp = start->left->size + 1;
		if( i == temp )
			return start;
		else {
			if(i < temp)
				return OS_select(start->left, i);
			else
				return OS_select(start->right, i - temp);
		}
	};
	int OS_rank(OSnode *node)
	{
		int answer = node->left->size + 1;
		OSnode *temp = node;
		while(temp != root) {
			if(temp == temp->parent->right)
				answer += temp->parent->left->size + 1;
			temp = temp->parent;
		}
		return answer;
	};
	int RB_count(OSnode *node)
	{
		if(node == nil)
			return 0; 
		return 1 + RB_count(node->left) + RB_count(node->right);
	};
	void left_rotate(OSnode *x)
	{
		OSnode *y = x->right;
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

		y->size = x->size;
		x->size = x->left->size + x->right->size + 1;
	};
	void right_rotate(OSnode *y)
	{
		OSnode *x = y->left;
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
	
		x->size = y->size;
		y->size = y->left->size + y->right->size + 1;
	};
	void OS_insert(OSnode *node)
	{
		OSnode *prev = nil,
			*next = root;
		while(next != nil) {
			next->size++;
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
	void insert_fixup(OSnode *node)
	{
		while(node->parent->color == Red) {
			OSnode *great_parent;
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
	void OS_delete(OSnode *node) 
	{
		OSnode *replace, 
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
		if(replace != node) {
			node->key = replace->key;
			node->data = replace->data;
		}
		if(replace->color == Black)
			delete_fixup(x);		
		
		while(replace != nil) {
			replace->size--;
			replace = replace->parent;
		}
	};
	void delete_fixup(OSnode *node) 
	{
		while(node != root && node->color == Black){
			OSnode *parallel;
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
		

	OSnode* RB_successor(OSnode *node)
	{
		if(node->right != nil)
			return RB_minimum(node->right);
		OSnode *answer = node->parent;
		while(answer != nil && node == answer->right) {
			node = answer;
			answer = answer->parent;
		}
		return answer;
	};
	OSnode* RB_maximum(OSnode *node)
	{
		while(node->right != nil)
			node = node->right;
		return node;
	};
	OSnode* RB_minimum(OSnode *node)	
	{
		while(node->left != nil)
			node = node->left;
		return node;
	};
	OSnode* RB_search(OSnode *node, int s_key)
	{
		if(node == nil || node->key == s_key)
			return node;
		if(s_key < node->key)
			return RB_search(node->left, s_key);
		return RB_search(node->right, s_key);
	};
};