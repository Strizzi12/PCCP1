#include "MyCalculator.h"
#include <iostream>
#include <vector>
#include <boost/algorithm/string/replace.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/filesystem/string_file.hpp>
#include <boost/foreach.hpp>
#include "MyResult.h"
#include <regex>
#include "mmfile.hpp"
#include <windows.h>

const BYTE Cheating[256] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3,
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

static const BYTE BitsSetTable65536[65536] =
{
#   define B2(n)  n,      n+1,      n+1,      n+2
#   define B4(n)  B2(n),  B2(n+1),  B2(n+1),  B2(n+2)
#   define B6(n)  B4(n),  B4(n+1),  B4(n+1),  B4(n+2)
#   define B8(n)  B6(n),  B6(n+1),  B6(n+1),  B6(n+2)
#   define B10(n) B8(n),  B8(n+1),  B8(n+1),  B8(n+2)
#   define B12(n) B10(n), B10(n+1), B10(n+1), B10(n+2)
#   define B14(n) B12(n), B12(n+1), B12(n+1), B12(n+2)
		   B14(0),B14(1), B14(1),   B14(2)
};

MyCalculator::MyCalculator()
{
}


MyCalculator::~MyCalculator()
{
}

uint64_t MyCalculator::GetFileSizeFast(wstring const &path)
{
	WIN32_FIND_DATAW data;
	HANDLE h = FindFirstFileW(path.c_str(), &data);
	if (h == INVALID_HANDLE_VALUE)
		return -1;

	FindClose(h);

	return data.nFileSizeLow | (uint64_t)data.nFileSizeHigh << 32;
}

uint64_t MyCalculator::GetFileSizeFaster(const char *filename)
{
	struct __stat64 st;
	__stat64(filename, &st);
	return st.st_size;
}

ifstream::pos_type MyCalculator::GetFileSize(const char* fileName)
{
	ifstream in(fileName, ios::binary | ios::ate);
	return in.tellg();
}

