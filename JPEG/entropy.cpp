#include <iostream>
#include "entropy.h"
#include <cmath>


using namespace std;

int Entropy::getCapacity(int num)
{
	if(num < 0)
	num *= -1;
	
	int ans = 1;
	int cap = 0;
	while(cap < 100)
	{
		if(num < ans)
		return cap;
		else
		{
			ans = ans * 2;
			cap ++;
		}
	}
	
	return -1;
}
			
void Entropy::nextCoordinates(char &dir, int &x, int &y, int &sum)
{
	//finding the next coordinates in an 8 by 8 block in a zig zag manner
	
	//the case when we are going up
	if(dir == 'u')
	{
		x --;
		y ++;
		if(outOfBounds(x,y))
		{
			x ++;
			y --;
		
			if(sum > 7)
			{
				x++;
			}
		
			else 
		
			{	
				y++;
			}
		
			dir = 'd';
			sum ++;
		}
	}
	
	//the case when we are going down
	else
	{
		x++;
		y--;
		
		if(outOfBounds(x,y))
		{
			x--;
			y++;
		
		
			if(sum > 7)
			{
				y++;
			
			}
			else
			{
				x++;
			}
		
			dir = 'u';
			sum ++;
		}
	}
	
	return;
}

bool Entropy::outOfBounds(int x, int y)
{
	return (!(((x >= 0) && (x < 8)) && ((y >= 0) && (y < 8))));
}

void Entropy::blockRunLength(int startx, int starty, int **quant, vector<triple> &result, int &resSize)
{
	//RLE on a single block. There can at the most 15 0's contiguously. 
	int x, y, i, j, sum, count;
	char dir;
		
	dir = 'u';
	x = y = 0;
	count = 0;
	sum = 1;
	
	int numZeroes = 0;
	
	while(count < 64)
	{
		if(numZeroes == 15 || quant[x + startx][y + starty] != 0 || count == 63)
		{
			triple newTriple;
			newTriple.zeroes = numZeroes;
			newTriple.val = quant[x + startx][y + starty];
			newTriple.cat = getCapacity(newTriple.val);
			result.push_back(newTriple);
			resSize++;
			numZeroes = 0;
		}
		else
		numZeroes ++;
		count ++;
		nextCoordinates(dir, x, y, sum);
	}
	
	return;
}
	
void Entropy::runLength(int boundX, int boundY, int **quant, vector<triple> &result, int &resSize)
{
	int x = boundX / 8, y = boundY/8;
	
	//runs RLE on each block
	for(int i = 0; i < x; i++)
	{
		for(int j = 0; j < y; j++)
		{
			blockRunLength((i * 8), (j * 8), quant, result, resSize);
		}
	}
	
	return;
}
			

	
void Entropy::inverseRunLength(int boundX, int boundY, int **quant, vector<triple> &result)
{
	//standard JPEG inverse RLE
	int resIndex = 0;
	for(int startx = 0; startx < boundX; startx += 8)
	{
		for(int starty = 0; starty < boundY; starty += 8)
		{			
			int count = 0, num, ictr;
			int sum = 1, x = 0, y = 0;
			char dir = 'u';
			while(count < 64)
			{
		
				num = result[resIndex].zeroes;
		
				ictr = 0;
				for(;ictr < num; ictr++)
				{
					quant[x + startx][y + starty] = 0;
					nextCoordinates(dir, x, y, sum);
					count ++;
				}
		
				quant[x + startx][y + starty] = result[resIndex].val;
				nextCoordinates(dir, x, y, sum);
				count ++;
				
				resIndex++;
			}
		
		}
	}
}
		
void Entropy::print(int start, int end, vector<triple> &vctr)
{
	for(int i = start; i<= end; i++)
	{
		vctr[i].print();
	}

}

