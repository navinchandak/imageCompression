#ifndef file_h
#define  file_h
#include <iostream>
#include<vector>
#include<fstream>
#include<stack>
#include<bitset>
using namespace std;

//outfile is a stream which can store the values, given in some number of bits 
class outfile{
	int index,value;
	//file contains all the values fed to it till now
	//it needs to store because all the outputting has to be done at the end because of multithreading
	string file;
	int p;
	public:
	//const
	outfile();
	//clears the stream..padding with zeroes
	void clear();
	//accepts a value and the number_of_bits that it needs to be stored in
	void writeToFile(int val,int bits);
	//accepts a real file onto which it can write all the content
	void writeToPhysicalFile(ofstream& f);
};
class infile{
	int index;
	bitset<8> value;
	public:
	//clear to remove padded zeroes
	void clear();
	//const
	infile();
	//reading 'bits'number of bits from the input real file
	int readFromFile(ifstream& in,int bits);
};

#endif
