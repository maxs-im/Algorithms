#include <iostream>
#include "Red-Black Tree.h"
#include "Outputting binary tree.h"

void main()
{
	RBTree tr;
	for(int i(rand()%200); i > 0; i--)
		tr.push(rand()%1000-500);
	std::cout<< "Before: "<< tr.count()<< "\n";
	tr.pop(-33);
	std::cout<< "After: "<< tr.count()<< "\n";

	OutPut<RBTree::RBnode> paint(tr.root, tr.nil, OutPut<RBTree::RBnode>::IOS::DEFAULT);
	paint.draw_horizontal();
	std::cout<< "\a";
	//paint.~OutPut();

	OutPut<RBTree::RBnode> paint1(tr.root, tr.nil, OutPut<RBTree::RBnode>::IOS::APP);
	paint1.draw_vertical();
	std::cout<< "\a";


	tr.clear();
	//tr.~RBTree();
	std::cin.get();
};