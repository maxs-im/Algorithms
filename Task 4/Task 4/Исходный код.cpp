#include "Optimal binary search Tree.h"
#include "..\\..\\Variant 2\\Library.h"
#include "Outputting binary tree.h"

#include <vector>

int main()
{
	Work_Space lib;

	std::vector<unsigned int> freq;
	/*for(int i = lib.Data.size(); i > 0 ; i--)
		freq.push_back(rand()%100 + 1);
*/
	std::vector<Work_Space::Library> ttt;
	for(int i =0; i < 4; i++)
		ttt.push_back(Work_Space::Library());
	ttt[0].key = 10;
	ttt[1].key = 12;
	ttt[2].key = 16;
	ttt[3].key = 21;
	freq.push_back(4);
	freq.push_back(2);
	freq.push_back(6);
	freq.push_back(3);

	O_BSTree<Work_Space::Library> tree(/*ttt*/lib.Data, freq);

	std::cout<< "Number elements is: "<< tree.count()<< std::endl;
	std::cout<< "Cost is: "<< tree.get_cost()<< std::endl;

	/*tree.generate(3, 1);
	std::cout<< tree.get_cost()<< std::endl;*/

	OutPut< O_BSTree<Work_Space::Library>::Node > paint( tree.get_pointer() );
	paint.draw_vertical();	
	std::cout<< "\a";

	std::cin.get();
	return 0;
}