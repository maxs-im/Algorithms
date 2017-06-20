#pragma once

#include <vector>
#include <fstream>
#include <iomanip>
#include <limits.h>

template <class Value>
class Fibonacci_Heap 
{
public:
	Fibonacci_Heap();
	~Fibonacci_Heap();

	struct Node;

	struct Heap {
		unsigned int nodes_number;
		unsigned int trees_number;
		unsigned int marked_nodes;
		Node* min;
		
		Heap();
	};
	struct Node {
		unsigned int degree;
		bool mark;
		
		int key;
		Value data;
		
		Node *parent,
			*child,
			*left,
			*right;
		
		Node(int, Value);
	};

	void decrease(int what_key, int on_what);
	Value* search(int key);
	int extract_minimum();
	Value* minimum();
	void insert(int key, Value Data);
	void erase(int key);
	int count(bool hard = false);
	int potential();

	void print(bool add = false)
	{
		if(!add)
			fout.open("output.txt");
		else
			fout.open("output.txt", std::ios_base::app);

		std::vector<Node*> vec;
		vec.push_back(pyramid->min);
		fout<< "\n_HEAP_:\n\n";
		rec_print(vec, 0);
		fout<< "\n_END_\n";
		fout.close();
	};

private:
	std::ofstream fout;
	void rec_print(std::vector<Node*> vec, int level) 
	{

		for(auto it : vec) {
			std::vector<Node*> new_vec;
			fout<< std::setfill('\t')<< std::setw(level)<< "";
			Node *start = it,
				*curr = it;
			do {
				if(curr->child) {
					new_vec.push_back(curr->child);
					fout<< "+";
				}
				fout<< curr->data.key << "  ";					// TO PRINT AUTHOR AND NAME;

				curr = curr->right;
			} while(curr != start);
			fout<< "\n";
			if(!(new_vec.empty()))
				rec_print(new_vec, level + 1);
		}
	};

	Heap* pyramid;
	
	Heap* FH_make();
	void FH_insert(Heap*&, Node*);
	Node* FH_minimum(Heap*);
	Heap* FH_union(Heap *, Heap *);
	Node* FH_extract_min(Heap*&);
	void FH_consolidate(Heap*&);
	void FH_link(Heap*&, Node*, Node*);
	void FH_decrease_key(Heap*&, Node*, int);
	void FH_cut(Heap*&, Node*, Node*);
	void FH_cascading_cut(Heap*&, Node*);
	void FH_delete(Heap*&, Node*);

	void FH_add_to_list(Node *&, Node *);
	void FH_delete_from_list(Node*&);
	int FH_count(Node*);
	Node* FH_search(Node*, int);

	void free_memory(Node*);
};

template <class Value>
Fibonacci_Heap<Value>::Fibonacci_Heap()
{
	pyramid = FH_make();
};
template <class Value>
Fibonacci_Heap<Value>::~Fibonacci_Heap()
{
	free_memory(pyramid->min);
	delete pyramid;
};

template <class Value>
Fibonacci_Heap<Value>::Heap::Heap() : min(nullptr), 
	nodes_number(0), trees_number(0), marked_nodes(0)
{};
template <class Value>
Fibonacci_Heap<Value>::Node::Node(int _key, Value _Data) : left(this), right(this), 
															parent(nullptr), child(nullptr)
{
	degree = 0;
	mark = false;
	key = _key;
	data = _Data;
};

