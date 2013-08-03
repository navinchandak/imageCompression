#include <iostream>
#include <fstream>
#include <stdio.h>
#include "CImg.h"
#include <cmath>
using namespace std;
using namespace cimg_library;

void meanSquaredError(CImg <unsigned char> &original, CImg <unsigned char> &modified, int imageHeight, int imageWidth, double &R, double &G, double &B)
{
	R = G = B = 0;
	int ro, go, bo, rm, gm, bm;
	for(int i = 0; i < imageHeight; i++)
	{
		for(int j = 0; j < imageWidth; j++)
		{
			ro = original(j, i, 0, 0);
			go = original(j, i, 0, 1);
			bo = original(j, i, 0, 2);
			rm = modified(j, i, 0, 0);
			gm = modified(j, i, 0, 1);
			bm = modified(j, i, 0, 2);
			R += ((double)((ro - rm) * (ro - rm)));
			G += ((double)((go - gm) * (go - gm)));
			B += ((double)((bo - bm) * (bo - bm)));
		}
	}
	R /= (imageHeight * imageWidth);
	G /= (imageHeight * imageWidth);
	B /= (imageHeight * imageWidth);
	return;
}

void printPSNR(char *original, char *modified)
{
	CImg <unsigned char> oimg(original);
	CImg <unsigned char> mimg(modified);
	double mser, mseg, mseb;
	int imageHeight = oimg.height(), imageWidth = mimg.width();
	meanSquaredError(oimg, mimg, imageHeight, imageWidth, mser, mseg, mseb);
	double maxr, maxg, maxb;
	maxr = maxg = maxb = 255.0;
	mser = sqrt(mser);
	mseg = sqrt(mseg);
	mseb = sqrt(mseb);
	mser = 20 * log10(maxr / mser);
	mseg = 20 * log10(maxg / mseg);
	mseb = 20 * log10(maxb / mseb);
	cout << "Images Compared" << endl;
	cout << original << endl;
	cout << modified << endl;
	cout << "PSNR RED = " << mser;
	cout << "PSNR GREEN = " << mseg;
	cout << "PSNR BLUE = " << mseb;
	cout << endl;
	
	return;
}

int main()
{
	char *n1;
	char *n2;
	n1 = new char[100];
	n2 = new char[100];
	char val = 'y';
	
	while(val != 'n')
	{
		cin >> n1 >> n2;
		printPSNR(n1, n2);
		cin >> val;
	}
	
	return 0;
}
	
	
