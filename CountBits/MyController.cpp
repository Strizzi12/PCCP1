#include "MyController.h"
#include <iostream>
#include "string"
#include <algorithm>
#include "MyCalculator.h"
#include <boost/iostreams/device/mapped_file.hpp>

using namespace std;

MyController::MyController()
{
	Error = false;
	WaitForTermination = true;
	PrintProcessTime = false;
	OptimizeThreadCount = false;
	MaxThreads = 0;
	FileFilter = string("");		//if fileFilter is "", then take all files
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
	for (int i = 1; i <= argc; i++)
	{
		if (argv[i] == "-w")
		{
			WaitForTermination = true;
			continue;
		}
		if (argv[i] == "-p")
		{
			PrintProcessTime = true;
			continue;
		}
		if (argv[i] == "-t" && argv[i + 1] != NULL)
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
		if (argv[i] == "-h")
		{
			PrintHelp();
			continue;
		}
		if (argv[i] == "-v")
		{
			MoreInfo = true;
			continue;
		}
		if (argv[i] == "-f" && argv[i + 1] != NULL)
		{
			FileFilter = string(argv[i + 1]);
			i++;									//Counter can be increased because the value of fileFilter is already read.
			continue;
		}
		if (argv[i] == "-s" && argv[i + 1] != NULL)
		{
			FilePaths.push_back(string(argv[i + 1]));
			i++;									//Counter can be increased because the value of filePath is already read.
			continue;
		}
		if (argv[i] == "-r" && argv[i + 1] != NULL)
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
	cout << "-s startPath		Gibt das Startverzeichnis an, ab dem die Dateien gelesen werden sollen;" << endl;
	cout << "					die Option -s kann auch mehrfach angegeben werden, z.B. wenn zwei Partitionen durchsucht werden sollen" << endl << endl;
	cout << "-r [n]				Rekursives Lesen der Unterverzeichnisse; wenn n (bei n >= 1) angegeben, dann" << endl;
	cout << "					bestimmt n die Tiefe der Rekursion; wird n nicht angegeben, dann werden" << endl;
	cout << "					rekursiv alle unter dem Startverzeichnis stehenden Verzeichnisse und deren Dateien gelesen;" << endl << endl;
	cout << "-f fileFilter		fileFilter gibt an, welche Dateien gelesen werden sollen; z.B. *.iso oder bild*.jpg;" << endl;
	cout << "					wird diese Option nicht angegeben, so werden alle Dateien gelesen;" << endl << endl;
	cout << "-t maxThreads		maximale Anzahl der Threads; wird diese Option nicht angegeben, dann wird die Anzahl der Threads automatisch optimiert." << endl << endl;
	cout << "-h					Anzeige der Hilfe & Copyright Info; wird automatisch angezeigt, wenn beim Programmstart keinen Option angegeben wird." << endl << endl;
	cout << "-p					Ausgabe der Prozesserungszeit auf stdout in Sekunden.Millisekunden" << endl << endl;
	cout << "-v					Erweiterte Ausgabe etwaiger Prozessierungsinformationen auf stdout" << endl << endl;
	cout << "-w					Warten auf eine Taste unmittelbar bevor die applikation terminiert." << endl;
}

void MyController::Wait()
{
	if (WaitForTermination == true)
	{
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
		printf("Time taken: %.6lds\n", (StartTime - StopTime) / CLOCKS_PER_SEC);
	}
}

clock_t MyController::GetTime()
{
	return clock();
}

void MyController::SetStartTime()
{
	if (PrintProcessTime == true)
	{
		StartTime = GetTime();
	}
}

void MyController::SetStopTime()
{
	if (PrintProcessTime == true)
	{
		StopTime = GetTime();
	}
}

//private functions
bool MyController::IsAllDigits(string& str)
{
	return all_of(str.begin(), str.end(), isdigit); // C++11
}