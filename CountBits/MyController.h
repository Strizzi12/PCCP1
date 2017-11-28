#pragma once
#include <string>
#include <vector>

using namespace std;

class MyController
{
public:
	MyController();
	~MyController();

	//Variables
	bool waitForTermination;
	bool printProcessTime;
	int maxThreads;
	bool optimizeThreadCount;
	bool error;
	string fileFilter;
	vector<string> filePaths;
	int depthOfRecursion;

	//Functions
	void ParseInputArguments(int argc, char* argv[]);
	void PrintHelp();
	void WaitForTermination();

private:
	bool IsAllDigits(string &str);

};

