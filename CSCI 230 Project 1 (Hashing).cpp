#include <iostream>
#include "ChainHashing.h"
#include <string>
#include "HashComparator.h"
#include "DoubleHashing.h"
#include "LinearHashing.h"
#include <fstream>

using namespace std;

void printOption()
{
	cout << "What would you like to do?" << endl;
	cout << "\t1. Search for a record" << endl;
	cout << "\t2. Insert a record" << endl;
	cout << "\t3. Delete a record" << endl;
	cout << "\t4. List all records" << endl;
	cout << "\t5. Exit" << endl << endl;
	cout << "Option--> ";
}

void intro()
{
	cout << "Please choose a hashing scheme: " << endl;
	cout << "\t1. Chain Hashing" << endl;
	cout << "\t2. Linear Probe Hashing" << endl;
	cout << "\t3. Double Hashing" << endl;
	cout << "Option--> ";
}

void sliceLine(const string &oneline, int &code, int &pop, string &name)
{
	char delimiter = ',';
	int codeEnd = oneline.find(delimiter);
	string codestr = oneline.substr(0, codeEnd);
	code = stoi(codestr.c_str());
	int popEnd = oneline.find(delimiter, codeEnd + 1);
	string popstr = oneline.substr(codeEnd + 1, popEnd);
	pop = stoi(popstr.c_str());
	name = oneline.substr(popEnd + 1);
}

int computePrime(int c)
{
	int capacity = c;
	for (int i = 2; i < capacity; i++)
	{
		if (capacity % i == 0)
		{
			capacity++;
			i = 2;
		}
	}
	return capacity;
}