MyResult MyCalculator::CountBitsOf1ForPath(const char *path, MyController &myController)
{

	MyCalculator myCalculator;
	MyResult result;

	try
	{
		boost::filesystem::path targetDir(path);
		boost::filesystem::directory_iterator it(targetDir), eod;

		BOOST_FOREACH(boost::filesystem::path const &p, std::make_pair(it, eod))
		{
			MyResult temp;
			if (is_directory(p) && myController.DepthOfRecursion != myController.CurrentRecursion && myController.DepthOfRecursion != 0)
			{
				myController.CurrentRecursion++;
				//call countBits function again with new path
				temp = myCalculator.CountBitsOf1ForPath(p.string().c_str(), myController);
				//result.SumBit0 += temp.SumBit0;
				result.SumBit1 += temp.SumBit1;
				result.FileSize += temp.FileSize;
			}
			else if (is_directory(p) && myController.DepthOfRecursion == 0)
			{
				temp = myCalculator.CountBitsOf1ForPath(p.string().c_str(), myController);
				//result.SumBit0 += temp.SumBit0;
				result.SumBit1 += temp.SumBit1;
				result.FileSize += temp.FileSize;
			}
			else if (is_regular_file(p))
			{
				uint64_t sum1 = 0;

				const string filename = p.filename().string();
				if (filename == "")
				{
					cerr << "Error: Filename cannot be empty" << endl;
					continue;
				}
				if (myController.FileFilter.empty() == true)
				{
					string fullPath = string(path) + "\\" + filename;
					//const auto fileSize = myCalculator.GetFileSize(fullPath.c_str());
					//const auto fileSize = myCalculator.GetFileSizeFast(StringToWString(fullPath));
					const auto fileSize = myCalculator.GetFileSizeFaster(fullPath.c_str());
					if (fileSize == 0)
					{
						continue;
					}
					if (myController.MoreInfo == true)
					{
						cout << "Processing file: " + filename + " with filesize of " + to_string(uint64_t(fileSize)) << endl;
					}
					//myController.MyPrint("Processing file: " + filename + " with filesize of " + to_string(uint64_t(fileSize)));
					try
					{
						boost::iostreams::mapped_file_source file; //is already readonly
						file.open(fullPath.c_str(), fileSize);
						// Check if file was successfully opened
						if (file.is_open())
						{
							// Get pointer to the data
							BYTE *data = (BYTE *)file.data();

							// Do something with the data
							sum1 = myCalculator.CountBits(data, fileSize);

							// Remember to unmap the file
							file.close();
						}
						else
						{
							cerr << "Could not map (in normal method) the file: " << fullPath << endl;
							return MyResult::MyResult();
						}
					}
					catch (exception &ex)
					{
						//cerr << "Error: " << ex.what() << endl;
						try
						{
							memory_mapped_file::read_only_mmf mmf(fullPath.c_str(), true);	//https://github.com/carlomilanesi/cpp-mmf/
							if (mmf.is_open())
							{
								// Get pointer to the data
								BYTE *data = (BYTE *)mmf.data();

								// Do something with the data
								sum1 = myCalculator.CountBits((BYTE *)mmf.data(), fileSize);

								//result.SumBit0 += ((fileSize * 8) - sum1);
								result.SumBit1 += sum1;
								result.FileSize += fileSize;
								continue;
							}
							else
							{
								ifstream file(fullPath, ios::in | ios::binary);
								unsigned char a;

								if (file.is_open())
								{
									while (!file.eof())
									{
										file.read(reinterpret_cast<char *>(&a), sizeof(a));
										auto anz1 = Cheating[a];
										sum1 += (uint64_t)anz1;
									}
								}
								else
								{
									cerr << "Could not map (in fabi method) the file: " << fullPath << endl;
									return MyResult::MyResult();
								}								
							}							
						}
						catch (exception &ex2)
						{
							cerr << "Inner Error: " << ex2.what() << endl;
						}
						continue;
					}
					//result.SumBit0 += ((fileSize * 8) - sum1);
					result.SumBit1 += sum1;
					result.FileSize += fileSize;
				}
				else
				{
					//Check file name
					for (auto token : myController.FileFilter)
					{
						boost::replace_all(token, "*", "([a-zA-Z0-9\._\-]*)");
						regex rx(token);
						bool found = regex_match(filename, rx);

						if (found != true)
						{
							continue;
						}

						string fullPath = string(path) + "\\" + filename;
						const auto fileSize = myCalculator.GetFileSizeFaster(fullPath.c_str());
						if (uint64_t(fileSize) == 0)
						{
							continue;
						}
						if (myController.MoreInfo == true)
						{
							cout << "Processing file: " + filename + " with filesize of " + to_string(uint64_t(fileSize)) << endl;
						}
						//myController.MyPrint("Processing file: " + filename + " with filesize of " + to_string(uint64_t(fileSize)));

						boost::iostreams::mapped_file_source file; //is already readonly
						file.open(fullPath.c_str(), fileSize);

						// Check if file was successfully opened
						if (file.is_open()) 
						{

							// Get pointer to the data
							BYTE *data = (BYTE *)file.data();

							// Do something with the data
							sum1 = myCalculator.CountBits(data, fileSize);

							// Remember to unmap the file
							file.close();
						}
						else 
						{
							cerr << "Could not map the file" << endl;
							return MyResult::MyResult();
						}
						//result.SumBit0 += ((fileSize * 8) - sum1);
						result.SumBit1 += sum1;
						result.FileSize += fileSize;
						break;
					}
				}
			}
		}
		myController.CurrentRecursion -= 1;	//Fix for depth of recursion, not perfect but it goes
	}
	catch (boost::filesystem::filesystem_error &ex)
	{
		cerr << "Boost filesystem error: " << ex.what() << endl;
		return MyResult::MyResult();
	}
	catch (exception &ex)
	{
		cerr << "Error: " << ex.what() << endl;
		return MyResult::MyResult();
	}
	return result;
}

/*
*https://stackoverflow.com/questions/3902609/how-do-i-count-the-zeros-and-ones-in-a-file
*/
uint64_t MyCalculator::CountBits(BYTE *data, int fileSize)
{
	uint64_t sumOf1 = 0;

	//#pragma omp parallel for reduction (+:total)
	for (int i = 0; i < fileSize; i++)
	{
		//auto anz1 = Cheating[data[i]];
		auto anz1 = Cheating[data[i]];
		sumOf1 += (uint64_t)anz1;
	}
	return sumOf1;
}