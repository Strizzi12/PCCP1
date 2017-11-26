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
	static bool waitForTermination;
	static bool printProcessTime;
	static int maxThreads;
	static bool optimizeThreadCount;
	static bool error;
	static string fileFilter;
	static vector<string> filePaths;
	static int depthOfRecursion;

	//Functions
		static void ParseInputArguments(int argc, char* argv[]);
	static void PrintHelp();
	static void WaitForTermination();

private:
	static bool IsAllDigits(string &str);

};

