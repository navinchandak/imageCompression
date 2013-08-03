#include <iostream>
#include<bitset>
#include <cmath>
#include <fstream>
#include <string>
#include "vectorf.h"
#include "neuron.h"
#include "file.h"
#include "declarations.h"
#include "huffman.h"
using namespace std;

#include "CImg.h"
using namespace cimg_library;

int run_len_encoder;
//decompression function
void decompress(char* compressed){
	ifstream file;
	file.open(compressed,ios::binary);
	if(file==NULL){
		//if bin file is not there
		cout<<"invalid file name "; return;
	}
	cout<<"Decompression of "<<compressed<<" started..\n";
	infile in;
	int NUM_NEURON,imageWidth,imageHeight,imgW,imgH,num_bits;
	//reading necessary parameters of the image
	imageWidth=in.readFromFile(file,20);
	imageHeight=in.readFromFile(file,20);
	num_bits=in.readFromFile(file,20);
	run_len_encoder=in.readFromFile(file,20);
	in.clear();
	NUM_NEURON=pow(2,num_bits);
	//opening a blank image using CImg
	CImg<unsigned char> sourceImage(imageWidth,imageHeight,1,3); 
//	CImg<unsigned char> test("lena.tiff"); 
	int **arr;
	arr=new int*[NUM_NEURON];//mem for saving the weights of the neurons, which can be used to decompress
	for(int i=0;i<NUM_NEURON;i++){arr[i]=new int[3];}
	debugger<<"params::num _neuron w h "<<NUM_NEURON<<" "<<imageWidth<<" "<<imageHeight<<endl;
 //deciding the number of blocks into the original image was decomposed
	int x_part=imageWidth/300+1;
	int x_step=imageWidth/x_part+1;
	int y_part=imageHeight/300+1;
	int y_step=imageHeight/y_part+1;
 
	for(int x_p=0;x_p<x_part;x_p++){
		for(int y_p=0;y_p<y_part;y_p++){
			//deciding the parameters for the current block
			int lx=x_step*x_p;
			int hx=min(imageWidth,lx+x_step);
			int ly=y_step*y_p;
			int hy=min(imageHeight,ly+y_step);
			int segH=hy-ly;
			int segW=hx-lx;
//			cout<<lx<<" "<<ly<<" "<<hx<<" "<<hy<<endl;
			for(int i=0;i<NUM_NEURON;i++){
				for(int j=0;j<3;j++){
					arr[i][j]=in.readFromFile(file,8);
					//reading the neuron weights from the file
				}
			}
		huffman_read huff;
		//ask the huff object to read the HuffmanTable data from the file and make its tree and stuff
		huff.readHuffmanTable(file,&in,NUM_NEURON);

			int index,count,count_bar;
			int temp;
			count=0;
			count_bar=-1;
			index=0;
			for(int i=lx;i<hx;i++){
				for(int j=ly;j<hy;j++){

					index=huff.readFromFile(file,&in); //read a pixel index from the file
				 //match it to the corresponding same-index-neuron weight read earlier from file
					for(int k=0;k<3;k++){
						temp=arr[index][k];
      //assign the required value to the pixel
						sourceImage(i,j,0,k)=temp; 					
					}
				}
			}
			
			in.clear();//just some generic routine
		}
	}
	string forMod=compressed;
	for(int i=0;i<NUM_NEURON;i++){delete[] arr[i];}
	delete[] arr;
	forMod=forMod.substr(0,forMod.find_first_of('.')).append("_neu.bmp"); //saving the decompressed image as tiff image
	sourceImage.save(forMod.c_str());
	cout<<"Decompressed file saved as "<<forMod<<endl;
	file.close();
}
