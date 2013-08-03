#include <iostream>
#include <algorithm>
#include<list>
#include<thread>
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

double alpha;
int imageWidth,imageHeight,x_step,y_step,NUM_NEURON,num_of_iterations;
int num_blocks;
double delta;
int num_bits;
ofstream imageIndex,rle;
outfile *out;
int run_len;
int x_part,y_part;
int minIndex(neuron* KohenenArray,double* in,int NUM_NEURON);
void segmentFunc(int tCount,int x_p,int y_p,CImg<unsigned char>& sourceImage);
void compress(char* imageSource){
	cout<<"Encrypting "<<imageSource<<".. "<<endl;
	cout<<"Processing percent completed:- \n";
	//opening the image
	CImg<unsigned char> sourceImage(imageSource);
	imageHeight = sourceImage.height();
	imageWidth = sourceImage.width();
	int imageDepth = sourceImage.depth();
	int imageChannels = sourceImage.spectrum();
	//debug file
	imageIndex.open("image.txt");
	rle.open("rle.txt");
	//value of constants , which decide the performance of the image compression
	alpha=0.7;
	num_bits=10;
	num_of_iterations=1;
	delta=0.12;
	run_len=2;
	NUM_NEURON=pow(2,num_bits);
	//outfile is a stream which handles bit by bit addition into the file..initializing that to write parameters onto the file
	outfile* params=new outfile;
//	params->init(300);
	params->writeToFile(imageWidth,20);
	params->writeToFile(imageHeight,20);
	params->writeToFile(num_bits,20); // 10 bits for our purpose
	params->writeToFile(run_len,20); //number of bits for the count
	params->clear();
	// the algo starts here 
	// x_part decides how many blocks the image can be decomposed into in the x_dir. similar for y_part
	x_part=imageWidth/300+1;
	x_step=imageWidth/x_part+1;
	y_part=imageHeight/300+1;
	y_step=imageHeight/y_part+1;
	thread threads[4];
	num_blocks=x_part*y_part;
	//allocating mem for the outfiles of the blocks
	//they are being parallely processed, so need to store the info somewhere and write to file at the end
	out=new outfile[num_blocks];
	int tcount=0;
	for(int i=0;i<4;i++){
		// the segment function runs the compression of about 1/4th of the blocks. which 1/4th is decided by the value of i passed to it..
		// this is to nesure that too much memory doesnt get consumed bacause the same thing opens in many threads
		threads[i]=thread(segmentFunc,i,x_part,y_part,ref(sourceImage));
	}
	for(int i=0;i<4;i++){
		threads[i].join();
	}
	cout<<"100%\nCompression done Successfully\n";
	//Compression done
	// Saving business starts
	ofstream file;
	string forMod=imageSource;
	forMod=forMod.substr(0,forMod.find_first_of('.')+1).append("bin");//converting xyz.jpg to xyz.bin
	file.open(forMod.c_str(),ios::binary);
	params->writeToPhysicalFile(file);
	cout<<"Saving the compressed file\n";
	for(int i=0;i<num_blocks;i++){
		out[i].writeToPhysicalFile(file);
	}
//	delete[] out;
	cout<<"Saved as "<<forMod<<endl;
	cout<<"Done\n";
//	cout<<"Number of bits taken ="<<countBits;
//	cout<<"\nhuff_bit="<<huff_countBits<<endl;
	file.close();
}

//a simple function to find which representative neuron is closest to the current pixel
int minIndex(neuron* KohenenArray,double* in,int NUM_NEURON){
	double min=distance(KohenenArray[0].w,in,3); int minIndex=0;
	for(int i=0;i<NUM_NEURON;i++){
		if(distance(KohenenArray[i].w,in,3)<min){
			min=distance(KohenenArray[i].w,in,3);
			minIndex=i;
		}
	}
	return minIndex;
}

//some function required for displaying percentages //see below for further details
int roundCube(int divisor,int N){
	N=(N+1)/2; N*=2;
	while(N%divisor!=0) divisor--;
	return divisor;
}
//declaration
void blockFunc(int tCount,int x_p,int y_p,CImg<unsigned char>& sourceImage);

//function which handles 1/4th of the blocks one at a time
void segmentFunc(int i,int x_part,int y_part,CImg<unsigned char>& sourceImage){	
//	cout<<"x_part and y_part "<<x_part<<" "<<y_part<<endl;
	int t=ceil((x_part*y_part*1.0)/4);
	int st=i*t;
	int end=min( (i+1)*t , x_part*y_part);
	int temp=0;
//	cout<<t<<"seg "<<i<<" "<<"st 8 end: "<<st<<" "<<end<<endl;
	for(int x_p=0 ; x_p< x_part  ;x_p++){
		for(int y_p= 0 ; y_p< y_part ;y_p++){
			if(temp>=st && temp<end){
			blockFunc(x_p*y_part + y_p,x_p,y_p,sourceImage);
			}
			temp++;
			// 	segmentFunc(x_p,y_p,sourceImage);
		}
	}
}