template <class Value>
typename Fibonacci_Heap<Value>::Heap* Fibonacci_Heap<Value>::FH_make()
{
	return new Heap();
};
template <class Value>
void Fibonacci_Heap<Value>::FH_insert(Heap *&H, Node *cell)
{
	if(H->min) {
		FH_add_to_list(H->min, cell);
		cell->parent = nullptr;
	}
	
	if(!(H->min) || cell->key < H->min->key)
		H->min = cell;
	H->nodes_number++;
	H->trees_number++;
};
template <class Value>
typename Fibonacci_Heap<Value>::Node* Fibonacci_Heap<Value>::FH_minimum(Heap* H)
{
	return H->min;
};
template <class Value>
typename Fibonacci_Heap<Value>::Heap* Fibonacci_Heap<Value>::FH_union(Heap *H1, Heap *H2)
{
	if(!(H1->min) || (H2->min))
		throw "Empty heap";

	Heap *H = FH_make();
	H->min = H1->min;
	
	//add roots from H2 to H
	Node *temp_H = H->min->right,
		*temp_H2 = H2->min->left;
	H->min->right = H2->min;
	H2->min->left = H->min;
	temp_H->left = temp_H2;
	temp_H2->right = temp_H;

	
	if(!(H1->min) || (H2->min && H2->min->key < H1->min->key))
		H->min = H2->min;

	H->nodes_number	 = H1->nodes_number + H2->nodes_number;
	H->trees_number = H1->trees_number + H2->trees_number;
	delete H1;
	delete H2;

	return H;
};
template <class Value>
typename Fibonacci_Heap<Value>::Node* Fibonacci_Heap<Value>::FH_extract_min(Heap *&H)	
{
	Node* ans = H->min;
	if(ans) {
		Node *add = ans->child,
			*next = nullptr, *start = ans->child;
		while(next != start){
			next = add->right;
			FH_add_to_list(H->min, add);
				add->parent = nullptr;
			H->trees_number++;
			add = next;
		}

		//delete ans from H
		FH_delete_from_list(ans);

		H->trees_number--;

		if(ans == ans->right)
			H->min = nullptr;
		else {
			H->min = ans->right;
			FH_consolidate(H);
		}

		H->nodes_number--;
	}
	return ans;
};
template <class Value>
void Fibonacci_Heap<Value>::FH_add_to_list(Node *&list, Node *insert)/*don't forget about parent*/
{
	if(!list) {
		list = insert;
		insert->right = insert;
		insert->left = insert;
		return;
	}
	Node *temp = list->left;
	list->left = insert;
	insert->right = list;
	insert->left = temp;
	temp->right = insert;
};
template <class Value>
void Fibonacci_Heap<Value>::FH_delete_from_list(Node *&cell)
{
	Node *temp_l = cell->left,
		*temp_r = cell->right;
	temp_l->right = temp_r;
	temp_r->left = temp_l;
	
	if(cell->parent && (cell->parent->child == cell)) {
		if(temp_r == cell)
			cell->parent->child = nullptr;
		else
			cell->parent->child = temp_r;
	}
};
template <class Value>
void Fibonacci_Heap<Value>::FH_link(Heap *&H, Node* cell, Node* root)
{
	FH_delete_from_list(cell);
	FH_add_to_list(root->child, cell);
		cell->parent = root;
	root->degree++;
	if(cell->mark) {
		cell->mark = false;
		H->marked_nodes--;
	}
};
template <class Value>
void Fibonacci_Heap<Value>::FH_consolidate(Heap *&H)
{
	double complexity = log(H->nodes_number)/log(2) + 1;
	int border = (int)complexity;
	std::vector<Node*> mass;
	mass.resize(border+1);
	for(int i = 0; i <= border; i++)
		mass[i] = nullptr;

	Node *start = H->min,
		*curr = H->min;
	do {
		Node *x = curr;
		curr = curr->right;

		unsigned int deg = x->degree;
		while(mass[deg]) {
			Node *y = mass[deg];
			if(x->key > y->key) {
				//std::swap(x, y);
				Node* temp = x;
				x = y;
				y = temp;
			}
			FH_link(H, y ,x);
			mass[deg] = nullptr;
			deg++;
		}
		mass[deg] = x;

	} while( curr != start);

	H->min = nullptr;
	H->trees_number = 0;

	for(int i = 0; i <= border; i++) {
		if(mass[i]) {
			FH_add_to_list(H->min, mass[i]);
				mass[i]->parent = nullptr;
			H->trees_number++;
			if(!(H->min) || (mass[i]->key < H->min->key))
				H->min = mass[i];
		}
	}
};
template <class Value>
void  Fibonacci_Heap<Value>::FH_decrease_key(Heap *&H, Node* cell, int _key)
{
	if(_key > cell->key)
		throw "New key bigger than current";

	cell->key = _key;
	Node *temp = cell->parent;

	if(temp && (cell->key < temp->key)) {
		FH_cut(H, cell, temp);
		FH_cascading_cut(H, temp);
	}
	if(cell->key < H->min->key)
		H->min = cell;
};
template <class Value>

