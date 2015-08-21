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
#include "Graph.hpp"
#include "BestBalancedPathEdge.hpp"
#include "Suurballe.hpp"
#include <sstream>

vector<string> &split(const string &s, char delim, vector<string> &elems);

vector<string> split(const string &s, char delim);

int main(int argc, char const *argv[])
{

	if (argc <= 1)
	{
		return 0;
	}

	ifstream file;

	file.open(argv[1]);

	vector<string> pathFile = split(argv[1],'/');

	
	if (file.is_open())
	{
		Graph g1,g2;

		g1.setUseEdgeWeight(false);
		g2.setUseEdgeWeight(false);

		string line = " ";

		getline (file,line);

		int n = stoi(line);//obtêm o número de nós

		g1.setNumberOfNodes(n);
		g1.setMinimumDegree(2);
		g1.setMaximumDegree(n-1);

		g2.setNumberOfNodes(n);
		g2.setMinimumDegree(2);
		g2.setMaximumDegree(n-1);

		// cout<<"Número de nós: "<<n<<endl;
		vector<string> nodes;

		while( getline (file,line) )
		{
			nodes = split(line.c_str(),' ');

			int u = stoi(nodes[0])-1;
			int v = stoi(nodes[1])-1;
			
			// g.setEdgeDirected(u,v);
			g1.setEdge(u,v);
			g1.setWeight(u,v,1.0);
			g1.setWeight(v,u,1.0);

			g2.setEdge(u,v);
			g2.setWeight(u,v,1.0);
			g2.setWeight(v,u,1.0);
		}
		
		// g.setWeightEdgeDirected(1-1,8-1,2.0f);
		// g.setWeightEdgeDirected(1-1,7-1,8.0f);
		// g.setWeightEdgeDirected(1-1,2-1,3.0f);
		// g.setWeightEdgeDirected(2-1,3-1,1.0f);
		// g.setWeightEdgeDirected(2-1,6-1,6.0f);
		// g.setWeightEdgeDirected(2-1,7-1,4.0f);
		// g.setWeightEdgeDirected(3-1,4-1,5.0f);
		// g.setWeightEdgeDirected(5-1,4-1,7.0f);
		// g.setWeightEdgeDirected(6-1,5-1,2.0f);
		// g.setWeightEdgeDirected(5-1,8-1,3.0f);
		// g.setWeightEdgeDirected(8-1,6-1,5.0f);
		// g.setWeightEdgeDirected(7-1,4-1,1.0f);

		Suurballe bs;

		bool sobrevivente = bs.execute(g2,pathFile[pathFile.size()-1]);

		cout<<"sobrevivente "<<sobrevivente<<"\n\n"<<endl;

		// Suurballe s;

		// sobrevivente = s.execute(g2,pathFile[pathFile.size()-1]);

		// cout<<"sobrevivente "<<sobrevivente<<endl;
		// 
		
		Suurballe b;

		b.execute(g1,pathFile[pathFile.size()-1]);

		file.close();
	}
	else
	{
		cout<<"ERROR"<<endl;
	}
	
	// TreeNode *t = new TreeNode(5);
	// TreeNode *n3 = new TreeNode(3);
	// TreeNode *n6_1 = new TreeNode(6);
	// TreeNode *n6_2 = new TreeNode(6);
	// TreeNode *n7_1 = new TreeNode(7);
	// TreeNode *n7_2 = new TreeNode(7);
	// TreeNode *n8_1 = new TreeNode(8);
	// TreeNode *n8_2 = new TreeNode(8);
	// TreeNode *n8_3 = new TreeNode(8);
	// TreeNode *n8_4 = new TreeNode(8);
	// n3->parent = t;
	// n6_1->parent = t;
	// n7_1->parent = n6_1;
	// n8_1->parent = n6_1;
	// n8_2->parent = n7_1;
	// n6_2->parent = n3;
	// n7_2->parent = n6_2;
	// n8_3->parent = n6_2;
	// n8_4->parent = n7_2;

	// t->addChild(n6_1);
	// t->addChild(n3);
	// n3->addChild(n6_2);
	// n6_2->addChild(n7_2);
	// n6_2->addChild(n8_3);
	// n7_2->addChild(n8_4);
	// n6_1->addChild(n7_1);
	// n6_1->addChild(n8_1);
	// n7_1->addChild(n8_2);

	// TreeNode *temp = n8_4;
	// while (temp) {
	// 	printf("%d\n", temp->index);
	// 	temp = temp->parent;
	// }
	
	return 0;
}


vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
   vector<string> elems;
    split(s, delim, elems);
    return elems;
}