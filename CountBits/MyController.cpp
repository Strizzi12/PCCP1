#include "MyController.h"
#include <iostream>
#include "string"
#include <algorithm>
#include "MyCalculator.h"
#include <boost/iostreams/device/mapped_file.hpp>

using namespace std;

MyController::MyController()
{
	error = false;
	waitForTermination = true;
	printProcessTime = false;
	optimizeThreadCount = false;
	maxThreads = 0;
	fileFilter = string("");		//if fileFilter is "", then take all files
	filePaths = vector<string>();
	depthOfRecursion = 0;
	currentRecursion = 0;}


MyController::~MyController()
{
}

void MyController::ParseInputArguments(int argc, char* argv[])
{
	for (int i = 1; i <= argc; i++)
	{
		if (argv[i] == "-w")
		{
			waitForTermination = true;
			continue;
		}
		if (argv[i] == "-p")
		{
			printProcessTime = true;
			continue;
		}
		if (argv[i] == "-t" && argv[i + 1] != NULL)
		{
			const char* str = "-";
			if (strstr(argv[i + 1], str) != NULL)	//Check if the next input argument is another functionality.
			{
				optimizeThreadCount = true;			//When file size is known, a calculation of the optimal thread count could be made.
				continue;
			}
			string help(argv[i + 1]);
			if (IsAllDigits(help))
			{
				maxThreads = stoi(help);
				i++;								//Counter can be increased because the value of maxThreads is already read.
				continue;
			}
			else
			{
				cerr << "Error! Max. thread count must not contain character." << endl;
				error = true;
				break;
			}			
		}
		if (argv[i] == "-h")
		{
			PrintHelp();
			continue;
		}
		if (argv[i] == "-f" && argv[i + 1] != NULL)
		{
			fileFilter = string(argv[i + 1]);
			i++;									//Counter can be increased because the value of fileFilter is already read.
			continue;
		}
		if (argv[i] == "-s" && argv[i + 1] != NULL)
		{
			filePaths.push_back(string(argv[i + 1]));
			i++;									//Counter can be increased because the value of filePath is already read.
			continue;
		}
		if (argv[i] == "-r" && argv[i + 1] != NULL)
		{
			const char* str = "-";
			if (strstr(argv[i + 1], str) != NULL)	//Check if the next input argument is another functionality.
			{
				depthOfRecursion = 0;				//All files and folders are progressed
				continue;
			}
			string help(argv[i + 1]);
			if (IsAllDigits(help))
			{
				depthOfRecursion = stoi(help);
				i++;								//Counter can be increased because the value of maxThreads is already read.
				continue;
			}
			else
			{
				cerr << "Error! Folder depth must not contain character." << endl;
				error = true;
				break;
			}
		}
		else
		{
			cerr << "Error! Unknown argument detected." << endl;
			error = true;
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
	cout << "-w					Warten auf eine Taste unmittelbar bevor die applikation terminiert." << endl;
}

void MyController::WaitForTermination()
{
	if (waitForTermination == true)
	{
		cin.ignore();
	}
}

//private functions
bool MyController::IsAllDigits(string& str)
{
	return all_of(str.begin(), str.end(), isdigit); // C++11
}