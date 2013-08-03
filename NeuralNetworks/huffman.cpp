#include<list>
#include<algorithm>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<fstream>
#include<queue>
#include"file.h"
#include "huffman.h"
#include "declarations.h"
using namespace std;
//to display the huffman code of any value nicely
void display(list<bool> l){
	while(!l.empty()){
		cout<<l.front()<<" ";
		l.pop_front();
	}
	cout<<endl;
}
//global values
int flag=0;
int countBits=0;
int huff_countBits=0;
//destroy the mem on the heap
huffman_write::~huffman_write(){
			delete[] count;
			delete[] arr;
			delete[] huffmanCodes;
}

//writing the value on the arr and changing count correspondingly
void huffman_write::writeToFile(int index){
	arr[parr++]=index;
	count[index]++;
}

//allocate mem
int huffman_write::initLosslessCompression(int maxMem,int countsize){
	parr=0;
	arr=new int[maxMem];
	countSize=countsize;
	count=new int[countSize];
	for(int i=0;i<countSize;i++){
		count[i]=0;
	}
	huffmanCodes=new list<bool>[countSize];
}

//class required for debugging
class encaps{
	public:
	int index,count;
	void init(int ind,int c){index=ind; count=c;}
	bool operator<(const encaps& a) const{return count>a.count;}
};

//writing all the data to the outfile stream
void huffman_write::writeToPhysicalFile(outfile* out){
	for(int i=0;i<countSize;i++){
		if(count[i]!=0){
 		//pushing all single nodes to the pq
			pq.push(tree(i,count[i]));
		}
	}
	//taking the trees having minm wt, and pushing a tree with combined wt after merging them..
	while(pq.size()>1){
		tree t1=pq.top();
		pq.pop();
		tree t2=pq.top();
		pq.pop();
		tree t3=t1.merge(t2);
		pq.push(t3);
	}
	tree final=pq.top();
	pq.pop();
	//build the array which stores the huffman code of all the values
	final.writeInArray(huffmanCodes);
	//write the tree to the outfile
	final.writeTreeToFile(out);
	//tree's work done,,deallocate
	final.deallocateMem();
	/* previous method of writing to file by value.commented out
	int bit_size=ceil(log2(countSize));
	int bit_size_size=pow(2,ceil(log2(bit_size)));
	int new_bit_size=pow(2,bit_size);
	for(int i=0;i<countSize;i++){
		int size=huffmanCodes[i].size();
		if(size<bit_size){
			out->writeToFile(0,1);
			out->writeToFile(size,bit_size_size);
			countBits+=5;
			huff_countBits+=5;
		}
		else{
			out->writeToFile(1,1);
			out->writeToFile(size-1,10);
			//out<<"here also :p\n";
			countBits+=11;
			huff_countBits+=11;
		}
		countBits+=size;
		huff_countBits+=size;
		for(list<bool>::iterator it=huffmanCodes[i].begin();it!=huffmanCodes[i].end();it++){
			out->writeToFile(*it,1);
		}
	}*/
	int rleCountMax=pow(2,run_len) ;
	int rleCount=-1;//rle init
	int prev_val=arr[0];

 // do simple rle,, and when you need to write a value, use the array of huffman value of each value that we had built
	for(int i=0;i<parr;i++){
		if(arr[i]==prev_val && rleCount<rleCountMax-1){
			rleCount++;
		}
		else {
			out->writeToFile(rleCount,run_len);
		 for(list<bool>::iterator it=huffmanCodes[prev_val].begin();it!=huffmanCodes[prev_val].end();it++){
			 out->writeToFile(*it,1);
		 }
			prev_val=arr[i];
			rleCount=0; 
		}
	}
	out->writeToFile(rleCount,run_len);
	for(list<bool>::iterator it=huffmanCodes[prev_val].begin();it!=huffmanCodes[prev_val].end();it++){
			 out->writeToFile(*it,1);
	}
//	routine to see sorted distribution of chars in sorted order..comented out..
/*
//	encaps* encap=new encaps[countSize];
//	for(int i=0;i<countSize;i++){
		encap[i].init(i,count[i]);
	}
//	sort(encap,encap+countSize);
	float a=0;
	if(flag==0 && encap[0].count==161){
	//	flag=1;
		//flag=1;
		for(int i=0;i<countSize;i++){
			a=(encap[i].count*1.0)/parr;
//			cout<<a<<endl;
		//	cout<<a<<"        ";
		//	display(huffmanCodes[encap[i].index]);
		}
		//cout<<" Ha ha "<<counter<<" "<<otherC<<endl;
	}*/ 
}

void huffman_read::readHuffmanTable(ifstream& inStream,infile* in,int NUM_NEURON){
	//Read HUFFMAN ABLE
	myTree->readHuffmanTable(inStream,in);

	//previous method of writing using arrays...commented out
	/*
	for(int i=0;i<NUM_NEURON;i++){
		int size_bit=in->readFromFile(inStream,1);
		int size;
		if(size_bit==0)
			size=in->readFromFile(inStream,4);
		else size=in->readFromFile(inStream,10)+1;
		//the add 1 is for considering a length of 1024.. :p  :p :p
		list<bool> l;
		while(size-->0){
			l.push_back(in->readFromFile(inStream,1));
		}
		makeTree(i,l);
	}*/
}
int huffman_read::readFromFile(ifstream& inStream,infile* in){
	//read individual values from the files
	if(counter==-1){
		counter=in->readFromFile(inStream,run_len_encoder);
	 curr_value=myTree->readValue(inStream,in);
	}
	counter--;
	return curr_value;
}


