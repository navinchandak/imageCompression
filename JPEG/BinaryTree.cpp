#include <iostream>
#include "BinaryTree.h"
#include <stdlib.h>

using namespace std;

BinaryTree::BinaryTree()
{
	start = new TreeNode();
}

int BinaryTree::size()
{
	return findSize(start);
}

int BinaryTree::findSize(TreeNode *node)
{
	if(node == NULL)
	return 0;
	else
	return (findSize(node->left) + findSize(node->right) + 1);
}

TreeNode& BinaryTree::root()
{
	return *start;
}

void BinaryTree::setElement(TreeNode& v, int e)
{
	v.value = e;
}

TreeNode& BinaryTree::addLeft(TreeNode& v, int e)
{
	TreeNode *n;
	n = new TreeNode();
	n->value = e;
	n->left = NULL;
	n->right = NULL;
	v.left = n;
	return *n;
}

TreeNode& BinaryTree::addRight(TreeNode& v, int e)
{
	TreeNode *n;
	n = new TreeNode();
	n->value = e;
	n->left = NULL;
	n->right = NULL;
	v.right = n;
	return *n;
}

TreeNode& BinaryTree::leftChild(TreeNode& v)
{
	if(v.left == NULL)
	exit(1);
	else
	return *(v.left);
}

TreeNode& BinaryTree::rightChild(TreeNode& v)
{
	if(v.right == NULL)
	exit(1);
	else
	return *(v.right);
}

int BinaryTree::element(TreeNode& v)
{
	return v.value;
}

void BinaryTree::printPreOrder()
{
	PreOrder(start);
}

void BinaryTree::PreOrder(TreeNode *node)
{
	if(node == NULL)
	return;
	else
	{
		cout << node->value << endl;
		PreOrder(node->left);
		PreOrder(node->right);
	}
}

void BinaryTree::printPostOrder()
{
	PostOrder(start);
}

void BinaryTree::PostOrder(TreeNode *node)
{
	if(node == NULL)
	return;
	else
	{
	
		PostOrder(node->left);
		PostOrder(node->right);
		cout << node->value << endl;
	}
}


void BinaryTree::printInOrder()
{
	InOrder(start);
}

void BinaryTree::InOrder(TreeNode *node)
{
	if(node == NULL)
	return;
	else
	{
	
		InOrder(node->left);
		cout << node->value << endl;
		InOrder(node->right);
	}
}

int BinaryTree::height(TreeNode& v)
{
	if(v.left == NULL && v.right == NULL)
	return 0;
	
	if(v.left == NULL)
	return (height(*(v.right)));
	
	if(v.right == NULL)
	return (height(*(v.left)));
	
	int a = height(*(v.left));
	int b = height(*(v.right));
	
	if(a>b)
	return a + 1;
	else
	return b + 1;
}

int BinaryTree::height()
{
	return height(*start);
}

int BinaryTree::depth(TreeNode &v)
{
	return (findDepth(v, *start));
}

int BinaryTree::findDepth(TreeNode &v, TreeNode &s)
{
	if(&s == NULL)
	return -1;
	
	if(&v == &s)
	return 0;
	
	int a = findDepth(v, *s.left);
	if(a >= 0)
	return a + 1;
	
	int b = findDepth(v, *s.right);
	return b + 1;
}

bool BinaryTree::isLeaf(TreeNode *node)
{
	return((node->left == NULL) && (node->right == NULL));
}

	
	
	
	
	
