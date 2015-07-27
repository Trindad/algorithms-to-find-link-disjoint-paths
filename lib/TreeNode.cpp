#include "TreeNode.hpp"

TreeNode::TreeNode(int node)
{
	this->parent = NULL;
	this->index = node;
}

TreeNode::~TreeNode(){ }

void TreeNode::addChild(TreeNode* child)
{
	this->children.push_back(child);
}
