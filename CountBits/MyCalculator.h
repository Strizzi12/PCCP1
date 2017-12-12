#pragma once
#include <fstream>
#include <vector>
#include "MyController.h"
#include "MyResult.h"
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/filesystem/path.hpp>

using namespace std;

typedef unsigned char BYTE;

class MyCalculator
{
public:
	MyCalculator();
	~MyCalculator();
	
	ifstream::pos_type GetFileSize(const char * fileName);
	uint64_t GetFileSizeFast(wstring const &path);
	uint64_t GetFileSizeFaster(const char *filename);
	uint64_t CountBits(BYTE *data, int fileSize);
	MyResult CountBitsOf1ForPath(const char *path, MyController &myController);

	wstring StringToWString(const string &s)
	{
		wstring temp(s.length(), L' ');
		copy(s.begin(), s.end(), temp.begin());
		return temp;
	}
};