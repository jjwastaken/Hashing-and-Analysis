#pragma once

#include <vector>
#include <list>
#include "Entry.h"
#include <regex>

using namespace std;

template <typename K, typename V1, typename V2, typename H>
class HashMap {
public:						// public types
	typedef Entry<const K, V1, V2> Entry;			// a (key,value) pair
	class Iterator;					// a iterator/position
public:						// public functions
	HashMap(int c = 100);			// constructor
	int size() const;					// number of entries
	bool empty() const;					// is the map empty?
	Iterator find(const K& k);				// find entry with key k
	Iterator put(const K& k, const V1& v1, const V2& v2);		// insert/replace (k,v1)
	void print();
	void print1(int& key);
	int counting();
	int getCollisions();
	int getCapacity() { return capacity; }
	int getProbe() { return probe; }
	int getTotalProbe() { return totalProbe; }
	void setProbe(int p) { probe = p; }
	void computeCluster();		// compute all the info about cluster (extra credit 2)
	void erase(const K& k);				// remove entry with key k
	void erase(const Iterator& p);			// erase entry at p
	Iterator begin();					// iterator to first entry
	Iterator end();					// iterator to end entry
protected:						// protected types
	typedef std::list<Entry> Bucket;			// a bucket of entries
	typedef std::vector<Bucket> BktArray;		// a bucket array
									// ...insert HashMap utilities here
	Iterator finder(const K& k);			    // find utility
	Iterator inserter(const Iterator& p, const Entry& e);   // insert utility
	void eraser(const Iterator& p);			    // remove utility
	typedef typename BktArray::iterator BItor;		    // bucket iterator
	typedef typename Bucket::iterator EItor;		    // entry iterator
	static void nextEntry(Iterator& p)			    // bucket's next entry
	{
		++p.ent;
	}
	static bool endOfBkt(const Iterator& p)		    // end of bucket?
	{
		return p.ent == p.bkt->end();
	}
private:
	int n;						// number of entries
	int collisions;
	int totalProbe;
	int probe;
	int capacity;
	H hash;						// the hash comparator
	BktArray B;						// bucket array
public:						// public types
  // ...insert Iterator class declaration here
	class Iterator {	                      		// an iterator (& position)
	private:
		EItor ent;					// which entry
		BItor bkt;					// which bucket
		const BktArray* ba;				// which bucket array
	public:
		Iterator(const BktArray& a, const BItor& b, const EItor& q = EItor())
			: ent(q), bkt(b), ba(&a) { }
		Entry& operator*() const;				// get entry
		bool operator==(const Iterator& p) const;		// are iterators equal?
		bool operator!=(const Iterator& p) const;
		Iterator& operator++();				// advance to next entry
		friend class HashMap;				// give HashMap access
	};
};

template <typename K, typename V1, typename V2, typename H>		// constructor
HashMap<K, V1, V2, H>::HashMap(int c) : n(0) , B(c)
{ 
	collisions = 0; 
	probe = 0; 
	totalProbe = 1; 
	capacity = c;
}

template <typename K, typename V1, typename V2, typename H>	// number of entries
int HashMap<K, V1, V2, H>::size() const { return n; }

template <typename K, typename V1, typename V2, typename H>	// is the map empty?
bool HashMap<K, V1, V2, H>::empty() const { return size() == 0; }

template <typename K, typename V1, typename V2, typename H>	// remove utility
int HashMap<K, V1, V2, H>::getCollisions()
{
	return collisions;
}

template <typename K, typename V1, typename V2, typename H>		// find utility
typename HashMap<K, V1, V2, H>::Iterator HashMap<K, V1, V2, H>::finder(const K& k) {
	int i = hash(k) % B.size();				// get hash index i
	BItor bkt = B.begin() + i;				// the ith bucket
	Iterator p(B, bkt, bkt->begin());			// start of ith bucket
	totalProbe++;
	probe++;
	while (!endOfBkt(p) && (*p).key() != k)		// search for k
	{
		nextEntry(p);
		probe++;
		totalProbe++;
	}
	return p;						// return final position
}

template <typename K, typename V1, typename V2, typename H>		// find key
typename HashMap<K, V1, V2, H>::Iterator HashMap<K, V1, V2, H>::find(const K& k) {
	Iterator p = finder(k);				// look for k
	if (endOfBkt(p))					// didn't find it?
		return end();					// return end iterator
	else
		return p;						// return its position
}

template <typename K, typename V1, typename V2, typename H>		// remove utility
void HashMap<K, V1, V2, H>::eraser(const Iterator& p) {
	p.bkt->erase(p.ent);				// remove entry from bucket
	n--;						// one fewer entry
}

template <typename K, typename V1, typename V2, typename H>		// remove entry at p
void HashMap<K, V1, V2, H>::erase(const Iterator& p)
{
	eraser(p);
}

template <typename K, typename V1, typename V2, typename H>		// remove entry with key k
void HashMap<K, V1, V2, H>::erase(const K& k) {
	Iterator p = finder(k);				// find k
	if (endOfBkt(p))					// not found?
	{
		std::cout << "Record does not exist, cannot be erased!" << std::endl;
		return;
	}
	else
	{
		std::cout << "Record " << k << " has been erased." << std::endl;
	}
	//throw NonexistentElement("Erase of nonexistent");	// ...error
	eraser(p);						// remove it
}

template <typename K, typename V1, typename V2, typename H>		// iterator to end
typename HashMap<K, V1, V2, H>::Iterator HashMap<K, V1, V2, H>::end()
{
	return Iterator(B, B.end());
}

