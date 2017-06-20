#pragma once

#include <vector>

template <class Value>
class Persistent_BT {
public:
	Persistent_BT();
	~Persistent_BT();

	struct Node {
		int key;
		Value *data;
		Node *left, 
			*right;

		Node(int, Value *);
	};
	
	void insert(int key, Value *Data);
	void erase(int key);
	bool find(int key);
	Node* get_pointer(int _version = -1);
	void set_version(int tree_version = -1);

private:
	std::vector<Value> data_base;
	std::vector<Node*> versions; 
	std::vector<Node*> rubbish;
	int current_version;

	Node* PBT_insert(Node* curr, Node* input);
	Node* PBT_erase(Node* curr, int _key);
	bool PBT_find(Node* curr, int _key);
	Node* PBT_continue_erase(Node* curr, Node* tail);
};

template <class Value>
Persistent_BT<Value>::Persistent_BT()
{
	versions.clear();
	rubbish.clear();
	data_base.clear();

	Node *temp = nullptr;
	versions.push_back(temp);
	current_version = 0;
};
template <class Value>
Persistent_BT<Value>::~Persistent_BT()
{
	for(auto &it : rubbish)
		delete it;	
};

template <class Value>
Persistent_BT<Value>::Node::Node(int _key, Value* _data)
{
	left = nullptr;
	right = nullptr;
	key = _key;
	data = _data;
};

template <class Value>
void  Persistent_BT<Value>::insert(int _key, Value* _data)
{
	if(find(_key))
		throw "Such key has exist allready";

	data_base.push_back(*_data);
	Node* input = new Node(_key, &(data_base.back()));

	Node* new_tree = PBT_insert(versions[current_version], input);

	versions.push_back(new_tree);
	current_version++;
};
template <class Value>
typename Persistent_BT<Value>::Node* Persistent_BT<Value>::PBT_insert(Node* curr, Node* input) 
{
	if(!curr) {
		rubbish.push_back(input);
		return input;
	}

	Node* temp = new Node(curr->key, curr->data);
	
	if(input->key < curr->key) {
		temp->right = curr->right;
		temp->left = PBT_insert(curr->left, input);
	}
	else {
		temp->left = curr->left;
		temp->right = PBT_insert(curr->right, input);	
	}

	rubbish.push_back(temp);
	//??????????????????push before change left and right
	return temp;
};


template <class Value>
void  Persistent_BT<Value>::erase(int _key)
{
	if(!find(_key))
		throw "Miss element";

	Node* new_tree = PBT_erase(versions[current_version], _key);

	versions.push_back(new_tree);
	current_version++;
};
template <class Value>
typename Persistent_BT<Value>::Node* Persistent_BT<Value>::PBT_erase(Node* curr, int _key)
{
	if(!curr)
		return nullptr;

	if(curr->key == _key) {
		Node *left_tail = curr->left;
		if(!left_tail)
			return curr->right;
		return PBT_continue_erase(curr->right, left_tail);
	}
	
	Node* temp = new Node(curr->key, curr->data);
	
	if(_key < curr->key) {
		temp->right = curr->right;
		temp->left = PBT_erase(curr->left, _key);
	}
	else {
		temp->left = curr->left;
		temp->right = PBT_erase(curr->right, _key);	
	}

	rubbish.push_back(temp);
	//??????????????????push before change left and right
	return temp;
};
template <class Value>
typename Persistent_BT<Value>::Node* Persistent_BT<Value>::PBT_continue_erase(Node* curr, Node* tail)
{
	if(!curr)
		return tail;

	Node *temp = new Node(curr->key, curr->data);
	temp->right = curr->right;

	if(curr->left)
		temp->left = PBT_continue_erase(curr->left, tail);
	else
		temp->left = tail;
	
	rubbish.push_back(temp);
	//??????????????????push before change left and right
	return temp;
};

template <class Value>
bool Persistent_BT<Value>::find(int _key)
{
	return PBT_find(versions[current_version], _key);
};
template <class Value>
bool  Persistent_BT<Value>::PBT_find(Node* curr, int _key) 
{
	if(!curr)
		return false;
	
	if(curr->key == _key)
		return true;

	return PBT_find(curr->left, _key) || PBT_find(curr->right, _key);
};

template <class Value>
void Persistent_BT<Value>::set_version(int tree_version)
{
	if(tree_version == -1)
		tree_version =  versions.size()-1;
	else {
		if(tree_version < 0 || tree_version >= versions.size())
			throw "Incorrect version";
	}

	current_version = tree_version;
};
template <class Value>
typename Persistent_BT<Value>::Node* Persistent_BT<Value>::get_pointer(int tree_version)
{
	if(tree_version == -1)
		tree_version =  current_version;
	else {
		if(tree_version < 0 || tree_version >= versions.size())
			throw "Incorrect version";
	}

	return versions[tree_version];
};
