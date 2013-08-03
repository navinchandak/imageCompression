#include <iostream>
#include<fstream>
#include<stack>
#include "file.h"
#include "declarations.h" 
using namespace std;

//const
outfile::outfile(){
	index=0;
	value=0;
	p=0;
}
//const
infile::infile(){
	index=-1;
	value=0;
}
//clear to pad the outfile with zeroes if the number_of_bits given till now is not sufficient to complete bytes(not a multiple of 8)
void outfile::clear(){
	if(index==0) return;
	writeToFile(0,8-index);
}
//just write the string to real file
void outfile::writeToPhysicalFile(ofstream& f){
	//debugger<<p<<endl;
	for(int i=0;i<p;i++){
		//debugger<<int(file[i])<<" ";
	}
//	debugger<<endl
	f.write(file.c_str(),file.size());
}

	//accepts a value and the number_of_bits that it needs to be stored in
void outfile::writeToFile(int val,int bits){
	while( bits>0 ){
	//	cout<<bits<<endl;
		value=value*2+(val%2);
		val/=2;
		bits--;
		index++;
		//index is just a counter which keeps increasing its value and reads a byte from thre real file when it becomes 8
		if(index==8){
   //cout<<value<<endl;
			file.append(1,char(value));
			value=0;
			index=0;
		}
		
	}
}
//just to start reading from the same input stream after discaring all progress til now
void infile::clear(){
	index=-1;
	//cout<<endl;
}
//implemented using a stack because of the way it was written
int infile::readFromFile(ifstream& file,int bits){
	char c;
	int temp; 
	bool flag=0;
	stack<bool> s;
	//while we have not read a fixed number of bits just keep repeating the loop
	while(bits>0){
		//when index becomes 1, a byte needs to be read to give the entire value in return
		if(index==-1){
			file.read(&c,1);
			//cout<<int(c)<<" ";
			value=c;
			index=7;
		}
		//stack just collects the 0s and 1s,, value is a bitset
		s.push(value[index--]);
		bits--;
	}
	int val=0;
	//so necessary operations on stack to get the value
	while(!s.empty()){
		val*=2;
		val+=s.top();
		s.pop();
	}
	return val;
}
