#include <fstream>
#include <iostream>

using namespace std;

#ifndef FILECONVERT
#define FILECONVERT	

class FileConvert{

public:
//reduce the size of a file by eight by converting the successive eight bits to its character ascii
void convertToFile(char *inpFile, char *optFile);
//recreate the original stream of bits from characters
void convertFromFile(char *inpFile, char *optFile);
//convert a number to binary
void convertToBinary(int num, string &result, int &resultSize);
//read the next eight bits
void eightBit(int num, string &ans);
//to find the power of a number raised to 2
inline int powerOfTwo(int n);
};

#endif
