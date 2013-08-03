#include <iostream>
#include <string>
#include <bitset>
#include <fstream>
#include <cmath>
#include <fstream>
#include "CImg.h"
using namespace std;

using namespace cimg_library;

#include "JPEG_Compression.h"


void JPEG::convertToNewColourSpace(short int **pixelsY, short int **pixelsCb, short int **pixelsCr, int newHeight, int newWidth, int newDepth, CImg<unsigned char> sourceImage)
{
	int imageHeight = sourceImage.height();
	int imageWidth = sourceImage.width();
	int imageDepth = sourceImage.depth();
	short int rComp;
	short int bComp;
	short int gComp;
	
	int i, j;
	
	for( i = 0; i < newHeight; i++)
	{
		for( j = 0; j < newWidth; j++)
		{
			if(i < imageHeight && j < imageWidth)
			{
				
			//reading pixels	
			rComp  = sourceImage(j, i, 0, 0);
		    gComp  = sourceImage(j, i, 0, 1);
		    bComp = sourceImage(j, i, 0, 2);
			}
			else
			{
				rComp = bComp = gComp = 255;
			}
			
			//transforming to YCbCr space
		    pixelsY[i][j] = (short int)(0.299 * rComp + .587 * gComp + 0.114  * bComp - 128);
		    pixelsCb[i][j] = (short int)((-1.0 * 0.168935) * rComp + (-1.0 * 0.331665) * gComp + 0.50059 * bComp);
		    pixelsCr[i][j] = (short int)(0.499813 * rComp + (-1.0 * 0.418531) * gComp + (-1.0 * 0.081282) * bComp);		    
		}
	}

	return;
}


void JPEG::convertToImage(short int **pixelsY, short int **pixelsCb, short int **pixelsCr, int Height, int Width, int Depth, CImg<unsigned char> &targetImage)
{
	int rComp;
	int bComp;
	int gComp;
	
	int i, j;
	
	for( i = 0; i < Height; i++)
	{
		for( j = 0; j < Width; j++)
		{
			//converting to RGB space
			rComp = (int)((1.0 * (pixelsY[i][j] + 128) + 0.0 * pixelsCb[i][j] + 1.403  * pixelsCr[i][j]));
		    gComp = (int)((1.0 * (pixelsY[i][j] + 128) + (-1.0 * 0.334) * pixelsCb[i][j] + (-1.0 * 0.714) * pixelsCr[i][j]));
		    bComp = (int)((1.0 * (pixelsY[i][j] + 128) + (1.770) * pixelsCb[i][j] + (0.0) * pixelsCr[i][j]));
		    if(rComp < 0) rComp = 0;
		    if(rComp > 255) rComp = 255;
		    if(gComp < 0) gComp = 0;
		    if(gComp > 255) gComp = 255;
		    if(bComp < 0) bComp = 0;
		    if(bComp > 255) bComp = 255;

			//storing in target image;
		    targetImage(j, i, 0, 0) = rComp;
		    targetImage(j, i, 0, 1) = gComp;
		    targetImage(j, i, 0, 2) = bComp;
		}
	}
			
	
	return;
}

void JPEG::discreteCosineTransform(short int **pixelsY, short int **pixelsCb, short int **pixelsCr, double **dctY, double **dctCb, double **dctCr, int imageHeight, int imageWidth, int imageDepth)
{
	int x = imageHeight / 8;
	int y = imageWidth / 8;
	
	int i, j;
	
	//dividing into blocks, applying DCT on each block
	for( i = 0; i < x; i++)
	{
		for( j = 0; j < y; j++)
		{
			blockDiscreteCosineTransform( ( i * 8), ( j * 8), pixelsY, pixelsCb, pixelsCr, dctY, dctCb, dctCr);
		}
	}
}

