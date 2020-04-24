#pragma once

using namespace std;

class SeparateHashing
{
public:
	int operator()(const int& v)
	{
		return v;
	}
	int operator()(string s)
	{
		unsigned int h = 0;
		for (int i = 0; i < s.length(); i++)
		{
			h = (h << 1) | (h >> 31);
			h += (unsigned int)s[i];
		}
		return h;
	}
};
