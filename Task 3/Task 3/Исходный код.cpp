#include "Splay_Tree.h"
#include "..\\..\\Variant 2\\Library.h"
#include "Outputting binary tree.h"

void main()
{
	Work_Space lib;

	SplayTree<Work_Space::Library> tr;

	for(int i(10/*lib.Data.size()*/); i > 0; i--)
		tr.push(lib.Data[i-1].key, lib.Data[i-1]);
	std::cout<< "Before: "<< tr.count()<< "\n";
	tr.find(47);
	tr.pop(166);		// not exist
	tr.find(169);
	//tr.pop(169);		
	std::cout<< "After: "<< tr.count()<< "\n";
	std::cout<< "Maximum: "<< tr.maximum()<< "\n";
	std::cout<< "Minimum: "<< tr.minimum()<< "\n";
	std::cout<< "Element: "<< tr.find(169); 
	//tr.pop(169);	
	OutPut< SplayTree<Work_Space::Library>::Node > paint(tr.root);
	paint.draw_vertical();	
	std::cout<< "\a";
	

	tr.clear();
	std::cout<< "After: "<< tr.count()<< "\n";
	std::cout<< "\nASK ABOUT WARNINGS!"<< "\n";
	std::cin.get();
}