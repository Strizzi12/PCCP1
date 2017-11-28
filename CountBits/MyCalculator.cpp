#include "MyCalculator.h"
#include <iostream>
#include <vector>


MyCalculator::MyCalculator()
{
}


MyCalculator::~MyCalculator()
{
}

ifstream::pos_type MyCalculator::GetFileSize(const char* fileName)
{
	ifstream in(fileName, ios::binary | ios::ate);
	return in.tellg();
}

/*
*https://stackoverflow.com/questions/3902609/how-do-i-count-the-zeros-and-ones-in-a-file
*/
uint64_t MyCalculator::CountBits(BYTE *data, int fileSize)
{
	//This array needs to be initialized in here
	BYTE Cheating[] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3,
		4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 1, 2, 2, 3, 2,
		3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4,
		5, 4, 5, 5, 6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2,
		3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4,
		5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5,
		6, 6, 7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 2, 3, 3,
		4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 2, 3, 3, 4, 3, 4, 4, 5, 3,
		4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6,
		7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 3, 4, 4, 5, 4,
		5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5,
		6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8 };

	uint64_t sumOf1 = 0;
	for (int i = 0; i < fileSize; i++)
	{
		auto anz1 = Cheating[data[i]];
		sumOf1 += (uint64_t)anz1;
	}
	return sumOf1;
}
