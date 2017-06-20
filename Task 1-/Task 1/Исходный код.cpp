#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#include "Perfect_Hash.h"

#define PATH_IN "Data.txt"
#define PATH_OUT "Data_out.txt"

std::ifstream fin(PATH_IN);
std::ofstream fout(PATH_OUT, std::ios_base::trunc);

struct Library {
	int key;
	std::string author;
	std::string name;
	Library() : key(0) {};
	friend std::ostream& operator<< (std::ostream &, const Library &);
};
std::ostream& operator<< (std::ostream& out, const Library* val)
{
	out<< val->key<< ") "<<val->author<< " - \""<< val->name<< "\"\n";
	return out;
};

std::vector<Library> Data;

Library read_book(std::string& str)
{
	Library book;
	int i = 0;
	while(str[i] != '\"')
		i++;
	
	i++;
	while(str[i] != '\"'){
		book.name += str[i];
		i++;
	}
	while(str[i] != '-')
		i++;
	i++;

	book.author = str.substr(i, str.length());

	int key;
	bool check;
	do {
		key = rand()%500 + 1;
		if(key == 45)
			int g =0;
		check = true;
		for(auto it : Data)
			if(it.key == key)
				check = false;
	} while(!check);
	book.key = key;

	return book;
};

void main()
{
	//srand(time(NULL));
	setlocale( LC_ALL,"Russian" ); 
	if(	!fin.is_open() ) {
		std::cout<< "NO FILE!\n";
		std::cin.get();
		return;
	}

	while(!fin.eof()) {
		std::string buff;
		getline(fin, buff);
		Data.push_back(read_book(buff));
	}
	fin.close();

	Perfect_Hash<Library> Table(Data);

	/*while(true) {
		int key;
		std::cin>> key;
		if( !key )
			break;
		std::cout<< Table[key];
	}*/
	int count_dif(0);
	for(int key_in(1); key_in <= 500; key_in++) {
		Library* temp = Table[key_in];
		if(temp != nullptr)
		{
			fout<< key_in << " -->\t" << temp;
			if(temp->key == key_in)
				count_dif++;
		}
	}
	fout.close();
	
	std::cout<< "\nCount difference: "<< count_dif;
	std::cin.get();
} // main
