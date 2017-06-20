#pragma once

#include <limits.h>
#include <fstream>

template <class Value>
class Binomial_Heap {
public:
	Binomial_Heap();
	~Binomial_Heap();

	struct Node;

	struct Heap {
		Node* head;

		int number_of_nodes;
		
		Heap();
	};

	struct Node {
		int key;
		Value data;
		Node *parent;
		unsigned short degree;
		Node *child,
			*sibling;

		Node();
		Node(int, Value);
	};

	void insert(int key, Value Data);
	void erase(int key);
	int count(bool = false);
	
	Value* minimum();
	int extract_minimum();
	Value* search(int key);
	void decrease(int what_key, int on_what);

	void print()
	{
		std::ofstream fout;
		fout.open("output.txt");

		fout<< "HEAP:\n";
		for(Node* i = pyramid->head; i; i = i->sibling) {
			fout<< i->key<< "\n";
			for(Node* i_child = i; i_child; i_child = i_child->child) {
				for(Node *i_sibling = i_child; i_sibling; i_sibling = i_sibling->sibling)
					fout<< i_sibling->key<< "  ";
				fout<< "\n";
			}
			fout<< "\n------------\n";
		}

		fout.close();
	};
private:
	Heap* pyramid;

	Heap* BH_make_Heap();
	void BH_insert(Heap*&, Node*);
	Node* BH_minimum(Heap*);
	Node* BH_extract_Min(Heap*&);
	Heap* BH_union(Heap*, Heap*);

	void BH_decrease_Key(Heap*, Node*, int);
	void BH_delete(Heap*&, Node*);

	void BH_link(Node*, Node*);
	Node* BH_merge(Heap*, Heap*);
	
	Node* BH_reverse(Node*);
	Node* BH_search(Node*, int);
	int BH_count(Node*);
	void free_memory(Node*);
};

template <class Value>
Binomial_Heap<Value>::Binomial_Heap()
{
	pyramid = BH_make_Heap();
};
template <class Value>
Binomial_Heap<Value>::~Binomial_Heap()
{
	free_memory(pyramid->head);
	delete pyramid;
};

