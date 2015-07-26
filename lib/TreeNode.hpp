#ifndef TREENODE_H
#define TREENODE_H

#include <iostream>
#include <algorithm> //reverse vector
#include <vector>
#include <iostream>


using namespace std;

class TreeNode
{
public:

	TreeNode(int);
	~TreeNode();
	void addChild(TreeNode*);//adiciona filho

	TreeNode *parent;//nó pai 
	int index;
	vector<TreeNode *> children; //filhos do nó pai
};

#endif