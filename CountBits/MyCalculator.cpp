#include "MyCalculator.h"
#include <iostream>
#include <vector>
#include <boost/iostreams/device/mapped_file.hpp>
#include <boost/filesystem/string_file.hpp>
#include <boost/foreach.hpp>
#include "MyResult.h"


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

MyResult MyCalculator::CountBitsOf1ForPath(const char *path, MyController &myController)
{
	try
	{
		MyCalculator myCalculator;
		MyResult result;
		
		boost::filesystem::path targetDir(path);
		boost::filesystem::directory_iterator it(targetDir), eod;
		
		BOOST_FOREACH(boost::filesystem::path const &p, std::make_pair(it, eod))
		{
			uint64_t sum1 = 0;
			//boost::filesystem::detail::permissions(targetDir, boost::filesystem::others_read | boost::filesystem::owner_read);
			if (is_directory(p) && myController.depthOfRecursion != myController.currentRecursion && myController.depthOfRecursion != 0)
			{
				myController.currentRecursion++;
				//call countBits function again with new path
				MyResult temp = myCalculator.CountBitsOf1ForPath(p.string().c_str(), myController);
				result.SumBit0 += temp.SumBit0;
				result.SumBit1 += temp.SumBit1;
				result.FileSize += temp.FileSize;
			}
			else if (is_directory(p) && myController.depthOfRecursion == 0)
			{
				MyResult temp = myCalculator.CountBitsOf1ForPath(p.string().c_str(), myController);
				result.SumBit0 += temp.SumBit0;
				result.SumBit1 += temp.SumBit1;
				result.FileSize += temp.FileSize;
			}
			else if (is_regular_file(p))
			{
				const string filename = p.filename().string();
				string fullPath = string(path) + "\\" + filename;

				const auto fileSize = myCalculator.GetFileSize(fullPath.c_str());
				if (uint64_t(fileSize) == 0)
				{
					continue;
				}
				boost::iostreams::mapped_file_source file; //is already readonly
				file.open(fullPath.c_str(), fileSize);

				// Check if file was successfully opened
				if (file.is_open()) {

					// Get pointer to the data
					BYTE *data = (BYTE *)file.data();

					// Do something with the data
					sum1 = myCalculator.CountBits(data, fileSize);

					// Remember to unmap the file
					file.close();
				}
				else {
					cerr << "Could not map the file" << endl;
					return MyResult();
				}
				result.SumBit0 += (uint64_t)fileSize * (uint64_t)8 - sum1;
				result.SumBit1 += sum1;
				result.FileSize += fileSize;
			}			
		}
		return result;
	}
	catch (boost::filesystem::filesystem_error &ex)
	{
		cerr << "Boost filesystem error: " << ex.what() << endl;
		return MyResult();
	}
	catch (exception &ex)
	{
		cerr << "Error: " << ex.what() << endl;
		return MyResult();
	}
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

	//#pragma omp parallel for reduction (+:total)
	for (int i = 0; i < fileSize; i++)
	{
		auto anz1 = Cheating[data[i]];
		sumOf1 += (uint64_t)anz1;
	}
	return sumOf1;
}
