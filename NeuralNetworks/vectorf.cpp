#include<iostream>
#include<cmath>
using namespace std;
//normalizing a vector
void normalize(double* a){
	double s=0;
	for(int i=0;i<3;i++){s=s+(a[i]*a[i]);}
	s=sqrt(s);
	for(int i=0;i<3;i++){a[i]/=s;}
}
//dot product of one vector with the other
double dotProduct(double* a, double* b, int c){
	double s=0;
	for(int i=0;i<c;i++){
		s+=a[i]*b[i];
	}
	return s;
}
//distance square of one vector from other 
double distance(double * a, double *b,int c){
	double d=0;
	for(int i=0;i<c;i++){
		d+=pow((a[i]-b[i]),2);
	}
	return d;
	
}
	

