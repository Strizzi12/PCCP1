// CountBytes.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <vector>
#include <fstream>
#include <iterator>
#include <iostream>

using namespace std;
typedef unsigned char BYTE;


int main(int argc, char* argv[])
{
  unsigned char cheating[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3,
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
    6, 5, 6, 6, 7, 4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8};

  // open the file:
  ifstream file("C:\\root\\ITS\\Projects\\Parallel Computing\\XmlLoaderApp\\XmlLoaderApp\\bin\\Debug\\KMLFiles\\Naunspitze-Stpeterskoepfle-pyramidenspitze.kml", std::ios::binary);

  // Stop eating new lines in binary mode!!!
  file.unsetf(std::ios::skipws);

  // get its size:
  streampos fileSize;

  file.seekg(0, ios::end);
  fileSize = file.tellg();
  file.seekg(0, ios::beg);

  // reserve capacity
  vector<BYTE> vec;
  vec.reserve(fileSize);

  // read the data:
  vec.insert(vec.begin(),
             std::istream_iterator<BYTE>(file),
             std::istream_iterator<BYTE>());

https://stackoverflow.com/questions/3902609/how-do-i-count-the-zeros-and-ones-in-a-file
  uint64_t sum1 = 0;

  for (int i = 0; i < vec.size(); i++)
  {
    //sum0 += 8 - cheating[vec[i]];
    sum1 += cheating[vec[i]];
  }

  uint64_t sum0 = 8 * vec.size() - sum1;

  cout << sum0 << endl;
  cout << sum1 << endl;

  return 0;
}
