// CountBytes.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <vector>
#include <fstream>
#include <iterator>
#include <iostream>
#include <bitset>
#include <stdio.h>
#include "MyConstants.h"
#include "MyCalculator.h"
#include "MyController.h"
#include <boost/iostreams/device/mapped_file.hpp>

using namespace std;

const char *fileName = "Main.cpp";
const char *fileName2 = "C:\\Users\\Andi\\Downloads\\HDP_2.6_1_docker_virtualbox_28_07_2017_15_12_53_hdp__ambari_.ova";
const char *fileName3 = "Logo22.png";
const char *fileName4 = "C:\\Users\\Andi\\Downloads\\a\\a.txt";
const char *fileName5 = "C:\\Users\\Andi\\Downloads\\a\\BincountTest.exe";

int main(int argc, char* argv[])
{
	clock_t tStart = clock();
	
	const char *fileToUse = fileName2;

	auto myCalculator = new MyCalculator();
	auto myController = new MyController();
	//BOOST Test
	boost::iostreams::mapped_file_source file;
	auto fileSize = myCalculator->GetFileSize(fileToUse);

	file.open(fileToUse, fileSize);
	uint64_t sum1 = 0;
	// Check if file was successfully opened
	if (file.is_open()) {

		// Get pointer to the data
		BYTE *data = (BYTE *)file.data();

		// Do something with the data
		sum1 = myCalculator->CountBits(data, fileSize);
				
		// Remember to unmap the file
		file.close();
	}
	else {
		cout << "could not map the file" << endl;
		return 0;
	}
	
	uint64_t sum0 = (uint64_t)fileSize * (uint64_t)8 - sum1;
	cout << "0er: " << sum0 << endl;
	cout << "1er: " << sum1 << endl;
	cout << "Sum: " << sum0 + sum1 << endl;

	/*if (argc < 2)
	{
		MyController::PrintHelp();
	}
	else 
	{
		MyController::ParseInputArguments(argc, argv);
	}*/

	printf("Time taken: %.6fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	myController->WaitForTermination();
	return 0;
}