void Entropy::printArr(int cap, int **arr)
{
	for(int i = 0; i< cap; i++)
	{
		for(int j = 0; j< cap; j++)
		{
			cout << arr[i][j] << " " << i << " " << j << " d ";
		}
		
		cout << endl;
	}

}

void Entropy::printStringArr(int cap, string **arr)
{
	for(int i = 0; i< cap; i++)
	{
		for(int j = 0; j< cap; j++)
		{
			cout << arr[i][j] << " " << i << " " << j << "d ";
		}
		
		cout << endl;
	}

}

void Entropy::findFrequency(int **result, vector<triple> &data, int dataSize)
{
	for(int i = 0; i < dataSize; i++)
	{
		triple tr = data[i];
		result[tr.zeroes][tr.cat]++;
	}
	
	return;
}

void Entropy::convertToBinary(int num, string &result, int &resultSize)
{
	int resSize = 0;
	short int a;
	char ch;
	//standard in the case of a positive number
	if(num > 0)
	{
		while(num > 0)
		{
			a = num % 2;
			if(a == 0)
			result[resSize] = '0';
			else
			result[resSize] = '1';
			
			resSize ++;
			num /= 2;
		}
		
		for(int i = resSize - 1; i >= resSize / 2; i--)
		{
			ch = result[i];
			result[i] = result[(resSize - 1) - i];
			result[(resSize - 1) - i] = ch; 
		}
		
		resultSize = resSize;
			
	}
	
	//1's complement for a negative number
	else if(num < 0)
	{
		num *= -1;
		
		while(num > 0)
		{
			a = num % 2;
			if(a == 0)
			result[resSize] = '0';
			else
			result[resSize] = '1';
			
			resSize ++;
			num /= 2;
		}
		
		for(int i = resSize - 1; i >= resSize / 2; i--)
		{
			ch = result[i];
			result[i] = result[(resSize - 1) - i];
			result[(resSize - 1) - i] = ch; 
		}
	
		resultSize = resSize;
		int i = resSize - 1;
		resSize --;
		
		while(resSize >= 0)
		{
			if(result[resSize] == '0') result[resSize] = '1';
			else result[resSize] = '0';
			resSize --;
		}
	}
	
	else
	{
		resultSize = 0;
	}
	return;
}
		
void Entropy::huffmanInsert(int actualHeight, int actualWidth, int Height, int Width, vector<triple> &data, int dataSize, ofstream &outputFile)
{
	string str;
	str.reserve(20);
	int resultSize = 0;
	int **freq;
	freq = new int*[16];
	for(int i = 0; i < 16; i++)
	{
		freq[i] = new int[16];
	}
	
	for(int i = 0 ; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			freq[i][j] = 0;
		}
	}
	
	//create frequency histogram
	findFrequency(freq, data, dataSize);
	
	BinaryTree huffTree;
	//create huffman tree
	makeHuffmanTree(freq, huffTree);
	
	string begin = "";
	
	string **huffData;
	huffData = new string*[16];
	for(int i = 0; i < 16; i ++)
	{
		huffData[i] = new string[16];
	}
	
	//create array of maps to huffman values
	makeHuffList(huffData, begin, huffTree.start, huffTree);
	
	
	string treeString;
	treeString.reserve(2600);
	
	//make string from huffman tree for storage
	treeToString(treeString, huffTree.start, huffTree);
	
	//image dimensions inserted in file
	string first,second,third,fourth;
	sixteenBit(actualHeight, first);
	printToFile(first, 16, outputFile);
	sixteenBit(actualWidth, second);
	printToFile(second, 16, outputFile);
	sixteenBit(Height, third);
	printToFile(third, 16, outputFile);
	sixteenBit(Width, fourth);
	printToFile(fourth, 16, outputFile);
	
	//huffman tree inserted
	printToFile(treeString, treeString.length(), outputFile);
	
	string indv;
	indv.reserve(40);
	int indvSize = 0;
	string huffStr;
	
	//RLE data inserted
	for(int i = 0; i < dataSize; i++)
	{
		huffStr = huffData[data[i].zeroes][data[i].cat];
		printToFile(huffStr, huffStr.length(), outputFile);
		convertToBinary(data[i].val, indv, indvSize);
		printToFile(indv, indvSize, outputFile);
	}
	
	
	return;
}

