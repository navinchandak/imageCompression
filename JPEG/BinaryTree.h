#include <iostream>
using namespace std;

#ifndef BINARYTREE
#define BINARYTREE

//the basic tree node
struct TreeNode
{
	int value;
	TreeNode *left;
	TreeNode *right;
};

class BinaryTree
{
	public:
	TreeNode *start;
	
	//constructor
	BinaryTree();
	//size of binary tree
	int size();
	int findSize(TreeNode *node);
	//return the root node
	TreeNode& root();
	//set a tree node
	void setElement(TreeNode& v, int e);
	//add children
	TreeNode& addLeft(TreeNode& v, int e);
	TreeNode& addRight(TreeNode& v, int e);
	//return the children
	TreeNode& leftChild(TreeNode& v);
	TreeNode& rightChild(TreeNode& v);
	//return element at a node
	int element(TreeNode& v);
	//standard preorder traversal
	void printPreOrder();
	void PreOrder(TreeNode *node);
	//standard postorder traversal
	void printPostOrder();
	void PostOrder(TreeNode *node);
	//stanard inorder traversal
	void printInOrder();
	void InOrder(TreeNode *node);
	//height of a node
	int height(TreeNode &v);
	int height();
	//depth of a node
	int depth(TreeNode &v);
	int findDepth(TreeNode &v, TreeNode &s);
	//check whether a node is a leaf
	bool isLeaf(TreeNode *node);
};
	
#endif	
