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
#include "WorstBalancedPathNode.hpp"
#include "WorstBalancedPathEdge.hpp"
#include "BestBalancedPathEdge.hpp"
#include "BestBalancedPathNode.hpp"
#include "BalancedSuurballe.hpp"
#include "Suurballe.hpp"
#include <sstream>

vector<bool> used = vector<bool> (100,false);
int limit = 20; // max path len
vector<int> path = vector<int> (100,-1);
vector<int> c;
int tail;

vector<string> &split(const string &s, char delim, vector<string> &elems);

vector<string> split(const string &s, char delim);

void print_path();
void dfs(int v, int target, int depth, Graph &g) ;
void return_path(int target);
void delete_tail(int source);


/**
 * Compilar : g++ *.cpp  -o b -g -std=c++11 -pthread 
 * Executar : ./b <arquivo>
 */
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

		// used[0] = true;
		
		// dfs(22,23,0, g1) ;

		// for (int v = 0; v < g1.getNumberOfNodes(); v++)
		// {
		// 	cout<<" "<<v<<endl;
		// 	g1.printAdjacents(v);
		// 	cout<<"----------------------------"<<endl;
		// }

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

		// Suurballe bs;

		// bool sobrevivente = bs.execute(g2,pathFile[pathFile.size()-1]);

		// if (!sobrevivente)
		// {
		// 	cout<<"Topologia não sobrevivente "<<endl;
		// 	return 0;
		// }

		// cout<<"sobrevivente "<<sobrevivente<<"\n\n"<<endl;

		// Suurballe s;

		// bool survivor = s.execute(g2,pathFile[pathFile.size()-1]);

		// if (!survivor)
		// {
		// 	cout<<"não sobrevivente "<<survivor<<endl;
		// }
		
		/**
		 * Calcula diâmetro do grafo para limitar 
		 */
		BestBalancedPathEdge be;
		
		bool survivor = be.execute(g1,"best_balanced_edge_"+pathFile[pathFile.size()-1]);
		
		if (!survivor)
		{
			cout<<"não sobrevivente "<<survivor<<endl;
		}
		
		BestBalancedPathNode bn;

		survivor = bn.execute(g1,"best_balanced_node_"+pathFile[pathFile.size()-1]);

		if (!survivor)
		{
			cout<<"não sobrevivente "<<survivor<<endl;
		}

		WorstBalancedPathNode wn;

		survivor = wn.execute(g1,"worst_balanced_node_"+pathFile[pathFile.size()-1]);

		if (!survivor)
		{
			cout<<"não sobrevivente "<<survivor<<endl;
		}

		WorstBalancedPathEdge we;

		survivor = we.execute(g2,"worst_balanced_edge_"+pathFile[pathFile.size()-1]);

		if (!survivor)
		{
			cout<<"não sobrevivente "<<survivor<<endl;
		}

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



void return_path(int target)
{
    vector<int> p;

    int u = target;

    p.push_back( target );

    while( path[u] != -1 )
    {
        p.push_back( path[u]);
        u = path[u];
    }
    reverse( p.begin(),p.end() );//inverte caminho


    for (int i = 0; i < p.size() ; i++)
    {
        cout<<" "<<p[i];
    }
    cout<<endl;
}

void delete_tail(int tail)
{
    path[tail] = -1;
}
void print_path()
{
	// return_path(1);
	for (int i = 0; i < (int)c.size(); i++)
	{
		cout<<" "<<c[i];
	}
	cout<<endl;
}


void dfs(int v, int target, int depth, Graph &g) 
{
	c.push_back(v);

    if (v == target) 
    {
       print_path();
       // return;
    } 
  	else
    {
      // cout << "depth: " << depth << endl;
      if (depth > 5) {
      	return;
      }
	  Node node = g.getNodeAtPosition(v);
	  vector<int > adjacents = node.getAdjacentsNodes();

	  for(int u = 0; u < (int)adjacents.size();u++) 
	  {
	    if (used[adjacents[u]]) 
	    {
	      continue;
	    }

	    used[adjacents[u]] = true;

	    dfs(adjacents[u], target,depth + 1,g);
	   	// cout<<" "<<c[(int)c.size()-1]<<endl;
	    c.pop_back();

	    used[adjacents[u]] = false;
	  }
    }
}