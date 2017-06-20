/*
			Optimal binary seach tree
*/

#pragma once

#include <vector>
#include <algorithm>

#define VAL INT_MAX

template <class Value>
class O_BSTree {
public:
	struct Node {
		int key;
		Value dat;
		Node *left,
			*right;

		Node(Value, unsigned int);
	};

	O_BSTree( const std::vector<Value>& input_Data, const std::vector<unsigned int> = std::vector<unsigned int>(0, 0));
	~O_BSTree();

	void generate(int number, int start = 0);
	Node* get_pointer()
	{
		return tree_root;
	};
	int get_cost();
	int count();

private:
	Node* tree_root;
	int amount;
	int tree_costs;

	std::vector<int> weight;
	std::vector<int> costs;
	std::vector<int> root;

	std::vector<Value> Data;

	int calc_weight(int, int);
	int find_min(int, int);

	void construct_tree(int, int);
	Node* add_Node(int number) 
	{
		Node* cell = new Node(Data[number], calc_weight(number, number));
		return cell;
	};
	void insert(Node*, Node*&);
	void clear(Node*);
};

template <class Value>
O_BSTree<Value>::Node::Node(Value insert, unsigned int _frequancy)  
{
	key = _frequancy;
	dat = insert;
	left = nullptr;
	right = nullptr;
};

template <class Value>
O_BSTree<Value>::~O_BSTree()
{
	clear(tree_root);
};

template <class Value>
O_BSTree<Value>::O_BSTree(const std::vector<Value> &input_Data, const std::vector<unsigned int> input_freq) :
	Data(input_Data), tree_root(nullptr)
{
	std::sort(Data.begin(), Data.end(), [](const Value &a, const Value &b) {
		return a.key < b.key;
	});
	
	amount = input_freq.size();

	weight.resize(amount);	
	costs.resize(amount*amount, VAL);
	root.resize(amount*amount);

	weight[0] = input_freq[0];
	for(int i = 1; i < amount; i++)
		weight[i] = weight[i-1] + input_freq[i]; 

	find_min(0, amount-1);
	generate(amount);					//default construct tree [0 -> amount-1]
};

template <class Value>
void O_BSTree<Value>::generate(int finish, int start)
{
	if(finish < 1 || finish > amount || start < 0 || start > finish)
		throw "Uncorrect input data";

	finish--;
	clear(tree_root);
	tree_root = nullptr;
	
	int number_root = root[start*amount + finish];
	tree_costs = costs[start*amount + finish + start];
	
	construct_tree(start, finish);
};

template <class Value>
int O_BSTree<Value>::calc_weight(int i, int j)
{
	if(i > j)
		return 0;
	
	int temp_w = weight[j];
	if(i > 0)
		temp_w -= weight[i-1];
	
	return temp_w;
};

template <class Value>
int O_BSTree<Value>::count()
{
	return amount;
};

template <class Value>
int O_BSTree<Value>::get_cost()
{
	return tree_costs;
};

template <class Value>
int O_BSTree<Value>::find_min(int start, int finish)
{
	if(finish < start)
		return 0;

	int temp_cost, 
		curr_cell,
		obj = start*amount + finish; 
	if(costs[ obj ] == VAL) {
		for(curr_cell = start; curr_cell <= finish; curr_cell++) {
			temp_cost = calc_weight(start, finish) + find_min(start, curr_cell-1) + find_min(curr_cell+1, finish);
			if(temp_cost < costs[ obj ]) {
				costs[ obj ] = temp_cost;
				root[ obj ] = curr_cell;
			}
		}
	}

	return costs[obj];
};

template <class Value>
void O_BSTree<Value>::clear(Node* curr)
{
	if(!curr)
		return;

	if(curr->left)
		clear(curr->left);
	if(curr->right)
		clear(curr->right);
	
	delete curr;
};

template <class Value>
void O_BSTree<Value>::insert(Node* cell, Node*& curr)
{
	if(cell->dat.key >= curr->dat.key) {
		if(!curr->right)
			curr->right = cell;
		else
			insert(cell, curr->right);
	}
	else {
		if(!curr->left)
			curr->left = cell;
		else
			insert(cell, curr->left);
	}
};

template <class Value>
void O_BSTree<Value>::construct_tree(int i, int j) 
{
	int temp_root = root[i*amount + j];

	if(!tree_root)
		tree_root = add_Node( temp_root );
	else
		insert( add_Node(temp_root), tree_root);
	
	if(temp_root > i)
		construct_tree(i, temp_root - 1);
	if(temp_root < j)
		construct_tree(temp_root + 1, j);
};