void JPEG::blockDiscreteCosineTransform(int startx, int starty, short int **pixelsY, short int **pixelsCb, short int **pixelsCr, double **dctY, double **dctCb, double **dctCr)
{
	int i, j, p, q, ia, ja, pa, qa;
	double alphau, alphav, cos1, cos2, term1, term2, term3, PIBY8 = (M_PI_4 / 2);
	
	//applying DCT to an 8 by 8 block
	for(ia = startx; ia < startx + 8; ia++)
	{
		for(ja = starty; ja< starty + 8; ja++)
		{
			i = ia - startx;
			j = ja - starty;
			if(i == 0)
			{
				alphau = sqrt(0.125);
			}
			else
			{
				alphau = 0.5;
			}
			if(j == 0)
			{
				alphav = sqrt(0.125);
			}
			else
			{
				alphav = 0.5;
			}
	
			term1 = 0.0;
			term2 = 0.0;
			term3 = 0.0;
			
			for(pa = startx; pa < startx + 8; pa++)
			{
				for(qa = starty; qa < starty + 8; qa++)
				{
					p = pa - startx;
					q = qa - starty;
					term1 += (cos( ( PIBY8 * ( (double) p + 0.5) * (double) i)) * cos( ( PIBY8 * ( (double) q + 0.5) * (double) j)) * pixelsY[pa][qa]); 
					term2 += (cos( ( PIBY8 * ( (double) p + 0.5) * (double) i)) * cos( ( PIBY8 * ( (double) q + 0.5) * (double) j)) * pixelsCb[pa][qa]);
					term3 += (cos( ( PIBY8 * ( (double) p + 0.5) * (double) i)) * cos( ( PIBY8 * ( (double) q + 0.5) * (double) j)) * pixelsCr[pa][qa]);
				}
			}
			
			dctY[ia][ja] = term1 * alphau * alphav;
			dctCb[ia][ja] = term2 * alphau * alphav;
			dctCr[ia][ja] = term3 * alphau * alphav;
		}
	}			
}

//the next two functions are identical to the previous two functions, except that the transformation is applied on only one of the three colour components
void JPEG::singleDiscreteCosineTransform(short int **pixels, double **dct, int imageHeight, int imageWidth, int imageDepth, CImg<unsigned char> sourceImage)
{
	int x = imageHeight / 8;
	int y = imageWidth / 8;
	
	int i, j;
	
	for( i = 0; i < x; i++)
	{
		for( j = 0; j < y; j++)
		{
			singleBlockDiscreteCosineTransform( ( i * 8), ( j * 8), pixels, dct, sourceImage);
		}
	}
}

void JPEG::singleBlockDiscreteCosineTransform(int startx, int starty, short int **pixels, double **dct, CImg<unsigned char> sourceImage)
{
	int i, j, p, q, ia, ja, pa, qa;
	double alphau, alphav, cos1, cos2, term1, term2, term3, PIBY8 = (M_PI_4 / 2);
	
	for(ia = startx; ia < startx + 8; ia++)
	{
		for(ja = starty; ja< starty + 8; ja++)
		{
			i = ia - startx;
			j = ja - starty;
			if(i == 0)
			{
				alphau = sqrt(0.125);
			}
			else
			{
				alphau = 0.5;
			}
			if(j == 0)
			{
				alphav = sqrt(0.125);
			}
			else
			{
				alphav = 0.5;
			}
	
			term1 = 0.0;
			
			for(pa = startx; pa < startx + 8; pa++)
			{
				for(qa = starty; qa < starty + 8; qa++)
				{
					p = pa - startx;
					q = qa - starty;
					term1 += (cos( ( PIBY8 * ( (double) p + 0.5) * (double) i)) * cos( ( PIBY8 * ( (double) q + 0.5) * (double) j)) * pixels[pa][qa]); 
				}
			}
			
			dct[ia][ja] = term1 * alphau * alphav;
		}
	}			
}

void JPEG::inverseDiscreteCosineTransform(double **dctY, double **dctCb, double **dctCr, short int **rPixelsY, short int **rPixelsCb, short int **rPixelsCr, int imageHeight, int imageWidth, int imageDepth)
{
int x = imageHeight / 8;
	int y = imageWidth / 8;
	
	int i, j;
	
	//dividing into 8 by 8 blocks, applying the inverse DCT on each block
	for( i = 0; i < x; i++)
	{
		for( j = 0; j < y; j++)
		{
			blockInverseDiscreteCosineTransform( ( i * 8), ( j * 8),  dctY, dctCb, dctCr, rPixelsY, rPixelsCb, rPixelsCr);
		}
	}
}

