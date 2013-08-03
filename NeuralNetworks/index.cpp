#include <iostream>
#include<bitset>
#include <cmath>
#include <fstream>
#include <string>
#include "vectorf.h"
#include "neuron.h"
#include "file.h"
#include "declarations.h"
using namespace std;

#include "CImg.h"
using namespace cimg_library;

ofstream debugger;
/* Here is how the saving business works .. A *.jpg file given to the program like this : ./a.out c xyz.jpg makes a xyz.bin. A *.bin given to the program like this : ./a.out d xyz.bin makes a xyz.tiff(raw format) from bin.. */
int main(int argc,char* argv[]){
	debugger.open("debug.txt");
	char *imageSource;
	char *option;
	option=argv[1];
	// The code below just initializes the file names , and bird.jpg is the default when no name is given
	if(argc==1){
		//Note to be displayed when the user doesnt know how to handle the executable
		cout<<"The correct syntax is \n\t"<<argv[0]<<" <c/d> <name_of_file> \nwhere c is for compression and d is for decompression\n";
		 return 1;
	}
	if(argc==2){
		if(argv[1][0]=='c'|| argv[1][0]=='a'){
			imageSource=(char*)"bird.jpg";
		}
		else imageSource = (char*)"bird.bin";
	}
	else if(argc==3) {
		 imageSource = argv[2];
	}
 debugger<<" Process   "<<option<<endl;
	debugger<<" File name "<<imageSource<<endl;
	if(option[0]=='c' || option[0]=='a'){
		//a stands for all, i.e compression and decompression
  compress(imageSource);
	}
	if(option[0]=='a'){
		//if the option is a, then *.jpg name has to converted to *.bin, to be given to decompress function
	 string forMod;
	 forMod=imageSource;
	 forMod=forMod.substr(0,forMod.find_first_of('.')+1).append("bin");
		imageSource=new char[forMod.size()+1];
		strncpy( imageSource, forMod.c_str(), forMod.size() );
		imageSource[forMod.size()]='\0';
 }
	if(option[0]=='d'||option[0]=='a'){
		//decompress function called when option is d or a
		decompress(imageSource);
	}
}
