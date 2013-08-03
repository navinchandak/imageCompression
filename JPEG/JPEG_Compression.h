#include <iostream>
#include "CImg.h"
using namespace std;
using namespace cimg_library;



#ifndef JPEG_COMPRESSION
#define JPEG_COMPRESSION
class JPEG{
public:

//To get the image file name
void getUserInput(string& imageSource);

//To read RGB values from an image and convert to YCbCr space
void convertToNewColourSpace(short int **pixelsY, short int **pixelsCb, short int **pixelsCr, int newHeight, int newWidth, int newDepth, CImg<unsigned char> sourceImage);

//To perform discrete cosine transformation on the individual pixels
void discreteCosineTransform(short int **pixelsY, short int **pixelsCb, short int **pixelsCr, double **dctY, double **dctCb, double **dctCr, int imageHeight, int imageWidth, int imageDepth);

//To perform discrete cosine transform on only one of the three components (Y, Cb, Cr)
void singleDiscreteCosineTransform(short int **pixels, double **dctY, int imageHeight, int imageWidth, int imageDepth, CImg<unsigned char> sourceImage);

//To perform discrete cosine transform on one component, in an 8 by 8 block
void singleBlockDiscreteCosineTransform(int startx, int starty, short int **pixels, double **dct,CImg<unsigned char> sourceImage);

//To perform discrete cosine transform, in an 8 by 8 block
void blockDiscreteCosineTransform(int startx, int starty, short int **pixelsY, short int **pixelsCb, short int **pixelsCr, double **dctY, double **dctCb, double **dctCr);

//To perform inverse discrete cosine transform on only one of the three components (Y, Cb, Cr)
void singleInverseDiscreteCosineTransform(double **dct, short int **rPixels, int imageHeight, int imageWidth, int imageDepth, CImg<unsigned char> sourceImage);

//To perform inverse discrete cosine transform on one component, in an 8 by 8 block
void singleBlockInverseDiscreteCosineTransform(int startx, int starty, double **dct, short int **rPixels, CImg<unsigned char> sourceImage);

//To perform inverse discrete cosine transformation on the individual pixels
void inverseDiscreteCosineTransform(double **dctY, double **dctCb, double **dctCr, short int **rPixelsY, short int **rPixelsCb, short int **rPixelsCr, int imageHeight, int imageWidth, int imageDepth);

//To perform inverse discrete cosine transform, in an 8 by 8 block
void blockInverseDiscreteCosineTransform(int startx, int starty, double **dctY, double **dctCb, double **dctCr, short int **rPixelsY, short int **rPixelsCb, short int **rPixelsCr);

//To perform quantization
void quantization(int **quantY, int **quantCb, int **quantCr, double **dctY, double **dctCb, double **dctCr, int imageHeight, int imageWidth, int imageDepth);

//To perform quantization on only one of the three components (Y, Cb, Cr)
void singleQuantization(int **quant, double **dct, int imageHeight, int imageWidth, int imageDepth, CImg<unsigned char> sourceImage);

//To perform quantization on one component, in an 8 by 8 block
void singleBlockQuantization(int startx, int starty, int **quantMatrix, int **quant, double **dct, int imageHeight, int imageWidth, int imageDepth, CImg<unsigned char> sourceImage);

//To perform quantization, in an 8 by 8 block
void blockQuantization(int startx, int starty, int **quantMatrix, int **quantY, int **quantCb, int **quantCr, double **dctY, double **dctCb, double **dctCr, int imageHeight, int imageWidth, int imageDepth);

//To perform inverse quantization
void inverseQuantization(int **quantY, int **quantCb, int **quantCr, double **dctY, double **dctCb, double **dctCr, int imageHeight, int imageWidth, int imageDepth);

//To perform inverse quantization on only one of the three components (Y, Cb, Cr
void singleInverseQuantization(int **quant, double **dct, int imageHeight, int imageWidth, int imageDepth, CImg<unsigned char> sourceImage);

//To perform inverse quantization on one component, in an 8 by 8 block
void singleBlockInverseQuantization(int startx, int starty, int **quantMatrix, int **quant, double **dct, int imageHeight, int imageWidth, int imageDepth, CImg<unsigned char> sourceImage);

//To perform inverse quantization, in an 8 by 8 block
void blockInverseQuantization(int startx, int starty, int **quantMatrix, int **quantY, int **quantCb, int **quantCr, double **dctY, double **dctCb, double **dctCr, int imageHeight, int imageWidth, int imageDepth);

//To convert from YCbCr space to RGB space, and store the same in the target image
void convertToImage(short int **pixelsY, short int **pixelsCb, short int **pixelsCr, int Height, int Width, int Depth, CImg<unsigned char> &targetImage);

//To round off a value to its nearest integer
int roundOff(double val);
};
#endif