void JPEG::blockInverseDiscreteCosineTransform(int startx, int starty, double **dctY, double **dctCb, double **dctCr, short int **rPixelsY, short int **rPixelsCb, short int **rPixelsCr)
{
	int i, j, p, q, ia, ja, pa, qa;
	double alphau, alphav, cos1, cos2, term1, term2, term3, PIBY8 = (M_PI_4 / 2);
	
	//applying standard inverse DCT on an 8 by 8 block
	for(ia = startx; ia < startx + 8; ia++)
	{
		for(ja = starty; ja < starty + 8; ja++)
		{
			term1 = 0.0;
			term2 = 0.0;
			term3 = 0.0;
			i = ia - startx;
			j = ja - starty;
			
			for(pa = startx; pa < startx + 8; pa++)
			{
				for(qa = starty; qa < starty + 8; qa++)
					{
						p = pa - startx;
						q = qa - starty;
						if(p == 0)
						{
							alphau = sqrt(0.125);
						}
						else
						{
							alphau = 0.5;
						}
						if(q == 0)
						{
							alphav = sqrt(0.125);
						}
						else
						{
								alphav = 0.5;
						}
						term1 += (alphau * alphav) * (cos( ( PIBY8 * ( (double) i + 0.5) * (double) p)) * cos( ( PIBY8 * ( (double) j + 0.5) * (double) q)) * dctY[pa][qa]); 
						term2 += (alphau * alphav) * (cos( ( PIBY8 * ( (double) i + 0.5) * (double) p)) * cos( ( PIBY8 * ( (double) j + 0.5) * (double) q)) * dctCb[pa][qa]);
						term3 += (alphau * alphav) * (cos( ( PIBY8 * ( (double) i + 0.5) * (double) p)) * cos( ( PIBY8 * ( (double) j + 0.5) * (double) q)) * dctCr[pa][qa]);
					}
			}
			
			//rounding off values to short int
			if(term1 - floor(term1) < 0.5)
			{
				rPixelsY[ia][ja] = (short int)(floor(term1));
			}
				else
			{
				rPixelsY[ia][ja] = (short int)(ceil(term1));
			}
			
			if(term2 - floor(term2) < 0.5)
			{
				rPixelsCb[ia][ja] = (short int)(floor(term2));
			}
				else
			{
				rPixelsCb[ia][ja] = (short int)(ceil(term2));
			}
			if(term3 - floor(term3) < 0.5)
			{
				rPixelsCr[ia][ja] = (short int)(floor(term3));
			}
				else
			{
				rPixelsCr[ia][ja] = (short int)(ceil(term3));
			}
		}
	}			
}

//the next two functions are identical to the previous two functions, except that the transformation is applied on only one of the three colour components
void JPEG::singleInverseDiscreteCosineTransform(double **dct, short int **rPixels, int imageHeight, int imageWidth, int imageDepth, CImg<unsigned char> sourceImage)
{
	int x = imageHeight / 8;
	int y = imageWidth / 8;
	
	int i, j;
	
	for( i = 0; i < x; i++)
	{
		for( j = 0; j < y; j++)
		{
			singleBlockInverseDiscreteCosineTransform( ( i * 8), ( j * 8),  dct, rPixels,sourceImage);
		}
	}
}

