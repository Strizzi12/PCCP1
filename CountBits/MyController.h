#pragma once
#include <string>
#include <vector>
#include <time.h>
#include <chrono>

using namespace std;
using namespace chrono;

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
	vector<string> FileFilter;
	vector<string> FilePaths;
	int DepthOfRecursion;
	int CurrentRecursion;
	bool MoreInfo;
	clock_t StartTime;
	clock_t StopTime;
	high_resolution_clock::time_point StartTimeHighResolution;
	high_resolution_clock::time_point StopTimeHighResolution;

	//Functions
	void ParseInputArguments(int argc, char* argv[]);
	void PrintHelp();
	void Wait();
	void MyPrint(string str);
	void PrintTime();
	static clock_t GetTime();
	static high_resolution_clock::time_point GetHighResolutionTime();
	void SetStartTime();
	void SetStopTime();

private:
	bool IsAllDigits(string &str);

};