void Entropy::stringPrint(string &str, int resultSize)
{
	for(int i = 0; i< resultSize; i++)
	{
		cout << str[i];
	}
	cout << endl;
	return;
}

void Entropy::makeHuffmanTree(int **freq, BinaryTree &tree)
{
	//make huffman tree using a standard priority queue, using probability of occurences
	priority_queue <huffPair> q;
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < 16; j++)
		{
			TreeNode *newNode;
			newNode = new TreeNode();
			newNode->value = i * 16 + j;
			newNode->left = NULL;
			newNode->right = NULL;
			huffPair newPair;
			newPair.node = newNode;
			newPair.freq = freq[i][j];
			q.push(newPair);
		}
	}
	
	huffPair p1;
	huffPair p2;
	
	while(q.size() > 1)
	{
		p1 = q.top();
		q.pop();
		p2 = q.top();
		q.pop();
		TreeNode *newNode;
		newNode = new TreeNode();
		newNode->value = -1;
		newNode->left = p1.node;
		newNode->right = p2.node;
		huffPair newPair;
		newPair.node = newNode;
		newPair.freq = p1.freq + p2.freq;
		q.push(newPair);	
	}
	
	huffPair p3 = q.top();
	q.pop();
	
	tree.start = p3.node;
}

void Entropy::makeHuffList(string **data, string current, TreeNode *node, BinaryTree &tree)
{
	if(tree.isLeaf(node))
	{
		int x = node->value / 16;
		int y = node->value % 16;
		
		data[x][y] = current;
	}
	
	else
	{
		string left = current, right = current;
		left.push_back('0');
		right.push_back('1');
		makeHuffList(data, left, node->left, tree);
		makeHuffList(data,right,node->right,tree);
	}
	
	return;
}

void Entropy::sixteenBit(int num, string &ans)
{
	string temp;
	temp.reserve(20);
	int tempSize;
	convertToBinary(num, temp, tempSize);
	
	for(int i = 0; i < 16 - tempSize; i++)
	{
		ans.push_back('0');
	}
	
	for(int i = 0; i < tempSize; i++)
	ans.push_back(temp[i]);
	
	return;
}

void Entropy::eightBit(int num, string &ans)
{
	string temp;
	temp.reserve(20);
	int tempSize;
	convertToBinary(num, temp, tempSize);
	
	for(int i = 0; i < 8 - tempSize; i++)
	{
		ans.push_back('0');
	}
	
	for(int i = 0; i < tempSize; i++)
	ans.push_back(temp[i]);
	
	return;
}

void Entropy::treeToString(string &ans, TreeNode *node, BinaryTree &tree)
{
	char p;
	if(node == NULL)//non existent case
	{
		ans.push_back('2');
	}
	else if(tree.isLeaf(node))//leaf node found
	{
		ans.push_back('1');
		string res;	
		eightBit(node->value, res);
		for(int i = 0; i < 8; i++)
		ans.push_back(res[i]);
	}
	else//non leaf node found
	{
		ans.push_back('0');
		treeToString(ans, node->left, tree);
		treeToString(ans, node->right, tree);
	}
	
	return;
}
	
inline void Entropy::printToFile(string &str, int num, ofstream &file)
{
	for(int i = 0; i < num; i++)
	{
		file << str[i];
	}
}

inline int Entropy::powerOfTwo(int n)
{
	int ans = 1;
	for(int i = 0; i < n; i++)
	ans *= 2;
	
	return ans;
}