void JPEG::singleBlockInverseDiscreteCosineTransform(int startx, int starty, double **dct, short int **rPixels, CImg<unsigned char> sourceImage)
{
int i, j, p, q, ia, ja, pa, qa;
	double alphau, alphav, cos1, cos2, term1, term2, term3, PIBY8 = (M_PI_4 / 2);
	
	for(ia = startx; ia < startx + 8; ia++)
	{
		for(ja = starty; ja < starty + 8; ja++)
		{
			term1 = 0.0;
			i = ia - startx;
			j = ja - starty;
			
			for(pa = startx; pa < startx + 8; pa++)
			{
				for(qa = starty; qa < starty + 8; qa++)
					{
						p = pa - startx;
						q = qa - starty;
						if(p == 0)
						{
							alphau = sqrt(0.125);
						}
						else
						{
							alphau = 0.5;
						}
						if(q == 0)
						{
							alphav = sqrt(0.125);
						}
						else
						{
								alphav = 0.5;
						}
						term1 += (alphau * alphav) * (cos( ( PIBY8 * ( (double) i + 0.5) * (double) p)) * cos( ( PIBY8 * ( (double) j + 0.5) * (double) q)) * dct[pa][qa]); 
					}
			}
			
			if(term1 - floor(term1) < 0.5)
			{
				rPixels[ia][ja] = (short int)(floor(term1));
			}
				else
			{
				rPixels[ia][ja] = (short int)(ceil(term1));
			}			
		}
	}			
}


void JPEG::quantization(int **quantY, int **quantCb, int **quantCr, double **dctY, double **dctCb, double **dctCr, int imageHeight, int imageWidth, int imageDepth)
{
	//reading quantization matrix from file
	ifstream filein;
	filein.open("quantizationMatrix.txt");
	int **quantMatrix;
	quantMatrix = new int*[8];
	for(int i = 0; i < 8; i++)
	{
		quantMatrix[i] = new int[8];
	}
	
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			filein >> quantMatrix[i][j];
		}
	}
	filein.close();
	
	//dividing image into 8 by 8 blocks and applying quantization to each block
	int x = imageHeight / 8;
	int y = imageWidth / 8;
	int i, j;
	
	for( i = 0; i < x; i++)
	{
		for( j = 0; j < y; j++)
		{
	    blockQuantization( (i * 8), (j * 8), quantMatrix, quantY, quantCb, quantCr, dctY, dctCb, dctCr, imageHeight, imageWidth, imageDepth);
		}
	}
	
	return;
}

void JPEG::blockQuantization(int startx, int starty, int **quantMatrix, int **quantY, int **quantCb, int **quantCr, double **dctY, double **dctCb, double **dctCr, int imageHeight, int imageWidth, int imageDepth)
{
	int ia, ja, i, j;
	
	//performing simple quantization on a simple 8 by 8 block
	for(ia = startx; ia < startx + 8; ia++)
	{
		for (ja = starty; ja < starty + 8; ja++)
		{
			i = ia - startx;
			j = ja - starty;
			quantY[ia][ja] = roundOff(dctY[ia][ja]/quantMatrix[i][j]);
			quantCb[ia][ja] = roundOff(dctCb[ia][ja]/quantMatrix[i][j]);
			quantCr[ia][ja] = roundOff(dctCr[ia][ja]/quantMatrix[i][j]);
		}
	}
	return;
}

//the next two functions are identical to the previous two functions, except that the transformation is applied on only one of the three colour components
void JPEG::singleQuantization(int **quant, double **dct, int imageHeight, int imageWidth, int imageDepth, CImg<unsigned char> sourceImage)
{
	ifstream filein;
	filein.open("quantizationMatrix.txt");
	int **quantMatrix;
	quantMatrix = new int*[8];
	for(int i = 0; i < 8; i++)
	{
		quantMatrix[i] = new int[8];
	}
	
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			filein >> quantMatrix[i][j];
		}
	}
	
	filein.close();
	int x = imageHeight / 8;
	int y = imageWidth / 8;
	int i, j;
	
	for( i = 0; i < x; i++)
	{
		for( j = 0; j < y; j++)
		{
	     singleBlockQuantization( (i * 8), (j * 8), quantMatrix, quant, dct, imageHeight, imageWidth, imageDepth, sourceImage);
		}
	}
	
	return;
}

