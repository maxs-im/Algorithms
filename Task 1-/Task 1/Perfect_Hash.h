#pragma once

#include <vector>
#include <string>

template <class Value>
class Perfect_Hash
{
public:
	std::vector< std::vector<Value> > T;
	std::vector<int> hash_fun_seeds;
	
	Perfect_Hash (std::vector<Value> Data)
	{
		int size = Data.size();

		T.resize(size);
		hash_fun_seeds.resize(size + 1);

		for(auto &it : hash_fun_seeds)
			it = rand();

		for(auto it : Data)
			T[ MurmurHash2(it.key, hash_fun_seeds[size]) % size ].push_back(it);
		
		for(int i(0); i < size; i++)
			Stage_second(T[i], i);

	};
	void Stage_second(std::vector<Value>& S, int index)
	{
		if(S.empty())
			return;

		int size = S.size();

		std::vector<Value> temp = S;
		S.clear();
		S.resize(size*size);

		for(auto it : temp)/* {
			int u = MurmurHash2( it.key, hash_fun_seeds[index] ) % (size*size);
			if(S[ u ].key != 0)
				std::cout<< "HEY";
			*/
			S[ MurmurHash2( it.key, hash_fun_seeds[index] ) % (size*size) ] = it;
		//}

	};

	unsigned int MurmurHash2 ( int _key, unsigned int seed )
	{

		const void * key = std::to_string(_key).c_str();
		int len = sizeof(key);

		const unsigned int m = 0x5bd1e995;
		const int r = 24;
	
		unsigned int h = seed ^ len;

		const unsigned char * data = (const unsigned char *)key;

		while(len >= 4)
		{
			unsigned int k = *(unsigned int *)data;

			k *= m; 
			k ^= k >> r; 
			k *= m; 
		
			h *= m; 
			h ^= k;
				
			data += 4;
			len -= 4;
		}
	
		switch(len)
		{
		case 3: h ^= data[2] << 16;
		case 2: h ^= data[1] << 8;
		case 1: h ^= data[0];
			    h *= m;
		};

		h ^= h >> 13;
		h *= m;
		h ^= h >> 15;

		return h;
	};

	int find_max()
	{
		int max(0);
		for(auto it : Data)
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
		for(int i(2); i < sqrt(m); i++)
			if(m % i == 0)
				return false;
		return true;
	};

	Value* operator[](const int key)
	{
		int first_stage = MurmurHash2( key, hash_fun_seeds[T.size()] ) % T.size();
		if(T[first_stage].empty())
			return nullptr;

		int size = T[first_stage].size();
		Value* answer;
		answer = &T[first_stage][ MurmurHash2(key, hash_fun_seeds[ first_stage ]) % size ];
		if(answer->key == 0)
			return nullptr;

		return answer;
	};	
};