
#include <iostream>

#include "..\\..\\Variant 2\\Library.h"
#include "Persistent Binary Tree.h"
#include "Outputting binary tree.h"

int main()
{
	Work_Space lib;

	Persistent_BT<Work_Space::Library> persistent_set;
	int size = lib.Data.size();
	persistent_set.insert(lib.Data[0].key, &(lib.Data[0]));

	OutPut< Persistent_BT<Work_Space::Library>::Node > paint( persistent_set.get_pointer(), nullptr, 
			OutPut< Persistent_BT<Work_Space::Library>::Node >::IOS::APP);

	for(int i = 1; i < 10; i++) {
		//persistent_set.set_version(0);
		persistent_set.insert(lib.Data[i].key, &(lib.Data[i]));

		//paint.set_root( persistent_set.get_pointer(i) );
		//paint.draw_vertical();	
		//paint.add_separator();
		//std::cout<< "\a";
	}
	
	//paint.set_root( persistent_set.get_pointer() );
	///std::cout<< persistent_set.find(169)<< std::endl;
	persistent_set.erase(169);
	//paint.set_root( persistent_set.get_pointer() );
	//persistent_set.set_version(10);
	paint.set_root( persistent_set.get_pointer() );
	//paint.add_sepaator();
	paint.draw_vertical();	
	std::cout<< "\a";
	
	std::cin.get();
	return 0;
}