void Entropy::nextNBits(int n, ifstream &inputFile, int &num)
{
	char ch;
	
	num = 0;
	for(int i = 0; i < n; i++)
	{
		inputFile >> ch;
		if(ch == '1')
		num += powerOfTwo((n - 1) - i);
	}
	
	
}

void Entropy::decodeToRLE(int &actualHeight, int &actualWidth, int &Height, int &Width, vector<triple> &rle, int &rleSize, ifstream &inputFile)
{
	//function to convert from file back to RLE data
	
	//extract image dimensions from file
	nextNBits(16, inputFile, actualHeight);
	nextNBits(16, inputFile, actualWidth);
	nextNBits(16, inputFile, Height);
	nextNBits(16, inputFile, Width);
	
	//extract the huffman tree used for encoding from file
	BinaryTree huffTree;
	buildHuffmanTreeFromFile(inputFile, huffTree);	
	
	int count = 0;

	//Read individual units of RLE data
	startRLE(actualHeight, actualWidth, count, inputFile, huffTree, rle, rleSize);

	return;  
}

void Entropy::buildHuffmanTreeFromFile(ifstream &inputFile, BinaryTree &tree)
{
	//main function to build Huffman Tree from file

	int count = 0;
	TreeNode *begin;
	begin = new TreeNode;
	int leaves = 0;
	buildHuffNode(begin, count, leaves, inputFile);
	
	tree.start = begin;
}

void Entropy::buildHuffNode(TreeNode *node, int &count, int &leaves, ifstream &inputFile)
{
	//Recursive function to build a Huffman Node from file
	char ch;
	
	
	inputFile >> ch;
	count ++;
	
	
	if(ch == '0')
	{
		TreeNode *left, *right;
		left = new TreeNode;
		right = new TreeNode;
		buildHuffNode(left, count, leaves, inputFile);
		buildHuffNode(right, count, leaves, inputFile);
		node->value = -1;
		node->left = left;
		node->right = right;
	}
	else if(ch == '1')
	{
		count += 8;
		leaves ++;
		int num;
		nextNBits(8, inputFile, num);
		node->value = num;
		node->left = NULL;
		node->right = NULL;
	}
	
	return;
}

void Entropy::makeRLE(int &totalPixels, int &count, ifstream &inputFile, TreeNode *node, BinaryTree &tree, vector<triple> &rle, int &rleSize)
{
	//Reading an RLE unit
	char ch;
	inputFile >> ch;
	count ++;
	
	if(ch == '0')
	node = node->left;
	else if(ch == '1')
	node = node->right;
	
	//node found, base case
	if(tree.isLeaf(node))
	{
		int num = node->value;
		int x = num / 16;
		int y = num % 16;
		int val = 0;
		nextNBits(y, inputFile, val);
		if(y > 0 && val < powerOfTwo(y - 1))
		{
			val = val - (powerOfTwo(y)) + 1;
		}
		triple newTriple;
		newTriple.zeroes = x;
		newTriple.cat = y;
		newTriple.val = val;
		rle.push_back(newTriple);
		rleSize++;
		totalPixels += 1;
		totalPixels += newTriple.zeroes;
		count += newTriple.cat;
	}
	
	else//recursive case for non leaf node
	{
		makeRLE(totalPixels, count, inputFile, node, tree, rle, rleSize);
	}
	
	return;
}

void Entropy::startRLE(int actualHeight, int actualWidth, int &count, ifstream &inputFile, BinaryTree &tree, vector<triple> &rle, int &rleSize)
{
	//convert back from file to complete RLE data
	int pixels = actualHeight * actualWidth;
	int pixelsDone = 0;

	while(pixelsDone < pixels)
	{
		makeRLE(pixelsDone, count, inputFile, tree.start, tree, rle, rleSize);
	}
	
	return;
}
	
//overloaded operator to check for equality of triples	
bool triple::operator==(const triple &t2)const
	{
		return ( ( (zeroes == t2.zeroes) && (cat == t2.cat)) && (val == t2.val));
	}

