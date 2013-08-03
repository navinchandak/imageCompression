#include "vectorf.h"
#include<iostream>
#include "neuron.h"
using namespace std;
//initializing the neuron
void neuron::initialize(double* a){
	w[0]=a[0];w[1]=a[1];w[2]=a[2];
}
//to approximate a neuron to the pixel
void neuron::approximate(double* in,double alpha){
	for(int i=0;i<3;i++){
		w[i]+=alpha*(in[i]-w[i]);
	}
}