template <class Value>
Binomial_Heap<Value>::Heap::Heap() 
{
	head = nullptr;
	number_of_nodes = 0;
};
template <class Value>
Binomial_Heap<Value>::Node::Node(){};
template <class Value>
Binomial_Heap<Value>::Node::Node(int _key, Value _Data)
{
	key = _key;
	data = _Data;
	parent = nullptr;
	child = nullptr;
	sibling = nullptr;
	degree = 0;
};
template <class Value>
typename Binomial_Heap<Value>::Heap* Binomial_Heap<Value>::BH_make_Heap()
{
	return new Heap();
};
template <class Value>
typename Binomial_Heap<Value>::Node* Binomial_Heap<Value>::BH_minimum(Heap* H)
{
	Node *min_node = nullptr,
		*temp = H->head;
	int min = INT_MAX;

	while(temp) {
		if (temp->key < min) {
			min = temp->key;
			min_node = temp;
		}
		temp = temp->sibling;
	}

	return min_node;
};
template <class Value>
void Binomial_Heap<Value>::BH_link(Node *x, Node *root)
{
	x->parent = root;
	x->sibling = root->child;
	root->child = x;
	root->degree++;
};
template <class Value>
void Binomial_Heap<Value>::BH_insert(Heap *&H, Node *cell)
{
	Heap* temp_H = BH_make_Heap();
	temp_H->head = cell;
	temp_H->number_of_nodes = 1;

	H = BH_union(H, temp_H);
};
template <class Value>
/*delete Node after return!!!*/
typename Binomial_Heap<Value>::Node* Binomial_Heap<Value>::BH_extract_Min(Heap*& H)
{
	int min = INT_MAX;
	Node *root = H->head,
		*prev_root = nullptr,
		*change = nullptr,
		*for_del = nullptr;

	while(root) {
		if(root->key < min) {
			min = root->key;
			change = prev_root;
			for_del = root;
		}
		prev_root = root;
		root = root->sibling;
	}
		
	Node *save = nullptr;
	if(for_del) {
		save = for_del->child;
		if(change)
			change->sibling = for_del->sibling;
		else 
			H->head = for_del->sibling;
	}

	while(save) {
		save->parent = nullptr;
		save = save->sibling;
	}

	for_del->child = BH_reverse(for_del->child);
	Heap *temp_H = BH_make_Heap();
	temp_H->head = for_del->child;
	pyramid = BH_union(H, temp_H);

	H->number_of_nodes--;

	return for_del;
};
template <class Value>
typename Binomial_Heap<Value>::Node* Binomial_Heap<Value>::BH_reverse(Node* curr)
{
	Node *prev = nullptr;
	while(curr) {
		Node *next = curr->sibling;
		curr->sibling = prev;
		prev = curr;
		curr = next;
	}

	return prev;
};
template <class Value>
void Binomial_Heap<Value>::BH_decrease_Key(Heap *H, Node *cell, int _key)
{
	if(_key > cell->key)
		throw "New key bigger than current";
	cell->key = _key;

	Node *temp = cell,
		*p_temp = cell->parent;
	
	while(p_temp && temp->key < p_temp->key ) {
		//std::swap(temp->key, p_temp->key);
		//std::swap(temp->data, p_temp->data);
		
		int temporary_key = temp->key;
		temp->key = p_temp->key;
		p_temp->key = temporary_key;

		
		Value temporary_data = temp->data;
		temp->data = p_temp->data;
		p_temp->data = temporary_data;
		

		temp = p_temp;
		p_temp = temp->parent;
	}
};
template <class Value>
void Binomial_Heap<Value>::BH_delete(Heap*& H, Node*cell)
{
	BH_decrease_Key(H, cell, INT_MIN);
	Node *find = BH_extract_Min(H);
	delete find;
};
template <class Value>
typename Binomial_Heap<Value>::Heap* Binomial_Heap<Value>::BH_union(Heap* H1, Heap* H2)
{
	Heap *H = BH_make_Heap();
	H->head = BH_merge(H1, H2);
	H->number_of_nodes = H1->number_of_nodes + H2->number_of_nodes;

	delete H1;
	delete H2;
	if(!(H->head))
		return H;

	Node *prev = nullptr,
		*curr = H->head,
		*next = H->head->sibling;

	while(next) {
		if(curr->degree != next->degree || 
				(next->sibling && next->sibling->degree == curr->degree)) {
			prev = curr;
			curr = next;
		}
		else {
			if(curr->key <= next->key) {
				curr->sibling = next->sibling;
				BH_link(next, curr);
			}
			else {
				if(!prev)
					H->head = next;
				else
					prev->sibling = next;
				BH_link(curr, next);
				curr = next;
			}
		}
		next = curr->sibling;
	}

	return H;
};
template <class Value>
typename Binomial_Heap<Value>::Node* Binomial_Heap<Value>::BH_merge(Heap *H1, Heap *H2)
{
	Node *bucket1 = H1->head,
		*bucket2 = H2->head,
		*start,
		*curr = new Node();
	start = curr;

	while(bucket1 && bucket2) {
		if(bucket1->degree > bucket2->degree) {
			curr->sibling = bucket2;
			bucket2 = bucket2->sibling;
		}
		else {
			curr->sibling = bucket1;
			bucket1 = bucket1->sibling;
		}
		curr = curr->sibling;
	}

	if(!bucket1) 
		curr->sibling = bucket2;
	else 
		curr->sibling = bucket1;
	
	curr =  start->sibling;
	delete start;
	
	return curr;
};
template <class Value>
typename Binomial_Heap<Value>::Node* Binomial_Heap<Value>::BH_search(Node *start, int _key)
{
	for( Node *curr = start; curr; curr = curr->sibling)
		for( Node *in_curr = curr; in_curr; in_curr = in_curr->child)
			if(in_curr->key == _key)
				return in_curr;

	throw "Lacks element";
};

template <class Value>
int Binomial_Heap<Value>::count(bool hard)
{
	if(!hard)
		return pyramid->number_of_nodes;

	return BH_count(pyramid->head);
};
template <class Value>
void Binomial_Heap<Value>::free_memory(Node* curr)
{
	if(!curr)
		return;

	free_memory(curr->child);
	free_memory(curr->sibling);

	delete curr;
};
template <class Value>
int Binomial_Heap<Value>::BH_count(Node* curr)
{
	if(!curr) 
		return 0;

	return 1 + BH_count(curr->child) + BH_count(curr->sibling);
};

template <class Value>
void Binomial_Heap<Value>::insert(int key, Value Data)
{
	Node *cell = new Node(key, Data);
	BH_insert(pyramid, cell);
};
template <class Value>
void Binomial_Heap<Value>::erase(int key)
{
	Node *find = BH_search(pyramid->head, key);
	BH_delete(pyramid, find);
};	
template <class Value>
Value* Binomial_Heap<Value>::minimum()
{
	Node *min = BH_minimum(pyramid);
	
	return &min->data;
};
template <class Value>
int Binomial_Heap<Value>::extract_minimum()
{
	Node *find = BH_extract_Min(pyramid);

	int answer = find->key;
	delete find;

	return answer;
};
template <class Value>
Value* Binomial_Heap<Value>::search(int _key)
{
	Node *find = BH_search(pyramid->head, _key);
	return &find->data;
};
template <class Value>
void Binomial_Heap<Value>::decrease(int what_key, int on_what)
{
	Node *find = BH_search(pyramid->head, what_key);
	BH_decrease_Key(pyramid, find, on_what);
};
