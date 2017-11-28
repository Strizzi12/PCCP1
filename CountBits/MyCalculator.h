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
	
	ifstream::pos_type GetFileSize(const char * fileName);
	uint64_t CountBits(vector<BYTE> vec);

private:
	unsigned char CheatingVector[];
};



