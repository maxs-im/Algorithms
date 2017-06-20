#include "..\\..\\Variant 2\\Library.h"
#include "Fibonacci Heap.h"

#include <iostream>

int main()
{
	Work_Space lib;

	Fibonacci_Heap<Work_Space::Library> pyramid;
	for(int i = lib.Data.size()-1; i >= 0; i--)
		pyramid.insert(lib.Data[i].key, lib.Data[i]);

	std::cout<< "Count: "<< pyramid.count(true)<< std::endl;
	std::cout<< "Minimum: " << pyramid.minimum();
	//std::cout<< "Minimum extract: " << pyramid.extract_minimum()<< std::endl;
	std::cout<< "Count: "<< pyramid.count(true)<< std::endl;
	std::cout<< "41 ?" << pyramid.search(41)<< std::endl;
	pyramid.decrease(41, -5);
	std::cout<< "Count: "<< pyramid.count(true)<< std::endl;
	pyramid.print();
	std::cout<< "-5 ? "<< pyramid.search(-5)<< std::endl;
	std::cout<< "Minimum: "<< pyramid.minimum()<< std::endl;
	std::cout<< "Minimum extract: "<< pyramid.extract_minimum()<< std::endl;
	std::cout<< "Count: "<< pyramid.count(true)<< std::endl;
	pyramid.erase(334);
	std::cout<< "Count: "<< pyramid.count(true)<< std::endl;

	pyramid.print(true);


	std::cin.get();
	return 0;
}