void  Fibonacci_Heap<Value>::FH_cut(Heap *&H, Node* cell, Node* root)
{
	FH_delete_from_list(cell);
	root->degree--;
	FH_add_to_list(H->min, cell);
		cell->parent = nullptr;
	H->trees_number++;
	if(cell->mark) {
		cell->mark = false;
		H->marked_nodes--;
	}
};
template <class Value>
void  Fibonacci_Heap<Value>::FH_cascading_cut(Heap *&H, Node* cell)
{
	Node* temp = cell->parent;
	if(temp) {
		if(!(cell->mark)) {
			cell->mark = true;
			H->marked_nodes++;
		}
		else {
			FH_cut(H, cell, temp);
			FH_cascading_cut(H, temp);
		}
	}
};
template <class Value>
void  Fibonacci_Heap<Value>::FH_delete(Heap *&H, Node* cell)
{
	FH_decrease_key(H, cell, INT_MIN);
	Node* find = FH_extract_min(H);

	delete find;
};

template <class Value>
int Fibonacci_Heap<Value>::count(bool hard)
{
	if(!hard)
		return pyramid->nodes_number;

	return FH_count(pyramid->min);
};
template <class Value>
void Fibonacci_Heap<Value>::free_memory(Node* curr)
{
	if(!curr)
		return;

	Node* start = curr;
	do {
		Node *temp = curr;
		free_memory(temp->child);
		curr = temp->right;

		delete temp;
	} while(curr != start);

};
template <class Value>
int Fibonacci_Heap<Value>::FH_count(Node* curr)
{
	if(!curr) 
		return 0;
	
	int on_level = 0;
	Node* start = curr;
	do {
		on_level = on_level + (1 + FH_count(curr->child));
		curr = curr->right;
	} while(curr != start);

	return on_level;
};
template <class Value>
typename Fibonacci_Heap<Value>::Node* Fibonacci_Heap<Value>::FH_search(Node *cell, int _key)
{
	if(!cell)
		return nullptr;
	Node *curr = cell;

	Node *find = nullptr,
		*start = curr;
	do {
		if(curr->key == _key)
			return curr;
		
		if(curr->key < _key)
			find = FH_search(curr->child, _key);

		curr = curr->right;
	} while((curr != start) && !find);
	
	return find;
};

template <class Value>
void Fibonacci_Heap<Value>::insert(int key, Value Data)
{
	Node *cell = new Node(key, Data);
	FH_insert(pyramid, cell);
};
template <class Value>
void Fibonacci_Heap<Value>::erase(int key)
{
	Node *find = FH_search(pyramid->min, key);
	if(!find)
		throw "Lacks element";

	FH_delete(pyramid, find);
};	
template <class Value>
Value* Fibonacci_Heap<Value>::minimum()
{
	Node *min = FH_minimum(pyramid);
	
	return &min->data;
};
template <class Value>
int Fibonacci_Heap<Value>::extract_minimum()
{
	Node *find = FH_extract_min(pyramid);
	if(!find)
		throw "Empty pyramid";

	int answer = find->key;
	
	delete find;

	return answer;
};
template <class Value>
Value* Fibonacci_Heap<Value>::search(int _key)
{
	Node *find = FH_search(pyramid->min, _key);
	if(!find)
		throw "Lacks element";
	return &find->data;
};
template <class Value>
void Fibonacci_Heap<Value>::decrease(int what_key, int on_what)
{
	Node *find = FH_search(pyramid->min, what_key);
	if(!find)
		throw "Lacks element";
	FH_decrease_key(pyramid, find, on_what);
};
template <class Value>
int Fibonacci_Heap<Value>::potential()
{
	return pyramid->trees_number + 2*pyramid->marked_nodes;
};
