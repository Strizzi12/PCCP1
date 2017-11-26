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
	
	static ifstream::pos_type GetFileSize(const char * fileName);
	static uint64_t CountBits(vector<BYTE> vec);

private:
	static unsigned char CheatingVector[];
};



