// input
//						#include "..\\..\\Variant 2\\Library.h"
// to connect

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#define PATH_IN "F:\\Solutions\\Labs Algorithms\\Variant 2\\Data.txt"
#define PATH_OUT "Data_out.txt"

std::ifstream fin(PATH_IN);
std::ofstream fout(PATH_OUT, std::ios_base::trunc);

class Work_Space
{
public:
	struct Library 
	{
		int key;
		std::string author;
		std::string name;
		Library() : key(0) {};

		friend std::ostream& operator<< (std::ostream &, const Library *);
	};

	std::vector<Library> Data;
	bool rand_time;

	Work_Space(bool _rand_time = false) : rand_time(_rand_time)
	{
		read();
	};
	~Work_Space()
	{
		fout.close();
	};

private:
	void read()
	{
		if(rand_time)
			srand((unsigned int)time(nullptr));

		setlocale( LC_ALL,"Russian" ); 
		if(	!fin.is_open() ) {
			std::cout<< "NO FILE!\n";
			std::cin.get();
			//this->~Work_Space();
			return;
		}

		while(!fin.eof()) {
			std::string buff;
			getline(fin, buff);
			Data.push_back(read_book(buff));
		}
		fin.close();
	};
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
		bool check;
		int key(0);
		do {
			key = rand()%500;		
			check = false;
			for(auto it : Data)
				if(key == it.key) {
					check = true;
					break;
				}
		} while(check);
		book.key = key;
	
		return book;
	};
};

std::ostream& operator<< (std::ostream& out, const Work_Space::Library* val)
{
	out<< val->key<< " = "<<val->author<< " - \""<< val->name<< "\"\n";
	return out;
};