//compressing a block and saving it in the outstream so that that can be written to a file later
void blockFunc(int tCount,int x_p,int y_p,CImg<unsigned char>& sourceImage){
//	cout<<"block func called \n";
	double in[3];
	//lx lower limit of x
	int lx=x_step*x_p;
	//hx higher limit of x - 1
	int hx=min(imageWidth,lx+x_step);
	//similar semantics for ly
	int ly=y_step*y_p;
	//similar semantics for hy
	int hy=min(imageHeight,ly+y_step);
	//segH and segW -- segment size of x and y
	int segH=hy-ly;
	int segW=hx-lx;
	//~cout<<lx<<" "<<ly<<" "<<hx<<" "<<hy<<endl;
	//allocating space to 1024 representative vectors
	neuron* KohenenArray=new neuron[NUM_NEURON];
	//~debugger<<"alpha "<<alpha<<endl;

	//initializing the representative vectors randomly
	for(int ind=0;ind<NUM_NEURON;ind++){
		int a=rand()%(segH*segW);
		int imgW=a%segW+lx;
		int imgH=a/segW+ly;
		for(int i=0;i<3;i++){
			in[i]=sourceImage(imgW,imgH,0,i);
			//debugger<<imgW<<" "<<imgH<<endl;
		}
		KohenenArray[ind].initialize(in);
	}
	double tempAlpha=alpha;
	int workDone=0;
	//cout<<"num_iter "<<num_of_iterations<<endl;
 int NUM_ITER;
	//Routine to traverse across the pixels in the block and move the closes neuron towards itself
	for(NUM_ITER=0;NUM_ITER<num_of_iterations;NUM_ITER++){
		for(int imgW=lx;imgW<hx;imgW++){
			
			if(tCount< ceil((x_part*y_part*1.0)/4) && (imgW-lx)%( int(ceil(x_part*y_part*1.0/4) * ceil((segW/100.0)*(num_of_iterations+1)*(roundCube(((floor(360.0/num_blocks)/(num_of_iterations+1))),ceil(100.0/(num_of_iterations+1)))))))==0){
				//routine to print the percentage of work done till now \m/
		cout<<int(  (100.0 *tCount)/ceil(x_part*y_part*1.0/4) + (100.0*(NUM_ITER/(1.0+num_of_iterations)) + (100.0/(num_of_iterations+1.0))* (((imgW-lx)*1.0)/segW))/ceil(x_part*y_part*1.0/4) ) <<"%"<<endl;
			}
			for(int imgH=ly;imgH<hy;imgH++){
				//debugger<<imgW<<" "<<imgH<<"   ";
				for(int i=0;i<3;i++){
					in[i]=sourceImage(imgW,imgH,0,i);
				}
				//minI is the index of the winning neuron
				int minI=minIndex(KohenenArray,in,NUM_NEURON);
				//the following pulls the winning neuron towards the pixel..i.e the weight of winning neuron tend to favour the curr pixel
				KohenenArray[minI].approximate(in,tempAlpha);
			}
			//	debugger<<endl;
		}
		tempAlpha-=(alpha-0.1)/num_of_iterations;//changing the value of alpha in the next iteration
	}

	//writing the weights of the neurons onto a file
	for(int i=0;i<NUM_NEURON;i++){
		for(int j=0;j<3;j++){
			out[tCount].writeToFile(int(KohenenArray[i].w[j]),8);
			countBits+=8;
		}
	}

	//debugger<<" is a good boy "<<endl;
	//opening the huffman and rle stream
	huffman_write huff;
	//giving the max memory that can be consumed and the size of the count vector
	//i.e the size of the set of values that is going to be inputted
	//it is imp that set contain values only in between 0 to |set|-1
	huff.initLosslessCompression(segW*segH, NUM_NEURON );
	int index;
	for(int imgW=lx;imgW<hx;imgW++){
		for(int imgH=ly;imgH<hy;imgH++){
			for(int i=0;i<3;i++){
				in[i]=sourceImage(imgW,imgH,0,i);
			}
			index=minIndex(KohenenArray,in,NUM_NEURON);
			//giving the index of the winning neuron to the huffman object
			huff.writeToFile(index);
		}
			if(tCount< ceil((x_part*y_part*1.0)/4) && (imgW-lx)%( int(ceil(x_part*y_part*1.0/4) * ceil((segW/100.0)*(num_of_iterations+1)*(roundCube(((floor(360.0/num_blocks)/(num_of_iterations+1))),ceil(100.0/(num_of_iterations+1)))))))==0){
		cout<<int(  (100.0 *tCount)/ceil(x_part*y_part*1.0/4) + (100.0*(num_of_iterations/(1.0+num_of_iterations)) + (100.0/(num_of_iterations+1.0))* (((imgW-lx)*1.0)/segW))/ceil(x_part*y_part*1.0/4) ) <<"%"<<endl;
	//~	earlier//cout<<tCount<<" "<<int( 100*(num_of_iterations/(1.0+num_of_iterations)) + (100.0/(num_of_iterations+1.0))* ((imgW-lx)*1.0)/segW )<<"%"<<endl;
		} 
	}
	//asking the huffman object to write the entire data onto outfile object, the outfile object handles stuff of any number of bits
	huff.writeToPhysicalFile(&out[tCount]);
	out[tCount].clear();//padding the end with 0s to complete full byt

}

