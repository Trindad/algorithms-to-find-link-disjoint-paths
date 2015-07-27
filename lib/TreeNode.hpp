/**
 * Copyright 2015 Silvana Trindade
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
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