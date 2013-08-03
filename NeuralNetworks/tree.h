#include<list>
using namespace std;

struct node{
	node* lc;
	node* rc;
	int val;
};
class tree{
	public:
		//root of the tree
		node* root;
		int wt;//weight
		int val;//value stored in node
		//reading values from the infile and instream using the curr tree
  int readValue(ifstream& ,infile* ni);
		//const
		tree(int val,int w);
		//overload.. for the pq
		bool operator< (const tree& t) const;
		//mrging .. for the pq
		tree merge(tree& t2);
		// copy a list  to form a new list
		void copy(list<bool>& newl, list<bool> oldList);
		//write the tree contents to a array..for writing
		void writeInArray(list<bool>* a);
		void writeInArray(list<bool>* a,node* curr,list<bool> l);
		//void insertPath(int c, list<bool> l);
		//void insertPath(int c, node* root, list<bool> l);

		//read the huffman table from the input file, infile etc and make the tree
void readHuffmanTable(node* curr,ifstream & iptfile, infile* in);
void readHuffmanTable(ifstream & in, infile* file);
//writing the tree to a file
void writeTreeToFile(node* curr, outfile* out);
void writeTreeToFile(outfile* out);
//deallocate memory
void deallocateMem();
void deallocateMem(node* & a);
};
