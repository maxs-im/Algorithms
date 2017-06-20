#include "Binomial Heap.h"
#include "..\\..\\Variant 2\\Library.h"

#include <iostream>

int main()
{
	Work_Space lib;

	Binomial_Heap<Work_Space::Library> pyramid;
	for(int i = 9/*lib.Data.size()-1*/; i >= 0; i--)
		pyramid.insert(lib.Data[i].key, lib.Data[i]);

	std::cout<< "Count: "<< pyramid.count(true)<< std::endl;
	std::cout<< "Minimum: " << pyramid.minimum();
	std::cout<< "Minimum extract: " << pyramid.extract_minimum()<< std::endl;
	std::cout<< "Count: "<< pyramid.count(true)<< std::endl;
	std::cout<< "334 ?" << pyramid.search(334)<< std::endl;
	pyramid.decrease(334, -5);
	pyramid.print();
	std::cout<< "-5 ? "<< pyramid.search(-5)<< std::endl;
	std::cout<< "Minimum: "<< pyramid.minimum()<< std::endl;
	std::cout<< "Minimum extract: "<< pyramid.extract_minimum()<< std::endl;
	std::cout<< "Count: "<< pyramid.count(true)<< std::endl;
	//pyramid.erase(478);
	std::cout<< "Count: "<< pyramid.count(true)<< std::endl;
	std::cin.get();
	return 0;
}