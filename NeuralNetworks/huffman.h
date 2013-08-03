#include<list>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<fstream>
#include<queue>
#include "file.h"
#include "tree.h"
using namespace std;
/* huffman write is a class which takes in all teh values that need to be written to the file,, it calculates the distribution of the input and then decides the huffman codes.. once done, we can ask it to write everything to a file in the correct format which can be read back meaningfully*/
class huffman_write{
	public:
	//count is to store the distribution
	//arr is to store the elements
		int*count; int* arr;
		list<bool>* huffmanCodes; //it stores the huffman codes of all the values in the array.. the huffman code is stored in the form of a list
		priority_queue<tree> pq; //this is a pq which helps to make th etree for generating huffman codes
		int countSize; //size of the set of values
		int parr; // position of the last element filled in arr
		void writeToFile(int index); // to input a value
		//to alllocate emmory to count and arr
		int initLosslessCompression(int maxMem,int countsize);
		//to write everything to the outfile stream, which supports bit-by-bit writing..
		void writeToPhysicalFile(outfile* out);
		//destructor
		~huffman_write();
};
/* huffman_read is a class which can read the formatted data about the huffman table from a file, and can keep on telling us the next value that was stored in the file with the help of the tree that it makes*/ 
class huffman_read{
	public:
	 int counter; //RLE
		int curr_value; //RLE
		tree* myTree; //the tree that it makes
		huffman_read(){
			counter=-1; //the counter stores what is the number of times the curr value appeared in the file
			myTree=new tree(-1,0); //init the tree
		}
		// reading the huffman table form the instream with the help of a infile
		void readHuffmanTable(ifstream&,infile* in,int NUM_NEURON);
		//reading individual values from the file
  int readFromFile(ifstream&,infile* in);
};