int main()
{
	cout << "Welcome to my HashMap Program" << endl;
	string filename = "C:\\Users\\joeje\\Desktop\\p1small.txt";
	int choice = 0;
	float loadFactor;
	cout << "Please enter the Load Factor -> ";
	cin >> loadFactor;

	cout << "Please enter the file to hash -> ";
	cin >> filename;
	ifstream infile;
	infile.open(filename);
	if (!infile)
	{
		cout << "Cannot find file, please try again!" << endl;
		return 0;
	}
	intro();
	cin >> choice;

	int numOfValues;
	infile >> numOfValues;
	infile.ignore(); // ignore the enter key
	HashMap<int, int, string, SeparateHashing> chainHashing(computePrime(int(numOfValues / loadFactor)));
	LinearHashing<int, int, string, SeparateHashing> linearHashing(int(numOfValues/loadFactor));
	DoubleHashing<int, int, string, SeparateHashing> doubleHashing(int(numOfValues/loadFactor));

	switch (choice)
	{
	case 1: // Chain Hashing
	{
		string oneLine;
		int code;
		int pop;
		string countyState;
		while (getline(infile, oneLine))
		{
			sliceLine(oneLine, code, pop, countyState);
			chainHashing.put(code, pop, countyState);
		}
		float probes = chainHashing.getTotalProbe();
		int records = chainHashing.size();
		cout << "Table size: " << chainHashing.getCapacity() << endl;
		cout << "Average number of probes: " << probes / records << endl;
		cout << "Maximum number of probes for the worst case: " << records << endl<< endl;
		cout << "Extra Credit:" << endl;
		chainHashing.computeCluster();
		cout << endl;
		chainHashing.setProbe(0);
		break;
	}
	case 2: // Linear Probe Hashing
	{
		string oneLine;
		int code;
		int pop;
		string countyState;
		int count = 0;
		while (getline(infile, oneLine))
		{
			sliceLine(oneLine, code, pop, countyState);
			linearHashing.put(code, pop, countyState);
			count++;
		}
		float probes = linearHashing.getTotalProbe();
		int records = linearHashing.getn();
		cout << "Table size: " << linearHashing.getCapacity() << endl;
		cout << "Average number of probes: " << probes / records << endl;
		cout << "Maximum number of probes for the worst case: " << records << endl << endl;
		cout << "Extra Credit:" << endl;
		cout << endl;
		linearHashing.computeCluster();
		linearHashing.setProbe(0);
		break;
	}
	case 3: // Double Hashing
	{
		string oneLine;
		int code;
		int pop;
		string countyState;
		while (getline(infile, oneLine))
		{
			sliceLine(oneLine, code, pop, countyState);
			doubleHashing.put(code, pop, countyState);
		}
		float probes = doubleHashing.getTotalProbe();
		int records = doubleHashing.getn();
		cout << "Table size: " << doubleHashing.getCapacity() << endl;
		cout << "Average number of probes: " << probes / records << endl;
		cout << "Maximum number of probes for the worst case: " << records << endl << endl;
		cout << "Extra Credit:" << endl;
		doubleHashing.computeCluster();
		cout << endl;
		doubleHashing.setProbe(0);
		break;
	}
	default:
	{
		cout << "Please enter a valid option" << endl;
		return 0;
	}
	}
	
	int choice2;
	do
	{
		cout << endl;
		printOption();
		cin >> choice2;
	
		switch (choice2)
		{
		case 1:		// Search
		{
			if (choice == 1) // chainHashing
			{
				int search;
				cout << "Record to search for--> ";
				cin >> search;
				chainHashing.print1(search);
				cout << "Number of Probe(s): " << chainHashing.getProbe() << endl;
				chainHashing.setProbe(0);
				break;
			}
			else if (choice == 2) // linearHashing
			{  
				int search;
				cout << "Record to search for--> ";
				cin >> search;
				linearHashing.find(search);
				cout << "Number of Probe(s): " << linearHashing.getProbe()+1 << endl;
				linearHashing.setProbe(0);
				break;
			}
			else if (choice == 3) // doubleHashing
			{
				int search;
				cout << "Record to search for--> ";
				cin >> search;
				doubleHashing.find(search);
				cout << "Number of Probe(s): " << doubleHashing.getProbe()+1 << endl;
				doubleHashing.setProbe(0);
				break;
			}
		}
		case 2:		// Insert
		{
			if (choice == 1) // chainHashing
			{
				int code;
				int population;
				string county;
				cout << "Record to insert(code population county/state)--> ";
				cin >> code >> population;
				getline(cin, county);
				chainHashing.put(code, population, county);
				cout << "Number of Probe(s): " << chainHashing.getProbe() << endl;
				chainHashing.setProbe(0);
				break;
			}
			else if (choice == 2) // linearHashing
			{
				int code;
				int population;
				string county;
				cout << "Record to insert(code population county/state)--> ";
				cin >> code >> population;
				getline(cin, county);
				linearHashing.put(code, population, county);
				cout << "Number of Probe(s): " << linearHashing.getProbe() << endl;
				linearHashing.setProbe(0);
				break;
			}
			else if (choice == 3) // doubleHashing
			{
				int code;
				int population;
				string county;
				cout << "Record to insert(code population county/state)--> ";
				cin >> code >> population;
				getline(cin, county);
				doubleHashing.put(code, population, county);
				cout << "Number of Probe(s): " << doubleHashing.getProbe() << endl;
				doubleHashing.setProbe(0);
				break;
			}
		}
		case 3:		// Delete
		{
			if (choice == 1) // chainHashing
			{
				int erase;
				cout << "Record to erase--> ";
				cin >> erase;
				chainHashing.erase(erase);
				cout << "Number of Probe(s): " << chainHashing.getProbe() << endl;
				chainHashing.setProbe(0);
				break;
			}
			else if (choice == 2) // linearHashing
			{
				int erase;
				cout << "Record to erase--> ";
				cin >> erase;
				linearHashing.erase(erase);
				cout << "Number of Probe(s): " << linearHashing.getProbe()+1 << endl;
				linearHashing.setProbe(0);
				break;
			}
			else if (choice == 3) // doubleHashing
			{
				int erase;
				cout << "Record to erase--> ";
				cin >> erase;
				doubleHashing.erase(erase);
				cout << "Number of Probe(s): " << doubleHashing.getProbe()+1 << endl;
				doubleHashing.setProbe(0);
				break;
			}
		}
		case 4:		// List All Values
		{
			cout << endl;
			switch (choice)
			{
			case 1: // chainHashing
			{
				chainHashing.print();
				break;
			}
			case 2: // linearHashing
			{
				linearHashing.print();
				break;
			}
			case 3: // doubleHashing
			{
				doubleHashing.print();
				break;
			}
			}
			break;
		}
		case 5:		// Exit
		{
			cout << "Thank you for using my program!" << endl;
			break;
		}
		default:
		{
			cout << "Please enter a valid option!" << endl;
			break;
		}
		}
	} while (choice2 != 5);
}
