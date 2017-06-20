//Splay Tree without dublicate keys

#pragma once

template <class Value>
class SplayTree
{
public:
	struct Node {
		int key;
		Value data;
		Node *left,
				*right,
				*parent;

		Node(int _key = 0, Value &_data = 0)
		{
			key = _key;
			data = _data;
			right = nullptr;
			left = nullptr;
			parent = nullptr;
		};
	};
	Node* root;

	SplayTree() : root(nullptr) {};
	void push(int _key, Value &_data)
	{
		Node *newNode = new Node(_key, _data);
		Node **term = split(root, _key);
		if(root) {
			newNode->left = term[0];
			newNode->right = term[1];
			keep_parent(newNode);
		}
		root = newNode;
	};
	void pop(int _key)
	{
		if(!root)
			return;

		root = s_find(root, _key);
		if(root->key != _key)
			return;

		set_parent(root->left, nullptr);
		set_parent(root->right, nullptr);

		Node *answer[2] = {root->left, root->right};

		delete root;

		root = merge(answer[0], answer[1]);
	};
	Value* find(int _key)
	{
		root = s_find(root, _key);

		if(root->key == _key)
			return &root->data;
		return nullptr;
	};
	void clear()
	{
		free(root);
		root = nullptr;
	};
	int count()
	{
		return s_count(root);
	};
	Value* maximum()
	{
		Node *temp = root;
		if(!temp)
			return nullptr;
		while(temp->right)
			temp = temp->right;
		return &temp->data;
	};
	Value* minimum()
	{
		Node *temp = root;
		if(!temp)
			return nullptr;

		while(temp->left)
			temp = temp->left;
		return &temp->data;
	};
private:

	void rotate(Node* curr)
	{
		Node *parent = curr->parent,
			*grandparent = parent->parent;

		if(grandparent) {
			if(grandparent->left == parent)
				grandparent->left = curr;
			else
				grandparent->right = curr;
		}
		curr->parent = grandparent;

		if(parent->left == curr) {
			parent->left = curr->right;
			curr->right = parent;
		}
		else {
			parent->right = curr->left;
			curr->left = parent;
		}
		keep_parent(curr);
		keep_parent(parent);
	}
	Node** split(Node *input, int _key)
	{
		Node* answer[2] = {nullptr, nullptr};		// answer[0] -- left
													// answer[1] -- right
		if(!input)
			return answer;

		input = s_find(input, _key);

		if(input->key == _key) {
			set_parent(input->left, nullptr);
			set_parent(input->right, nullptr);

			answer[0] = input->left;
			answer[1] = input->right;
		}
		if(input->key > _key) {
			set_parent(input->left, nullptr);

			answer[0] = input->left;
			input->left = nullptr;
			answer[1] = input;
		}
		if(input->key < _key) {
			set_parent(input->right, nullptr);

			answer[1] = input->right;
			input->right = nullptr;
			answer[0] = input;
		}

		return answer;
	};
	void splay(Node *curr) 
	{
		if(!(curr->parent))
			return;
		Node *parent = curr->parent,
			*grandparent = parent->parent;


			//	ZIG
		if(!grandparent) {
			rotate(curr);
			root = curr;
			return;
		}

		if(grandparent == root)
			root = curr;

		bool zigzag;
		if(((grandparent->left == parent) && (parent->right == curr)) || 
			((grandparent->right == parent) && (parent->left == curr)))
			zigzag = true;

		if(zigzag) {
				// ZIG-ZAG
			rotate(curr);
			rotate(curr);
		}
		else {
			// ZIG-ZIG
			rotate(parent);
			rotate(curr);
		}
		return splay(curr);
	};
	Node* merge(Node *left, Node *right)
	{
		if(!left)
			return right;
		if(!right)
			return left;

		right = s_find(right, left->key);
		right->left = left;
		left->parent = right;

		return right;
	};
	Node* s_find(Node *curr, int _key)
	{
		if(!curr)
			return nullptr;

		if(_key > curr->key && curr->right)
			return s_find(curr->right, _key);
		if(_key < curr->key && curr->left)
			return s_find(curr->left, _key);

		splay(curr);

		return curr;
	};
	void free(Node *node)
	{
		if(node) {
			free(node->right);
			free(node->left);
			delete node;
		}
	};
	int s_count(Node *node) 
	{
		if(!node)
			return 0;
		return 1 + s_count(node->right) + s_count(node->left);
	};
	void keep_parent(Node *input) 
	{
		set_parent(input->left, input);
		set_parent(input->right, input);
	};
	void set_parent(Node *child, Node *parent)
	{
		if(child)
			child->parent = parent;
	};
};