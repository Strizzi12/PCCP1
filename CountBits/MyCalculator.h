#pragma once
#include <fstream>
#include <vector>

using namespace std;

typedef unsigned char BYTE;

class MyCalculator
{
public:
	MyCalculator();
	~MyCalculator();
	static unsigned char CheatingVector[];
	
	static ifstream::pos_type GetFileSize(const char * fileName);
	static uint64_t CountBits(vector<BYTE> vec);
};



