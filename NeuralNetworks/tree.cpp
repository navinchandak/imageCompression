#include<list>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<fstream>
#include<queue>
#include "file.h"
#include"tree.h"
#include "declarations.h"
using namespace std;
//constructor make a new tree with val value, w weight and NULL left and right child 
tree::tree(int val,int w){
	root=new node;
	root->val=val;
	wt=w;
	root->lc=NULL;
	root->rc=NULL;
}
//jus operator overload
bool tree::operator< (const tree& t) const{
	return wt>t.wt;
}
//simple, create a new tree with curr tree as left child and the para tree as right child
tree tree::merge(tree& t2){
	tree newTree(-1,wt+t2.wt);
	(newTree.root)->lc=root;
	(newTree.root)->rc=t2.root;
	return newTree;
}

//copy oldlist into a new list
void tree::copy(list<bool>& newl, list<bool> oldList){
	list<bool>::iterator it;
	for(it=oldList.begin();it!=oldList.end();it++){
		newl.push_back(*it);
	}
}
//write the content of tree into the array for quick retrieval while writing the values onto the file
void tree::writeInArray(list<bool>* a){
	list<bool> l;
	writeInArray(a,root,l);
}
void tree::writeInArray(list<bool>* a,node* curr,list<bool> l){
	if(curr->val==-1){
		list<bool> leftList;
		copy(leftList,l);
		l.push_back(1);
		leftList.push_back(0);
		writeInArray(a,curr->rc,l);
		writeInArray(a,curr->lc,leftList);
	}
	else{
		while(!l.empty()){
			a[curr->val].push_back(l.front());
			l.pop_front();
		}
	}
}
//read the huffman table from the file.. which is represented in terms of 0 and 1s (for edges and value!=-1 vertex)
//see details in writeTreeToFile for details of how the huffman_table is stored in the file
void tree::readHuffmanTable(node* curr,ifstream & iptfile, infile* in){
	bool a=in->readFromFile(iptfile,1);
	if(a==1){
		curr->val=in->readFromFile(iptfile,10);
	}
	else if (a==0){
		if(curr->lc==NULL){
			tree t1(-1,0);
			curr->lc=t1.root;
			tree t2(-1,0);
			curr->rc=t2.root;
		}
			readHuffmanTable(curr->lc,iptfile,in);
			readHuffmanTable(curr->rc,iptfile,in);
	}
}
//helper function
void tree::readHuffmanTable(ifstream & in, infile* file){
	readHuffmanTable(root,in,file);
}
//writing the tree onto the file with a special method
void tree::writeTreeToFile(node* curr, outfile* out){
	//if the curr_node contains a value (!=-1), then write a 1 to the file and write the value in 10 bits
	//otherwise write a 0 onto the file and recurse in the left and right edges
	//note that this value is not required for the root
	//the 0 and 1 can be understood to represent the edges
	if(curr->val==-1){
		out->writeToFile(0,1);
		writeTreeToFile(curr->lc,out);
		writeTreeToFile(curr->rc,out);
	}
	else{
	 out->writeToFile(1,1);
		out->writeToFile(curr->val,10);
	}
}
void tree::writeTreeToFile(outfile* out){
	writeTreeToFile(root,out);
}
//insert path in earlier method of writing, reading .. commented out..
/*void tree::insertPath(int c, list<bool> l){
	insertPath(c,root,l);
}
void tree::insertPath(int c, node* root, list<bool> l){
	if(l.empty()){
		root->val=c;
		return;
	}
	bool a=l.front();
	l.pop_front();
	if(a==0){
		if(root->lc==NULL){
			tree t1(-1,0);
			root->lc=t1.root;
		}
		insertPath(c,root->lc,l);
	}
	else if(a!=0){
		if(root->rc==NULL){
			tree t1(-1,0);
			root->rc=t1.root;
		}
		insertPath(c,root->rc,l);
	}
}*/

//read the value by using the instream and ifile
int tree::readValue(ifstream& inStream,infile* ni){
	bool c;
	node *curr=root;
	while(1){
		c=ni->readFromFile(inStream,1);
		if(c==0) curr=curr->lc; 
		else curr=curr->rc;
		if(curr->val!=-1) return curr->val;
	}
}

//deallocate the entire tree structure when done
void tree::deallocateMem(node* & a){
	if(a->lc!=NULL){
		deallocateMem(a->lc);
		deallocateMem(a->rc);
	}
	delete a;
}
//helper function
void tree::deallocateMem(){
	deallocateMem(root);
}



	
