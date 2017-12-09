#pragma once
#include <fstream>
#include <vector>
#include "MyController.h"
#include "MyResult.h"

using namespace std;

typedef unsigned char BYTE;

class MyCalculator
{
public:
	MyCalculator();
	~MyCalculator();
	
	ifstream::pos_type GetFileSize(const char * fileName);
	uint64_t CountBits(BYTE *data, int fileSize);
	MyResult CountBitsOf1ForPath(const char *path, MyController &myController);

};