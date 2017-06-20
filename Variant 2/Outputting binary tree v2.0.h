/*
		This program draw only binary tree into the file PATH
-> The tree should not be changed while drawing!
-> Each node must contain "key" field!
-> Each node must contain "left" and "right" pointer to the sons!
*/

#pragma once

#include <iomanip>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>

#define PATH "output.txt"
#define FILL_EMPTY '#'
#define FILL_EMPTY_BETWEEN ' '
#define FILL_KEY '_'
#define END_SPACE 1


template <class Node>
class OutPut {
public:
	enum IOS { DEFAULT, APP };
	OutPut(Node* _root, Node* _nil = nullptr, IOS ios_type = DEFAULT, std::string _path = PATH);
	~OutPut();
	void set_root(Node* new_root);
	void draw_horizontal(unsigned short finish = USHRT_MAX, unsigned short start = 0);
	void draw_vertical(bool = false, unsigned short finish = USHRT_MAX, unsigned short start = 0);
private:
	Node *root;
	Node *nil;
	unsigned short tree_height;	


	unsigned short place_for_number;		
	long long width_margin;
	bool scale;
	
	std::ofstream fout;
	std::string path_to_file;
	IOS default_ios;

	unsigned short searchHeight;
	std::vector< std::pair<long long, int> > searchContainer;


	void dfs_initial(Node*, unsigned short = 0);
	void dfs_paint_horizontal(Node*, unsigned short = 0, const long long = 1);
	void dfs_paint_vertical(Node*, unsigned short, const unsigned short *);
	void paint_line();
};


template <class Node>
OutPut<Node>::OutPut(Node *_root, Node *_nil, IOS ios_type, std::string _path) : 
	root(_root), nil(_nil),
	place_for_number(0), tree_height(0),
	path_to_file(_path),
	default_ios(ios_type)
{
	if(root == nil || root == nullptr)
		throw "Empty tree";

	if(default_ios == DEFAULT) 
		fout.open(path_to_file);
	else 
		fout.open(path_to_file, std::ios_base::app);
	
	if(!fout.is_open())
		throw "Incorrect file";
	
	set_root(root);
}

template <class Node>
OutPut<Node>::~OutPut()
{
	fout.close();
}


template <class Node>
void OutPut<Node>::set_root(Node* new_root)
{
	this->root = new_root;

	dfs_initial(root);

	long long max_number_nodes_in_field = (long long)pow(2, tree_height);
	width_margin = max_number_nodes_in_field*place_for_number + (max_number_nodes_in_field + 1)*END_SPACE;
};
template <class Node>
void OutPut<Node>::dfs_initial(Node* curr, unsigned short height)
{
	if(curr == nullptr)
		throw "Incorrect node in the tree";

	// height
	if(height > tree_height)
		tree_height = height;
	// ------

	// place_for_number
	unsigned short temp = std::to_string(curr->key).length();
	if(temp > place_for_number)
		place_for_number = temp;
	// ----------------

	if(curr->left != nil)
		dfs_initial(curr->left, height+1);
	if(curr->right != nil)
		dfs_initial(curr->right, height+1);
}

template <class Node>
void OutPut<Node>::draw_horizontal(unsigned short finish, unsigned short start)
{
	if(finish > tree_height || start > finish)
		finish = tree_height;
	if(start > tree_height)
		start = tree_height;

	searchContainer.clear();
	for(searchHeight = start; searchHeight <= finish; searchHeight++)
		paint_line();
}

template <class Node>
void OutPut<Node>::draw_vertical(bool _scale, unsigned short finish, unsigned short start)
{
	scale = _scale;
	if(finish > tree_height || start > finish)
		finish = tree_height;
	if(start > tree_height)
		start = tree_height;


	unsigned short borders[2] = {finish, start};
	dfs_paint_vertical(root, 0, borders);
}

template <class Node>
void OutPut<Node>::dfs_paint_vertical(Node *curr,
							 unsigned short height, const unsigned short *border)
{
	if(height < border[0]) {
		if(curr->left != nil)
			dfs_paint_vertical(curr->left, height+1, border);
		else {
			if(scale)
				fout<< std::setfill('\n')<< std::setw(pow(2, (int)border[0] - height) - 1)<< "";
		}
	}

	if(height >= border[1]) {
		if(height != border[0])
			fout<< std::setfill(FILL_EMPTY_BETWEEN)<< std::setw((border[0] - height)*(place_for_number+1))<< "" << ">";
		else 
			fout<< " ";

		fout<< std::setfill(FILL_KEY)<< std::setw(place_for_number)<< curr->key;
		//fout<< std::setfill(FILL_EMPTY)<< std::setw((height - border[1])*(place_for_number + 1)) << "";
		fout<< std::endl;
	}

	if(height < border[0]) {
		if(curr->right != nil)
			dfs_paint_vertical(curr->right, height+1, border);
		else
			if(scale)
				fout<< std::setfill('\n')<< std::setw(pow(2, (int)border[0] - height) - 1)<< "";
	}
}

template <class Node>
void OutPut<Node>::dfs_paint_horizontal(Node *curr,
							 unsigned short height, const long long num) 
{
	if(height == searchHeight) {
		searchContainer.push_back( std::make_pair(num, curr->key) );
		return;
	}

	if(curr->left != nil)
		dfs_paint_horizontal(curr->left, height+1, 2*num);
	if(curr->right != nil)
		dfs_paint_horizontal(curr->right, height+1, 2*num + 1);
}

template <class Node>
void OutPut<Node>::paint_line()
{
	dfs_paint_horizontal(root);

	long long number_nodes_in_field = (long long)pow(2, searchHeight);
	long long space_between_nodes = (width_margin - number_nodes_in_field * place_for_number) / (number_nodes_in_field + 1);
	long long tail_space = (width_margin - number_nodes_in_field * place_for_number) % (number_nodes_in_field + 1);

	bool in_one = false;
	for(long long i = number_nodes_in_field; i < 2*number_nodes_in_field; i++) {
		
		bool btail = 0;
		if(tail_space > 0) {
			tail_space--;
			btail = 1;
		}

		if(in_one) {
			fout<< std::setfill(FILL_EMPTY_BETWEEN)<< std::setw(space_between_nodes + 1*btail)<< ""; 
			in_one = false;
		}
		else
			fout<< std::setfill(FILL_EMPTY)<< std::setw(space_between_nodes + 1*btail)<< "";

		if(!searchContainer.empty() && searchContainer.front().first == i) {
			if(searchContainer.front().first % 2 == 0)
				in_one = true;
			std::string temp = std::to_string(searchContainer.front().second);
			fout<< std::setfill(FILL_KEY)<< std::setw(place_for_number)<< temp;
			searchContainer.erase( searchContainer.begin() );
		}
		else
			fout<< std::setw(place_for_number)<< "";
	}

	fout<< std::setfill(FILL_EMPTY)<< std::setw(space_between_nodes)<< "";
	fout<< std::endl;
}
