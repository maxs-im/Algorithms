#include "Order-statistic Tree.h"
#include "..\\..\\Variant 2\\Library.h"
#include "Outputting binary tree.h"

void main()
{
	Work_Space lib;

	OSTree<Work_Space::Library> tr;
	for(int i(lib.Data.size()); i > 0; i--)
		tr.push(lib.Data[i-1].key, lib.Data[i-1]);
	std::cout<< "Before: "<< tr.count()<< "\n";
	tr.pop(166);		// not exist	
	std::cout<< tr.rank(169)<< std::endl;
	std::cout<< "After: "<< tr.count()<< "\n";

	Work_Space::Library* temp = tr.search(158);
	//if(temp == tr.nil)
		//std::cout<<"NETY\n";
	std::cout<< temp;
	tr.pop(169);
	OutPut<OSTree<Work_Space::Library>::OSnode> paint(tr.root, tr.nil);
	paint.draw_vertical();
	//paint.draw_horizontal();
	std::cout<< "\a";
	
	tr.clear();
	std::cin.get();
};