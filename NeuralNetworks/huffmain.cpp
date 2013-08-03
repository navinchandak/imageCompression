#include<list>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<fstream>
#include<queue>
#include "huffman.h"
#include "file.h"
using namespace std;
int main(int argc,char* arv[]){
	if(argc==1) cout<<"foooINsuff args";
	ifstream ifile(arv[1]);
	ifile.seekg(0,ios::end);
	int len=ifile.tellg();
	cout<<"Len= "<<len<<endl;
	ifile.seekg(0,ios::beg);
	huffman_write a;
	cout<<len<<endl;
	a.initLosslessCompression(len,256);
	cout<<" init done \n";
	char c;
	for(int i=0;i<len;i++){
		ifile.read(&c,1);
		a.writeToFile((int)c+128);
	}
	cout<<"Writing to file done \n";
	outfile out;
	out.writeToFile(len,30);
	a.writeToPhysicalFile(&out);
	out.clear();
	ofstream ofile; 
	ofile.open("comp.bin");
	out.writeToPhysicalFile(ofile);
	ofile.close();
	cout<<" Done \n";


	ifstream inStream;
	inStream.open("comp.bin");
	ofstream decompFile;
	decompFile.open("decomp.txt");
	huffman_read read;
	infile inFile; 
	int length=inFile.readFromFile(inStream,30);
	read.readHuffmanTable(inStream,&inFile,256);
	for(int i=0;i<length;i++){
  c=char(read.readFromFile(inStream,&inFile) - 128);
		decompFile.write(&c,1);
	}
	inFile.clear();	
	decompFile.close();
}
