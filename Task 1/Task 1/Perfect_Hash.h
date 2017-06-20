#pragma once

#include <vector>
#include <string>

struct Hash 
{
	static int p;
	int a, b, m;
	Hash(int _a = 0, int _b = 0) : 
		a((_a%(p-2))+1), b(_b%(p-1)), m(0) {};
	int operator()(const int key) {
		return ((a*key + b) % p) % m;
	}
};
int Hash::p(0);

template <class Value>
class Perfect_Hash
{
public:
	std::vector< std::vector<Value> > T;
	std::vector<Hash> hash_fun;

	Perfect_Hash (std::vector<Value>& Data)
	{
		int size = Data.size();

		T.resize(size);
		hash_fun.resize(size + 1);
		
		Hash::p = find_prost( find_max(Data) );
		
		for(int i(0); i <= size; i++)
			hash_fun[i] = Hash( rand(), rand() );
		hash_fun[size].m = size;
		
		for(auto it : Data)  
			T[ hash_fun[size](it.key) ].push_back(it);
		
		for(int i(0); i < size; i++)
			Stage_second(T[i], i);

	};
	void Stage_second(std::vector<Value>& S, int index)
	{
		if(S.empty())
			return;

		int size = S.size();
		hash_fun[index].m = size*size;

		std::vector<Value> temp = S;
		
		S.clear();
		S.resize(size*size);

		for(auto it : temp)
			S[ hash_fun[index](it.key) ] = it;
	};

	int find_max(std::vector<Value> &input)
	{
		int max(0);
		for(auto it : input)
			if(it.key >= max)
				max = it.key + 1;
		return max;
	};
	int find_prost(int m)
	{
		if(is_prost(m))
			return m;
		return find_prost(m+1);
	};
	bool is_prost(int m) 
	{
		for(int i(2); i <= sqrt(m); i++)
			if(m % i == 0)
				return false;
		return true;
	};

	Value* operator[](const int key)
	{
		int first_stage = hash_fun[ T.size() ](key);

		if(T[first_stage].empty())
			return nullptr;

		Value* answer;
		answer = &T[ first_stage ][ hash_fun[first_stage](key) ];
		if(answer->key == 0)
			return nullptr;

		return answer;
	};
};