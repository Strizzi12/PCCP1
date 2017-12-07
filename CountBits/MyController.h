#pragma once
#include <string>
#include <vector>
#include <time.h>

using namespace std;

class MyController
{
public:
	MyController();
	~MyController();
	


	//Variables
	bool WaitForTermination;
	bool PrintProcessTime;
	int MaxThreads;
	bool OptimizeThreadCount;
	bool Error;
	string FileFilter;
	vector<string> FilePaths;
	int DepthOfRecursion;
	int CurrentRecursion;
	bool MoreInfo;
	clock_t StartTime;
	clock_t StopTime;

	//Functions
	void ParseInputArguments(int argc, char* argv[]);
	void PrintHelp();
	void Wait();
	void MyPrint(string str);
	void PrintTime();
	clock_t GetTime();
	void SetStartTime();
	void SetStopTime();

private:
	bool IsAllDigits(string &str);

};

