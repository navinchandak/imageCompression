#include <iostream>
#include <fstream>
#include <stdio.h>
#include "CImg.h"
#include "JPEG_Compression.h"
#include "entropy.h"
#include "FileConvert.h"
using namespace std;
using namespace cimg_library;


void getUserInput(string& imageSource);
void compress();
void deCompress();

void getUserInput(string& imageSource)
{
	cout << "Please enter file name:" << endl;
	cin >> imageSource;
	
	return;
}


int main() //the main function: starting point of execution
{
	cout << "Enter c for compression, d for decompression" << endl;
	char ch;
	cin >> ch;
	if(ch == 'c')
	compress();
	else if(ch == 'd')
	deCompress();
	
	return 0;
}
void compress() //When one wants to perform compression
{
	char *fileName;
	fileName = new char[100];
	cout << "Please enter name of file to be compressed:" << endl;
	cin >> fileName;//the name of file to be compressed
	
	CImg<unsigned char> sourceImage(fileName);//the image to be compressed
	
	int imageHeight, imageWidth, imageDepth;
	//actual image dimensions
	int Height = sourceImage.height();
	int Width = sourceImage.width();
	int Depth = sourceImage.depth();
	
	//padded image dimensions
	imageHeight = Height;
	imageWidth = Width;
	imageDepth = Depth;
	
	JPEG Compression;
	
	//padding
	if (Height % 16 != 0)
	{
		int temp = imageHeight / 16;
		temp ++;
		temp *= 16;
		imageHeight = temp;
	}
	
	if (Width % 16 != 0)
	{
		int temp = imageWidth / 16;
		temp ++;
		temp *= 16;
		imageWidth = temp;
	}
	
	
	//the arrays storing pixels after space transformation
	short int **pixelsY;
	pixelsY = new short int * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		pixelsY[i] = new short int[imageWidth];
	
	short int **pixelsCb;
	pixelsCb = new short int * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		pixelsCb[i] = new short int[imageWidth];
		
	short int **pixelsCr;
	pixelsCr = new short int * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		pixelsCr[i] = new short int[imageWidth];


	//the arrays after discrete cosine transform
    double **dctY;
	dctY = new double * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		dctY[i] = new double[imageWidth];
	
	double **dctCb;
	dctCb = new double * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		dctCb[i] = new double[imageWidth];
		
	double **dctCr;
	dctCr = new double * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		dctCr[i] = new double[imageWidth];	
		
	//the arrays after quantization	
	int **quantY;
	quantY = new int * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		quantY[i] = new int[imageWidth];

	int **quantCb;
	quantCb = new int * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		quantCb[i] = new int[imageWidth];	
	
	int **quantCr;
	quantCr = new int * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		quantCr[i] = new int[imageWidth];
	
	//Conversion of image from RGB to YCbCr Space
	Compression.convertToNewColourSpace(pixelsY, pixelsCb, pixelsCr, imageHeight, imageWidth, imageDepth, sourceImage);
	
	//Dicrete Cosine Transformation
	Compression.discreteCosineTransform(pixelsY, pixelsCb, pixelsCr, dctY, dctCb, dctCr, imageHeight, imageWidth, imageDepth);
	
	//deleting unrequired memory
	for(int i=0;i<imageHeight ;i++){
		delete[] pixelsY[i];
		delete[] pixelsCb[i];
		delete[] pixelsCr[i];
	}
	delete[] pixelsY;
	delete[] pixelsCb;
	delete[] pixelsCr;
	
	//Quantization
	Compression.quantization(quantY, quantCb, quantCr, dctY, dctCb, dctCr, imageHeight, imageWidth, imageDepth);
	
	//deleting unrequired memory
	for(int i=0;i<imageHeight ;i++){
		delete[] dctY[i];
		delete[] dctCb[i];
		delete[] dctCr[i];
	}
	delete[] dctY;
	delete[] dctCb;
	delete[] dctCr;
	
	
	//Lossless Entropy Compression Begins
	Entropy losslessCompression;
	
	//run length encoding
	vector<triple> rleY;
	int rleY_size = 0;
	losslessCompression.runLength( imageHeight, imageWidth, quantY, rleY, rleY_size);
	vector<triple> rleCb;
	int rleCb_size = 0;
	losslessCompression.runLength( imageHeight, imageWidth, quantCb, rleCb, rleCb_size);
	vector<triple> rleCr;
	int rleCr_size = 0;
	losslessCompression.runLength( imageHeight, imageWidth, quantCr, rleCr, rleCr_size);
	
	//deleting unrequired memory
	for(int i=0;i<imageHeight ;i++){
		delete[] quantY[i];
		delete[] quantCb[i];
		delete[] quantCr[i];
	}
	delete[] quantY;
	delete[] quantCb;
	delete[] quantCr;
	
	//huffman coding and output to file
	ofstream outputFile;
	char *optFile;
	optFile = new char[100];
	cout << "Please enter name of compressed file:" << endl;
	cin >> optFile;
	FileConvert convObj;
	
	//temporary file for storage
	string temp = "tempCompress";
	char *tempC = (char*)temp.c_str();
	outputFile.open(tempC);
	//huffman coding and insertion into temporary file
	losslessCompression.huffmanInsert(imageHeight, imageWidth, Height, Width, rleY, rleY_size, outputFile);
	losslessCompression.huffmanInsert(imageHeight, imageWidth, Height, Width, rleCb, rleCb_size, outputFile);
	losslessCompression.huffmanInsert(imageHeight, imageWidth, Height, Width, rleCr, rleCr_size, outputFile);
	outputFile.close();
	
	//converting to permanent file
	convObj.convertToFile(tempC, optFile);
	
	//deleting temporary file
	remove(tempC);
	
	//end of compression
	return;
}