void JPEG::singleBlockQuantization(int startx, int starty, int **quantMatrix, int **quant, double **dct, int imageHeight, int imageWidth, int imageDepth, CImg<unsigned char> sourceImage)
{
	int ia, ja, i, j;
	for(ia = startx; ia < startx + 8; ia++)
	{
		for (ja = starty; ja < starty + 8; ja++)
		{
			i = ia - startx;
			j = ja - starty;
			quant[ia][ja] = roundOff(dct[ia][ja]/quantMatrix[i][j]);
		}
	}
	return;
}

void JPEG::inverseQuantization(int **quantY, int **quantCb, int **quantCr, double **dctY, double **dctCb, double **dctCr, int imageHeight, int imageWidth, int imageDepth)
{
	//Reading Quantization Matrix From File
	ifstream filein;
	filein.open("quantizationMatrix.txt");
	int **quantMatrix;
	quantMatrix = new int*[8];
	for(int i = 0; i < 8; i++)
	{
		quantMatrix[i] = new int[8];
	}
	
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			filein >> quantMatrix[i][j];
		}
	}
	filein.close();

	//dividing image into 8 by 8 blocks for inverse quantization
	int x = imageHeight / 8;
	int y = imageWidth / 8;
	int i, j;
	
	for( i = 0; i < x; i++)
	{
		for( j = 0; j < y; j++)
		{
	    blockInverseQuantization( (i * 8), (j * 8), quantMatrix, quantY, quantCb, quantCr, dctY, dctCb, dctCr, imageHeight, imageWidth, imageDepth);
		}
	}
	
	return;
}

void JPEG::blockInverseQuantization(int startx, int starty, int **quantMatrix, int **quantY, int **quantCb, int **quantCr, double **dctY, double **dctCb, double **dctCr, int imageHeight, int imageWidth, int imageDepth)
{
	int ia, ja, i, j;
	
	//applying simple inverse quantization on a single 8 by * block
	for(ia = startx; ia < startx + 8; ia++)
	{
		for (ja = starty; ja < starty + 8; ja++)
		{
			i = ia - startx;
			j = ja - starty;
			dctY[ia][ja] = quantY[ia][ja] * quantMatrix[i][j];
			dctCb[ia][ja] = quantCb[ia][ja] * quantMatrix[i][j];
			dctCr[ia][ja] = quantCr[ia][ja] * quantMatrix[i][j];
		}
	}
	return;
}

//the next two functions are identical to the previous two functions, except that the transformation is applied on only one of the three colour components
void JPEG::singleInverseQuantization(int **quant, double **dct, int imageHeight, int imageWidth, int imageDepth, CImg<unsigned char> sourceImage)
{
	ifstream filein;
	filein.open("quantizationMatrix.txt");
	int **quantMatrix;
	quantMatrix = new int*[8];
	for(int i = 0; i < 8; i++)
	{
		quantMatrix[i] = new int[8];
	}
	
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 8; j++)
		{
			filein >> quantMatrix[i][j];
		}
	}
	
	filein.close();
	int x = imageHeight / 8;
	int y = imageWidth / 8;
	int i, j;
	
	for( i = 0; i < x; i++)
	{
		for( j = 0; j < y; j++)
		{
	     singleBlockInverseQuantization( (i * 8), (j * 8), quantMatrix, quant, dct, imageHeight, imageWidth, imageDepth, sourceImage);
		}
	}
	
	return;
}

void JPEG::singleBlockInverseQuantization(int startx, int starty, int **quantMatrix, int **quant, double **dct, int imageHeight, int imageWidth, int imageDepth, CImg<unsigned char> sourceImage)
{
	int ia, ja, i, j;
	for(ia = startx; ia < startx + 8; ia++)
	{
		for (ja = starty; ja < starty + 8; ja++)
		{
			i = ia - startx;
			j = ja - starty;
			dct[ia][ja] = quant[ia][ja] * quantMatrix[i][j];
		}
	}
	return;
}

//function to round off a decimal number to its closest integer
int JPEG::roundOff(double val)
{
	if(val - floor(val) < 0.5)
	return ((int) (floor(val)));
	else
	return ((int) (ceil(val)));
}			
		
		


