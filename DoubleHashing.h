#pragma once
#include "LinearHashing.h"
#include "Entry.h"
#include <vector>
#include "SecondHashing.h"

using namespace std;

template <typename K, typename V1, typename V2, typename H>
class DoubleHashing : public LinearHashing<K, V1, V2, H> 
{
public:
	typedef LinearHashing<K, V1, V2, H> LinearHashing;
public:
	DoubleHashing() : LinearHashing() {};
	DoubleHashing(int c) : LinearHashing(c) {};
	int secondHash(const int& k);
};

template <typename K, typename V1, typename V2, typename H>
int DoubleHashing<K, V1, V2, H>::secondHash(const int& k) // second hash
{
	int capa = this->getCapacity();
	capa--;
	for (int i = 2; i < capa; i++) // find the lower nearest prime number
	{
		if (capa % i == 0)
		{
			capa--;
			i = 2;
		}
	}
	int inc = capa - (k % capa);
	return inc;
}

