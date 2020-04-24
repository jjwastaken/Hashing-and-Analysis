#pragma once
#include "ChainHashing.h"
#include "Entry.h"
#include <vector>
#include "DoubleHashing.h"
#include "SecondHashing.h"

using namespace std;

template <typename K, typename V1, typename V2, typename H>
class LinearHashing : public SecondHashing
{
public:
	typedef Entry<K, V1, V2> Entry;
private:
	int capacity;
	int n;
	vector<Entry> myV;
	int totalProbe;
	int probe;
public:
	LinearHashing() { capacity = 100; n = 0; totalProbe = 0; probe = 0; }
	LinearHashing(int c) 
	{ 
		capacity = c;
		for (int i = 2; i < capacity; i++)
		{
			if (capacity % i == 0)
			{
				capacity++;
				i = 2;
			}
		}
		Entry e(-1, -1, "-1");
		for (int i = 0; i < capacity; i++)
		{
			myV.push_back(e);
		}
		n = 0; 
	}
	void put(const K& k, const V1& v1, const V2& v2);
	int search(const K& k);
	int secondHash(const int& k);
	void find(const K& k);
	void print();
	void erase(const K& k);
	int getCapacity() { return capacity; }
	int getTotalProbe() { return totalProbe; }
	int getn() { return n; }
	int getProbe() { return probe; }
	void setProbe(int p) { probe = p; }
	void computeCluster();
 };

template <typename K, typename V1, typename V2, typename H>
int LinearHashing<K, V1, V2, H>::search(const K& k)  // helper function that finds the position where it should be
{
	int pos = k % capacity;
	return pos;
}

template <typename K, typename V1, typename V2, typename H>
int LinearHashing<K, V1, V2, H>::secondHash(const int& k)  // second hash just returning 1 for increment
{
	return 1;
}


template <typename K, typename V1, typename V2, typename H>
void LinearHashing<K, V1, V2, H>::put(const K& k, const V1& v1, const V2& v2)
{
	if (n == myV.size())
	{
		cout << "Cannot insert, the vector is full!" << endl;
		return;
	}
	int pos = search(k);
	if (myV[pos].key() == -1) // if empty
	{
		myV[pos].setKey(k);
		myV[pos].setValue1(v1);
		myV[pos].setValue2(v2);
		n++;
		totalProbe++;
		probe++;
	}
	else // if not empty, linear probe
	{
		while (myV[pos].key() != -1) // if it is not empty
		{
			pos += secondHash(k);
			totalProbe++;
			probe++;
			if (pos > myV.size()-1) // if at the end of vector
			{
				pos -= myV.size(); // start from the beginning
			}
		}
		totalProbe++;
		probe++;
		myV[pos].setKey(k);
		myV[pos].setValue1(v1);
		myV[pos].setValue2(v2);
		n++;
	}
}

template <typename K, typename V1, typename V2, typename H>
void LinearHashing<K, V1, V2, H>::find(const K& k)
{
	int pos = search(k);
	if (myV[pos].key() == k) // if found
	{
		cout << "Record: <" << myV[pos].key() << ", " << myV[pos].value1() << ", " << myV[pos].value2() << "> is found." << endl;
		return;
	}
	else // if not found or other places
	{
		int check = pos;
		pos += secondHash(k);
		probe++;
		if (pos > myV.size() - 1) // if at the end of vector
		{
			pos -= myV.size(); // start from the beginning
		}
		while (myV[pos].key() != k && pos != check) // if not found or looped the entire vector 
		{
			pos += secondHash(k);
			probe++;
			if (pos > myV.size() - 1) // if at the end of vector
			{
				pos -= myV.size(); // start from the beginning
			}
		}
		if (pos == check) // if not found
		{
			cout << "Cannot find the record!" << endl;
			probe--;
		}
		else
			cout << "Record: <" << myV[pos].key() << ", " << myV[pos].value1() << ", " << myV[pos].value2() << "> is found." << endl;
	}
}

template <typename K, typename V1, typename V2, typename H>
void LinearHashing<K, V1, V2, H>::print()
{
	for (int i = 0; i < capacity; i++)
	{
		cout << "<" << myV[i].key() << ", " << myV[i].value1() << ", " << myV[i].value2() << ">" << endl;
	}
}

template <typename K, typename V1, typename V2, typename H>
void LinearHashing<K, V1, V2, H>::erase(const K& k)
{
	int pos = search(k);
	if (myV[pos].key() == k) // if found
	{
		cout << "Record: <" << myV[pos].key() << ", " << myV[pos].value1() << ", " << myV[pos].value2() << "> is deleted." << endl;
		myV[pos].setKey(-1);
		myV[pos].setValue1(-1);
		myV[pos].setValue2("-1");
		n--;
		return;
	}
	else // if not found or other places
	{
		int check = pos;
		pos += secondHash(k);
		probe++;
		if (pos > myV.size() - 1) // if at the end of vector
		{
			pos -= myV.size(); // start from the beginning
		}
		while (myV[pos].key() != k && pos != check) // if not found or looped the entire vector 
		{
			pos += secondHash(k);
			probe++;
			if (pos > myV.size() - 1) // if at the end of vector
			{
				pos -= myV.size(); // start from the beginning
			}
		}
		if (pos == check) // if not found
		{
			cout << "Record does not exist, cannot be erased!" << endl;
			probe--;
			return;
		}
		else
		{
			cout << "Record: <" << myV[pos].key() << ", " << myV[pos].value1() << ", " << myV[pos].value2() << "> is deleted." << endl;
			myV[pos].setKey(-1);
			myV[pos].setValue1(-1);
			myV[pos].setValue2("-1");
			n--;
			return;
		}
	}
}

template <typename K, typename V1, typename V2, typename H>
void LinearHashing<K, V1, V2, H>::computeCluster()
{
	int numOfCluster = 0;
	int maxCluster = 0;
	int round = 1;
	int totalCluster = 0;
	for (int i = 0; i < capacity; i++)
	{
		if (i >= capacity)
		{
			break;
		}
		if (myV[i].key() != -1)
		{
			int checkCluster = 0;
			if (i + 1 < capacity)
			{
				if (myV[i + 1].key() != -1)
				{
					numOfCluster++;
					checkCluster++;
				}
			}
			else
			{
				break;
			}
			i++;
			if (i >= capacity)
			{
				break;
			}
			while (myV[i].key() != -1)
			{
				i++;
				checkCluster++;
				if (i >= capacity)
				{
					break;
				}
			}
			totalCluster += checkCluster;
			if (checkCluster > maxCluster)
			{
				maxCluster = checkCluster;
			}
		}
	}
	if (myV[0].key() != -1 && myV[capacity - 1].key() != -1)	// WRAP AROUND
	{
		numOfCluster--;
		int check = 0;
		int u = 0;
		while (myV[u].key() != -1)
		{
			check++;
			u++;
		}
		int d = capacity - 1;
		while (myV[d].key() != -1)
		{
			check++;
			d--;
		}
		if (check > maxCluster)
		{
			maxCluster = check;
		}
	}
	cout << "The Number of Clusters is: " << numOfCluster << endl;
	cout << "The Largest Cluster is: " << maxCluster << endl;
	cout << "The Average Cluster Size is: " << float(totalCluster) / numOfCluster << endl;
}