void deCompress()
{
	//input file data
	ifstream inputFile;
	char *inpFile;
	inpFile = new char[100];
	cout << "Please enter the name of the compressed file:" << endl;
	cin >> inpFile;
	
	//temporary file for decompression
	string temp = "tempDeCompress";
	char *tempC = (char*)temp.c_str();
	
	//decoded image dimensions
	int decActHeight, decActWidth, decHeight, decWidth;
	vector <triple> decRleY;
	int decRleYSize = 0;
	vector <triple> decRleCr;
	int decRleCrSize = 0;
	vector <triple> decRleCb;
	int decRleCbSize = 0;
	
	//converting to temporary file
	FileConvert convObj;
	convObj.convertFromFile(inpFile, tempC);
	
	//lossless deCompression
	Entropy losslessCompression;
	inputFile.open(tempC);
	
	//decoding back to RLE
	losslessCompression.decodeToRLE(decActHeight, decActWidth, decHeight, decWidth, decRleY, decRleYSize, inputFile);
	losslessCompression.decodeToRLE(decActHeight, decActWidth, decHeight, decWidth, decRleCb, decRleCbSize, inputFile);
	losslessCompression.decodeToRLE(decActHeight, decActWidth, decHeight, decWidth, decRleCr, decRleCrSize, inputFile);
	inputFile.close();
	
	//padded image dimensions
	int imageHeight = decActHeight;
	int imageWidth = decActWidth;
	
	//actual image dimensions
	int Height = decHeight;
	int Width = decWidth;
	
	//dummy depth dimensions
	int imageDepth = 1, Depth = 1;
	
	//arrays for storing decompressed pixels in YCbCr Space
	short int **rPixelsY;
	rPixelsY = new short int * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		rPixelsY[i] = new short int[imageWidth];
	
	short int **rPixelsCb;
	rPixelsCb = new short int * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		rPixelsCb[i] = new short int[imageWidth];
	
	short int **rPixelsCr;
	rPixelsCr = new short int * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		rPixelsCr[i] = new short int[imageWidth];
	
	//arrays for storing dct coefficients after decompression 	
	double **rDctY;
	rDctY = new double * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		rDctY[i] = new double[imageWidth];
	
	double **rDctCb;
	rDctCb = new double * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		rDctCb[i] = new double[imageWidth];
		
	double **rDctCr;
	rDctCr = new double * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		rDctCr[i] = new double[imageWidth];	
	
	//arrays for storing quantization coefficients after decompression
	int **rQuantY;
	rQuantY = new int * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		rQuantY[i] = new int[imageWidth];
		
	int **rQuantCb;
	rQuantCb = new int * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		rQuantCb[i] = new int[imageWidth];	
	
	int **rQuantCr;
	rQuantCr = new int * [imageHeight];
	for(int i = 0; i < imageHeight; i++)
		rQuantCr[i] = new int[imageWidth];
	
	//performing inverse Run Length encoding to retrieve quantized coefficients
	losslessCompression.inverseRunLength( imageHeight, imageWidth, rQuantY, decRleY);
	losslessCompression.inverseRunLength( imageHeight, imageWidth, rQuantCb, decRleCb);
	losslessCompression.inverseRunLength( imageHeight, imageWidth, rQuantCr, decRleCr);
	
	//preparing target image
	CImg<unsigned char> targetImage(Width, Height, Depth, 3);
    
    char *tgtFile;
    tgtFile = new char[100];
    cout << "Please enter name of decompressed image:" << endl;
    cin >> tgtFile;

	//JPEG DeCompression
	JPEG Compression;
	
	//inverse quantization
	Compression.inverseQuantization(rQuantY, rQuantCb, rQuantCr, rDctY, rDctCb, rDctCr, imageHeight, imageWidth, imageDepth);
	
	//deleting unrequired memory
	for(int i=0;i<imageHeight ;i++){
		delete[] rQuantY[i];
		delete[] rQuantCb[i];
		delete[] rQuantCr[i];
	}
	delete[] rQuantY;
	delete[] rQuantCb;
	delete[] rQuantCr;
	
	//inverse Discrete Cosine Transform
	Compression.inverseDiscreteCosineTransform(rDctY, rDctCb, rDctCr, rPixelsY, rPixelsCb, rPixelsCr, imageHeight, imageWidth, imageDepth);
    
    //deleting unrequired memory
    for(int i=0;i<imageHeight ;i++){
		delete[] rDctY[i];
		delete[] rDctCb[i];
		delete[] rDctCr[i];
	}
	delete[] rDctY;
	delete[] rDctCb;
	delete[] rDctCr;
    
    //Converting back to RGB Space and inserting into target image
    Compression.convertToImage(rPixelsY, rPixelsCb, rPixelsCr, Height, Width, Depth, targetImage);
	
	//deleting unrequired memory
	
	for(int i=0;i<imageHeight ;i++){
		delete[] rPixelsY[i];
		delete[] rPixelsCb[i];
		delete[] rPixelsCr[i];
	}
	delete[] rPixelsY;
	delete[] rPixelsCb;
	delete[] rPixelsCr;
	
	
	//saving target image
	targetImage.save(tgtFile);
	
	
	//removing temporary file
	remove("tempC");
}
