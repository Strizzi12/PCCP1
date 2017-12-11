#include "MyController.h"
#include <Windows.h>
#include <iostream>
#include "string"
#include <sstream> 
#include <algorithm>
#include "MyCalculator.h"
#include <boost/iostreams/device/mapped_file.hpp>
#include <chrono>

using namespace std;
using namespace chrono;

MyController::MyController()
{
	Error = false;
	WaitForTermination = false;
	PrintProcessTime = false;
	OptimizeThreadCount = false;
	MaxThreads = 0;
	FileFilter = vector<string>();
	FilePaths = vector<string>();
	DepthOfRecursion = 0;
	CurrentRecursion = 0;
	MoreInfo = false;
}


MyController::~MyController()
{
}

void MyController::ParseInputArguments(int argc, char* argv[])
{
	for (int i = 1; i <= (argc - 1); i++)
	{
		if (strcmp(argv[i], "-w") == 0)
		{
			WaitForTermination = true;
			continue;
		}
		if (strcmp(argv[i], "-p") == 0)
		{
			PrintProcessTime = true;
			continue;
		}
		if (strcmp(argv[i], "-t") == 0 && argv[i + 1] != NULL)
		{
			const char* str = "-";
			if (strstr(argv[i + 1], str) != NULL)	//Check if the next input argument is another functionality.
			{
				OptimizeThreadCount = true;			//When file size is known, a calculation of the optimal thread count could be made.
				continue;
			}
			string help(argv[i + 1]);
			if (IsAllDigits(help))
			{
				MaxThreads = stoi(help);
				i++;								//Counter can be increased because the value of maxThreads is already read.
				continue;
			}
			else
			{
				cerr << "Error! Max. thread count must not contain character." << endl;
				Error = true;
				break;
			}
		}
		if (strcmp(argv[i], "-h") == 0)
		{
			PrintHelp();
			continue;
		}
		if (strcmp(argv[i], "-v") == 0)
		{
			MoreInfo = true;
			continue;
		}
		if (strcmp(argv[i], "-f") == 0 && argv[i + 1] != NULL)
		{
			auto completeString = string(argv[i + 1]);
			stringstream test(completeString);
			string segment;
			while (getline(test, segment, ';'))
			{
				FileFilter.push_back(segment);
			}
			i++;									//Counter can be increased because the value of fileFilter is already read.
			continue;
		}
		if (strcmp(argv[i], "-s") == 0 && argv[i + 1] != NULL)
		{
			FilePaths.push_back(string(argv[i + 1]));
			i++;									//Counter can be increased because the value of filePath is already read.
			continue;
		}
		if (strcmp(argv[i], "-r") == 0 && argv[i + 1] != NULL)
		{
			const char* str = "-";
			if (strstr(argv[i + 1], str) != NULL)	//Check if the next input argument is another functionality.
			{
				DepthOfRecursion = 0;				//All files and folders are progressed
				continue;
			}
			string help(argv[i + 1]);
			if (IsAllDigits(help))
			{
				DepthOfRecursion = stoi(help);
				i++;								//Counter can be increased because the value of maxThreads is already read.
				continue;
			}
			else
			{
				cerr << "Error! Folder depth must not contain character." << endl;
				Error = true;
				break;
			}
		}
		else
		{
			cerr << "Error! Unknown argument detected." << endl;
			Error = true;
			break;
		}
	}
}

void MyController::PrintHelp()
{
	cout << "Die Applikation kann wie folgt aufzurufen:" << endl << endl;
	cout << "cntFileBits [-r [n]] [-f fileFilter] [-t maxThreads] [-h] [-p] [-v] [-w] [-s startPath]" << endl << endl;
	cout << "-s startPath	Gibt das Startverzeichnis an, ab dem die Dateien gelesen werden sollen;" << endl;
	cout << "		die Option -s kann auch mehrfach angegeben werden, z.B. wenn zwei Partitionen durchsucht werden sollen" << endl;
	cout << "-r [n]		Rekursives Lesen der Unterverzeichnisse; wenn n (bei n >= 1) angegeben, dann" << endl;
	cout << "		bestimmt n die Tiefe der Rekursion; wird n nicht angegeben, dann werden" << endl;
	cout << "		rekursiv alle unter dem Startverzeichnis stehenden Verzeichnisse und deren Dateien gelesen;" << endl;
	cout << "-f fileFilter	fileFilter gibt an, welche Dateien gelesen werden sollen; z.B. *.iso oder bild*.jpg;" << endl;
	cout << "		wird diese Option nicht angegeben, so werden alle Dateien gelesen;" << endl;
	cout << "-t maxThreads	maximale Anzahl der Threads; wird diese Option nicht angegeben, dann wird die Anzahl der Threads automatisch optimiert." << endl;
	cout << "-h		Anzeige der Hilfe & Copyright Info; wird automatisch angezeigt, wenn beim Programmstart keinen Option angegeben wird." << endl;
	cout << "-p		Ausgabe der Prozesserungszeit auf stdout in Sekunden.Millisekunden" << endl;
	cout << "-v		Erweiterte Ausgabe etwaiger Prozessierungsinformationen auf stdout" << endl;
	cout << "-w		Warten auf eine Taste unmittelbar bevor die applikation terminiert." << endl << endl;
}

void MyController::Wait()
{
	if (WaitForTermination == true)
	{
		cout << endl << "Finished, waiting for key press.";
		cin.ignore();
	}
}

void MyController::MyPrint(string str)
{
	if (MoreInfo == true)
	{
		cout << str << endl;
	}
}

void MyController::PrintTime()
{
	if (PrintProcessTime == true)
	{
		printf("Time taken: %.6fs\n", double(StopTime - StartTime) / CLOCKS_PER_SEC);

		duration<double> time_span = duration_cast<duration<double>>(StopTimeHighResolution - StartTimeHighResolution);
		//printf("Time taken (high resolution time): %.6fs\n", time_span.count());
		cout << "Time taken (high resolution time): " << time_span.count() << endl;
	}
}

clock_t MyController::GetTime()
{
	return clock();
}

high_resolution_clock::time_point MyController::GetHighResolutionTime()
{
	return high_resolution_clock::now();
}	

void MyController::SetStartTime()
{
	if (PrintProcessTime == true)
	{
		StartTime = GetTime();
		StartTimeHighResolution = GetHighResolutionTime();
	}
}

void MyController::SetStopTime()
{
	if (PrintProcessTime == true)
	{
		StopTime = GetTime();
		StopTimeHighResolution = GetHighResolutionTime();
	}
}

//private functions
bool MyController::IsAllDigits(string& str)
{
	return all_of(str.begin(), str.end(), isdigit); // C++11
}