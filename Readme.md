# Parallel Computing Course

Parallel Computing is a specialization in the 3rd semester of the master's degree at the University of Applied Sciences in Salzburg.

## Getting Started

The task was to develope a Windows C++ console application which counts the 0 and 1 of any data from a given start path and it's subdirectories.
The application should be optimized by process time and memory management with the use of threads and other algorithms.

**Hard demands**
* The application should be built for batch processing. That means no other user inputs are necessary to run the application
* The application should print all console outputs and the count of read 0 and 1 bits to *stdout*
* Errors need to be printed to *stderr* output

Here is an example on how the application can be called and it's parameter:

cntFileBits [-r [n]] [-f fileFilter] [-t maxThreads] [-h] [-p] [-v] [-w] [-s startPath]

-s startPath    Gibt das Startverzeichnis an, ab dem die Dateien gelesen werden sollen;<br />
                die Option -s kann auch mehrfach angegeben werden, z.B. wenn zwei Partitionen durchsucht werden sollen<br />
-r [n]          Rekursives Lesen der Unterverzeichnisse; wenn n (bei n >= 1) angegeben, dann<br />
                bestimmt n die Tiefe der Rekursion; wird n nicht angegeben, dann werden<br />
                rekursiv alle unter dem Startverzeichnis stehenden Verzeichnisse und deren Dateien gelesen;<br />
-f fileFilter   fileFilter gibt an, welche Dateien gelesen werden sollen; z.B. *.iso oder bild*.jpg;<br />
                wird diese Option nicht angegeben, so werden alle Dateien gelesen;<br />
-t maxThreads   maximale Anzahl der Threads; wird diese Option nicht angegeben, dann wird die Anzahl der Threads automatisch optimiert<br />
-h              Anzeige der Hilfe & Copyright Info; wird automatisch angezeigt, wenn beim Programmstart keinen Option angegeben wird<br />
-p              Ausgabe der Prozesserungszeit auf stdout in Sekunden.Millisekunden<br />
-v              Erweiterte Ausgabe etwaiger Prozessierungsinformationen auf stdout<br />
-w              Warten auf eine Taste unmittelbar bevor die applikation terminiert<br />

### Code

At first the input parameters from the command line are parsed and variables are set.
A help function is called if invalid parameter are parsed.

The basic idea of counting the 0 and 1 bits is by using a predefined lookup table, where the number of ones are stored. For 8 bit the lookup table looks like:
```cpp
const BYTE Cheating[256] = 
{ 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3,
4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 
5, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 
5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 
6, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 
5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 
6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 
6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 
7, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 
5, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 
6, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 
6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 
7, 2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 
6, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 
7, 3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 
7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8 };
```

This can also be done in a much more readable way (See [this](https://stackoverflow.com/questions/41104560/how-to-generate-16-bit-lookup-table-for-counting-set-bits) post for further information on how this works):

```cpp
const unsigned char BitsSetTable256[256] = {
#   define B2(n) n,     n+1,     n+1,     n+2
#   define B4(n) B2(n), B2(n+1), B2(n+1), B2(n+2)
#   define B6(n) B4(n), B4(n+1), B4(n+1), B4(n+2)
           B6(0),B6(1), B6(1),   B6(2)
};
```

To read a file the boost implemented class "mapped_file_source" is used. With this method the complete file is mapped and can easily be opened without reading the bytes beforehand.
```cpp
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
```

The bit count function is implemented like this:
```cpp
uint64_t MyCalculator::CountBits(BYTE *data, int fileSize)
{
    uint64_t sumOf1 = 0;

    for (int i = 0; i < fileSize; i++)
    {
        //auto anz1 = Cheating[data[i]];
        auto anz1 = Cheating[data[i]];
        sumOf1 += (uint64_t)anz1;
    }
    return sumOf1;
}
```

To assign a file filter a regex pattern is used to match any * in the given input parameter:
```cpp
for (auto token : myController.FileFilter)
{
    boost::replace_all(token, "*", "([a-zA-Z0-9\._\-]*)");
    regex rx(token);
    bool found = regex_match(filename, rx);
    if (found != true)
    {
        continue;
    }
    //Calculation stuff
}
```

## Built With

* [Boost Library 1.65.1](http://www.boost.org/users/history/version_1_65_1.html) - The library used to map files from the hard drive

## Authors

* **Mike Thomas**
* **Andreas Reschenhofer**

See also the list of [contributors](https://github.com/your/project/contributors) who participated in this project.

## License

No license information