template <typename K, typename V1, typename V2, typename H>  	// iterator to front
typename HashMap<K, V1, V2, H>::Iterator HashMap<K, V1, V2, H>::begin() {
	if (empty()) return end();				// emtpty - return end
	BItor bkt = B.begin();				// else search for an entry
	while (bkt->empty()) ++bkt;				// find nonempty bucket
	return Iterator(B, bkt, bkt->begin());		// return first of bucket
}

template <typename K, typename V1, typename V2, typename H> 	// are iterators equal?
bool HashMap<K, V1, V2, H>::Iterator::operator==(const Iterator& p) const {
	if (ba != p.ba || bkt != p.bkt) return false;	// ba or bkt differ?
	else if (bkt == ba->end()) return true;		// both at the end?
	else return (ent == p.ent);				// else use entry to decide
}

template <typename K, typename V1, typename V2, typename H>		// are iterators equal?
bool HashMap<K, V1, V2, H>::Iterator::operator!=(const Iterator& p) const {
	if (ba != p.ba || bkt != p.bkt) return true;	// ba or bkt differ?
	else if (bkt == ba->end()) return false;		// both at the end?
	else return (ent != p.ent);				// else use entry to decide
}

template <typename K, typename V1, typename V2, typename H>		// advance to next entry
typename HashMap<K, V1, V2, H>::Iterator& HashMap<K, V1, V2, H>::Iterator::operator++() {
	++ent;						// next entry in bucket
	if (endOfBkt(*this)) {				// at end of bucket?
		++bkt;						// go to next bucket
		while (bkt != ba->end() && bkt->empty())		// find nonempty bucket
			++bkt;
		if (bkt == ba->end()) return *this;		// end of bucket array?
		ent = bkt->begin();				// first nonempty entry
	}
	return *this;					// return self
}

template <typename K, typename V1, typename V2, typename H>		// get entry
typename HashMap<K, V1, V2, H>::Entry&
HashMap<K, V1, V2, H>::Iterator::operator*() const
{
	return *ent;
}

template <typename K, typename V1, typename V2, typename H>		// insert utility
typename HashMap<K, V1, V2, H>::Iterator HashMap<K, V1, V2, H>::inserter(const Iterator& p, const Entry& e) {
	EItor ins = p.bkt->insert(p.ent, e);		// insert before p
	n++;						// one more entry
	return Iterator(B, p.bkt, ins);			// return this position
}

template <typename K, typename V1, typename V2, typename H>		// insert/replace (v1,k)
typename HashMap<K, V1, V2, H>::Iterator HashMap<K, V1, V2, H>::put(const K& k, const V1& v1, const V2& v2) {
	Iterator p = finder(k);				// search for k

	if (endOfBkt(p)) {					// k not found?
		return inserter(p, Entry(k, v1, v2));			// insert at end of bucket
	}
	else {						// found it?
		if (p.ent->value1() != v1 && p.ent->value2() != v2)
		{
			Iterator q = inserter(p, Entry(k, v1, v2));
			collisions++;
			probe++;
			totalProbe++;
			return q;
		}
		else
		{
			p.ent->setValue1(v1);				// replace value with v1
			p.ent->setValue2(v2);
			return p;						// return this position
		}
	}
}

template <typename K, typename V1, typename V2, typename H>
void HashMap<K, V1, V2, H>::print()
{
	Iterator it = this->begin();
	for (it = this->begin(); it != this->end(); ++it)
	{
		std::cout << "<" << it.ent->key() << ", " << it.ent->value1() << ", " << it.ent->value2() << ">" << std::endl;
	}
}

template <typename K, typename V1, typename V2, typename H>
void HashMap<K, V1, V2, H>::print1(int& key)
{
	Iterator it = this->find(key);

	if (it != this->end())
	{
		std::cout << "Record: <" << it.ent->key() << ", " << it.ent->value1() << ", " << it.ent->value2() << ">" << std::endl;
	}
	else
	{
		std::cout << "Cannot find the record!" << std::endl;
	}

}

template <typename K, typename V1, typename V2, typename H>
void HashMap<K, V1, V2, H>::computeCluster()
{
	int numOfCluster = 0;
	int maxCluster = 0;
	vector<int> myCluster;
	int j = 0;
	for (int i = 0; i < B.size(); i++)
	{
		if (i >= B.size())
		{
			break;
		}
		int checkCluster = 0;
		if (B[i].size() > 1)
		{
			numOfCluster++;
			checkCluster = B[i].size();
			i++;
			if (i >= B.size())
			{
				break;
			}
		}
		myCluster.push_back(checkCluster);
		if (maxCluster < checkCluster)
		{
			maxCluster = checkCluster;
		}
		i++;
	}
	float total = 0.0;
	for (int j = 0; j < myCluster.size(); j++)
	{
		total += myCluster[j];
	}
	float avg;
	if (numOfCluster == 0)
	{
		avg = 0;
	}
	else
	{
		avg = total / numOfCluster;
	}
	std::cout << "The Number of Clusters is: " << numOfCluster << std::endl;
	std::cout << "The Largest Cluster is: " << maxCluster << std::endl;
	std::cout << "The Average Cluster is: " << avg << std::endl;
}

template <typename K, typename V1, typename V2, typename H>
int HashMap<K, V1, V2, H>::counting()
{
	Iterator it = this->begin();
	int num = 0;
	for (it = this->begin(); it != this->end(); ++it)
	{
		num++;
	}
	return num;
}
