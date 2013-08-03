#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <queue>
#include "BinaryTree.h"

using namespace std;

#ifndef ENTROPY
#define ENTROPY

//the structure which stores the run length encoded triple
struct triple{
	short int zeroes;
	short int cat;
	int val;
	
	void print()
	{
		cout << zeroes << " " << cat << " " << val << endl;
	}
	
	bool operator==(const triple &t2)const;
};

//structure which stores the node along with its priority value for creating the huffman tree
struct huffPair{
	int freq;
	TreeNode *node;
	
	bool operator<(const huffPair &h2)const
	{
		return(freq > h2.freq);
	}
};

class Entropy{
	private:
	//find the next coordinates in a zig zag fashion
	void nextCoordinates(char &d, int &x, int &y, int &sum);
	//checks whether the coordinates are out of bounds of an 8 by 8 matrix
	bool outOfBounds(int x, int y);
	//returns the number of bits required to store the absolute value of num
	int getCapacity(int num);
	//find the frequency histogram of a pair of 4 bit numbers 
	void findFrequency(int **result, vector<triple> &data, int dataSize); 
	//read the next n bits from a file, convert to binary, and store the result in a number
	void nextNBits(int n, ifstream &inputFile, int &num);

	
	public:
	//Perform Run Length Encoding on a simple eight by eight block
	void blockRunLength(int startx, int starty, int **quant, vector<triple> &result, int &resSize);
	//Perform Run Length Encoding
	void runLength(int boundX, int boundY, int **quant, vector<triple> &result, int &resSize);
	//Perform inverse run length encoding
	void inverseRunLength(int boundX, int boundY, int **quant, vector<triple> &result);
	//coordinating function to create the huffman tree from rle data, and make the compressed temporary file
	void huffmanInsert(int actualHeight, int actualWidth, int Height, int Width, vector<triple> &data, int dataSize, ofstream &outputFile);
	//to find the binary equivalent of a number
	void convertToBinary(int num, string &result, int &resultSize);
	//to make the best huffman table for a given frequency histogram
	void makeHuffmanTree(int **freq, BinaryTree &tree);
	//to map each possible combination's value to its huffman code in a simple array
	void makeHuffList(string **data, string current, TreeNode *node, BinaryTree &tree);
	//to print the rle data
	void print(int start, int end, vector <triple> &vctr);
	//to print an array
	void printArr(int cap, int **arr);
	//to print a string array
	void printStringArr(int cap, string **arr);
	//to print a string upto a given size
	void stringPrint(string &str, int resultSize);
	//to find the sixteen bit representation of a number
	void sixteenBit(int num, string &ans);
	//to find the eight bit representation of a number
	void eightBit(int num, string &ans);
	//To convert a huffman tree into a string which can be stored in a file
	void treeToString(string &ans, TreeNode *node, BinaryTree &tree);
	//To decode back to RLE data from file, this is the coordinating function
	void decodeToRLE(int &actualHeight, int &actualWidth, int &Height, int &Width, vector<triple> &rle, int &rleSize, ifstream &inputFile);
	//To rebuild the Huffman Tree from file
	void buildHuffmanTreeFromFile(ifstream &inputFile, BinaryTree &tree);
	//To assist in building the huffman tree from file
	void buildHuffNode(TreeNode *node, int &count, int &leaves, ifstream &inputFile);
	//To Make a single rle unit from file, given the huffman tree
	void makeRLE(int &totalPixels, int &count, ifstream &inputFile, TreeNode *node, BinaryTree &tree, vector<triple> &rle, int &rleSize);
	//To remake the entire rle data from file, given the Huffman Tree
	void startRLE(int actualHeight, int actualWidth, int &count, ifstream &inputFile, BinaryTree &tree, vector<triple> &rle, int &rleSize);
	//inline function a given number of bits to file
	inline void printToFile(string &str, int num, ofstream &file);
	//inline funtion to find 2 raised to some number
	inline int powerOfTwo(int n);




};




#endif
