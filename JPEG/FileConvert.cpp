#include <fstream>
#include <iostream>
#include "FileConvert.h"
#include <string>

using namespace std;

void FileConvert::convertToFile(char *inpFile, char *optFile)
{
	ifstream input;
	input.open(inpFile);
	
	ofstream output;
	output.open(optFile);
	
	char ch, ent;
	int num = 0, i;
	
	if (input.is_open() == false)
	return;
	
	while(1)
	{
		num = 0;
		
		//converting next eight bits to number
		for(i = 7; i >= 0; i--)
		{
			//end of file condition
			if(input.eof())
			{	
				ent = (num - 128);
				output.write(&ent , 1);
				output.close();
				return;
			}
			
			input >> ch;
			if(ch == '1')
			num += powerOfTwo(i);
		}
		
		//writing to file
		ent = (num - 128);
		output.write(&ent, 1);
	}
	
	return;
}

void FileConvert::convertFromFile(char *inpFile, char *optFile)
{
	fstream input;
	input.open(inpFile);
	
	ofstream output;
	output.open(optFile);
	
	char ch;
	int num = 0;
	string res;
	res.reserve(20);
	int resSize;
	
	
	while(input.good())
	{
		//Reading the next character and converting to bits
		input.read(&ch, 1);

		eightBit((ch + 128), res);
		for(int i = 0; i < 8; i++)
		{
			if(res[i] == '0')
			output << '0';
			else
			output << '1';
		}
	}
	
	output.close();
}
			

void FileConvert::convertToBinary(int num, string &result, int &resultSize)
{
	int resSize = 0;
	short int a;
	char ch;
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
	
void FileConvert::eightBit(int num, string &ans)
{
	string temp;
	temp.reserve(20);
	int tempSize;
	convertToBinary(num, temp, tempSize);
	ans.erase();
	
	for(int i = 0; i < 8 - tempSize; i++)
	{
		ans.push_back('0');
	}
	
	for(int i = 0; i < tempSize; i++)
	ans.push_back(temp[i]);
	
	return;
}

inline int FileConvert::powerOfTwo(int n)
{
	int ans = 1;
	for(int i = 0; i < n; i++)
	ans *= 2;
	
	return ans;
}
