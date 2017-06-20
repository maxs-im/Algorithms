#include "Perfect_Hash.h"
#include "..\\..\\Variant 2\\Library.h"

void main()
{
	Work_Space lib;

	Perfect_Hash<Work_Space::Library> Table(lib.Data);

	int keys_works(0);
	int count_keys(0);
	for(auto it : lib.Data) {
		int i = it.key;
		Work_Space::Library* temp = Table[i];
		if(temp != nullptr) {
			count_keys++;
			fout<< i << " -->\t" << temp;
			if(temp->key == i)
				keys_works++;
		}
	}

	std::vector< std::vector<Work_Space::Library> > temp_vec = Table.T;
	int _size = temp_vec.size();
	for(int i = 0; i < _size; i++, std::cout<< std::endl) {
		if(temp_vec.empty()) {
			std::cout<< "----";
			continue;
		}
		int size0 = temp_vec[i].size();
		for(int j = 0; j < size0; j++)
			if(temp_vec[i][j].key == 0)
				std::cout<< " - ";
			else 
				std::cout<< temp_vec[i][j].key << " ";
	}


	std::cout<< "\n\nKeys works: "<<  keys_works;
	//std::cout<< "\nCount keys: "<< count_keys;
	std::cin.get();

} // main