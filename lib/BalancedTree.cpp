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

#include "BalancedTree.hpp"


BalancedTree::BalancedTree(){}
BalancedTree::~BalancedTree(){}

void BalancedTree::averageHops(Graph g)
{
    vector<double> avgHops;
    double kp = 0;

    double sumWorkingHops = 0, sumBackupHops = 0;

    for (unsigned int u = 0; u < this->hopBackup.size(); u++)
    {
        sumWorkingHops = sumWorkingHops + this->hopWorking[u];
        sumBackupHops = sumBackupHops + this->hopBackup[u];
    }

    this->datas<<"sum Working = "<<sumWorkingHops<<endl;
    this->datas<<"sum Backup = "<<sumBackupHops<<endl;
    int N = g.getNumberOfNodes();

    cout<<"sumBackupHops "<<sumBackupHops<<" sumWorkingHops "<<sumWorkingHops<<" diff = "<<abs(sumWorkingHops-sumBackupHops)<<endl;
    double avgWorkingHops = ( 2* sumWorkingHops )/ ( N*(N-1) );
    double averageBackupHops = (2 *sumBackupHops )/ ( N*(N-1) );

    avgHops.push_back(avgWorkingHops);
    avgHops.push_back(averageBackupHops);

    kp = avgHops[1]/avgHops[0];
    this->datas<<"protection coefficient = "<<kp<<endl;
    cout<<"Kp  = "<<kp<<endl;
    this->datas.close();

    avgHops.clear();
}

 vector< vector<int> > BalancedTree::compareWithOthers(Graph g,vector<int> &p1, vector<int> &p2) 
{
    vector<int> temp = vector<int> (g.getNumberOfNodes(),-1);
    vector<int> path1;
    vector<int> path2;
    vector<vector<int>> paths;

    makePathVector(p1,path1,temp);
    makePathVector(p2,path2,temp);

    /**
     * Remover arestas invertidas
     * Dos caminhos mínimos p1 e p2
     */
    for ( unsigned int u = 0; u < path1.size()-1; u+=2)
    {
        for (unsigned int v = 0; v < path2.size()-1; v+=2)
        {
            //exclui arestas em comum mas invertidas
            if (path1[u] == path2[v+1] && path1[u+1] == path2[v])
            {
                discardCommonEdge(path1,path2,u,v);
            }

            if (path1[u] == path2[v] && path1[u+1] == path2[v+1])
            {
                return paths;
            }

        }
    }

    paths.push_back(path1);
    paths.push_back(path2);

    return paths;
}

bool BalancedTree::isNodeInPath(TreeNode *node,int index)
{
    TreeNode *temp = node;

    while(temp)
    {
        if (temp->index == index)
        {
            return true;
        }

        temp = temp->parent;
    }

    return false;
} 

bool BalancedTree::searchPath(vector< vector<int> > paths, vector<int> path)
{
    bool eq = false;

    for (int u = 0; u < paths.size(); u++)
    {
        if (paths[u].size() != path.size() )
        {
            continue;
        }

        for (int i = 0; i < paths[u].size(); i++)
        {
            for (int v = 0; v < path.size(); v++)
            {
                if (paths[u][i] != path[v])
                {
                    eq = false;
                    break;
                }
                else
                {
                    eq = true;
                }
            }

            if (eq == false)
            {
                break;
            }
        }

        if (eq == true)
        {
            return eq;
        }
    }

    return eq;
}

void BalancedTree::addChildren(Graph g,TreeNode *root,int source,int target, vector< vector<int> > &paths)
{
    Node node;
    node = g.getNodeAtPosition(root->index);

    vector <int> adjacents = node.getAdjacentsNodes();

    for (int i = 0; i < adjacents.size(); i++)
    {
        if (!isNodeInPath(root,adjacents[i]))
        {
            TreeNode *child = new TreeNode(adjacents[i]);
            child->parent = root;
            root->addChild(child);

            if (adjacents[i] == target)
            {
                vector<int> temp = returnPath(child);

                if (!searchPath(paths,temp))
                {
                    paths.push_back(temp);
                }
            }
            else
            {
                addChildren(g,child,source,target,paths);
            }
        }
    }

}

vector< vector<int> > BalancedTree::findAllPaths(Graph g,int source,int target)
{
    vector< vector<int> > paths;

    TreeNode *root = new TreeNode(source);//a raíz inicia pela origem do caminho e as folhas serão sempre o destino

    addChildren(g,root,source,target,paths);//adiciona filhos na árvore

    return paths;
} 

void BalancedTree::findPairOfBalancedPaths(Graph g,int source,int target)
{
    vector< vector<int> > pairOfPaths;
    
    pairOfPaths = findAllPaths(g,source,target);
    
    int sum = g.getNumberOfNodes()+1;//somatório dos caminhos mínimos encontrados pelo algoritmo
    int diff = sum+1; //iniciando com número infinito
    int a = 0, b = 0;

    for (unsigned int i = 0; i < pairOfPaths.size()-1; i++)
    {
    	// for (unsigned int k = 0; k < pairOfPaths[i].size(); k++) cout<<" "<<pairOfPaths[i][k];
    	// cout<<endl;
        
        for (unsigned int j = i+1; j < pairOfPaths.size(); j++)
        {
            if (pairOfPaths[i][1] == pairOfPaths[j][1] || pairOfPaths[i][(int)pairOfPaths[i].size()-2] == pairOfPaths[j][(int)pairOfPaths[j].size()-2])
            {
                continue;
            }
            
            vector< vector<int> > paths = compareWithOthers(g,pairOfPaths[i],pairOfPaths[j]);
            
            if ((int)paths.size() <= 0)
            {
               continue;
            }

            int newDiff = abs( (int)paths[0].size() - (int)paths[1].size() );
            int s = ( (int)paths[0].size() + (int)paths[1].size() )/2;
            
            // cout<<"s "<<s<<" sum "<< sum<<" path1 "<< paths[0].size()<<" path2 "<<paths[1].size()<<" diff "<<diff<<" size "<<paths.size()<<endl;      
            if ( s < sum )
            {  
                a = i;
                b = j;

                diff = newDiff;
                sum = s;
            }
            else if (s == sum)
            {
                if (newDiff < diff)
                {
                    a = i;
                    b = j;

                    diff = newDiff;
                    sum = s;
                }
            }
        }
    }
    // cout<<"--------------------------------\n\n";

    if (a != b)
    {
        vector< vector<int> > paths = compareWithOthers(g,pairOfPaths[a],pairOfPaths[b]);

    	printPaths(paths[0],paths[1], g);
    }
    else
    {
        cout<<"Topologia não sobrevivente."<<endl;
        exit(1);
    }
}

vector<int> BalancedTree::returnPath(TreeNode *child)
{
    vector<int> path;

    TreeNode *temp = child;

    while(temp)
    {
        path.push_back(temp->index);

        temp = temp->parent;
    }

    reverse(path.begin(),path.end());

    return path;
}

void BalancedTree::makePathVector(vector<int> p1,vector<int> &p2, vector<int> &inPath)
{
    for ( unsigned int u = 0; u < p1.size()-1; u++)
    {
        p2.push_back( p1[u] );
        p2.push_back( p1[u+1] );

        inPath[ p1[u] ] = p1[u];
        inPath[ p1[u+1] ] = p1[u+1];
    }
}


void BalancedTree::freeTree(TreeNode *root)
{
    for (int i = 0; i < root->children.size(); i++)
    {
        freeTree(root->children[i]);
    }

    delete root;
}                                          

void BalancedTree::discardCommonEdge(vector<int> &p1, vector<int> &p2, int x, int y)
{
    vector<int> t1, t2;
    unsigned int u = 0, v = 0;
  
    for (u = 0; u < p1.size(); u += 2)
    {
        if( u == x )
        {
            break;
        }

        t1.push_back(p1[u]);
        t1.push_back(p1[u+1]);
    }
   
    for (u = 0; u < p2.size(); u += 2)
    {
        if( u == y ) {
            break;
        }
        
        t2.push_back(p2[u]);
        t2.push_back(p2[u+1]);
    }

    v = 0;

    for (u = y+2; u < p2.size(); u += 2)
    {
        if (p2[u] == t1[t1.size() - 1] && p2[u+1] == t1[t1.size() - 2])
        {
            t1.erase( t2.begin() + t1.size() - 1 );
            t1.erase( t2.begin() + t1.size() - 1 );
            continue;
        }

        t1.push_back(p2[u]);
        t1.push_back(p2[u+1]);
    }
  
    v = 0;
    for (u = x+2; u < p1.size(); u += 2)
    {
        if (p1[u] == t2[t2.size() - 1] && p1[u+1] == t2[t2.size() - 2])
        {
            t2.erase( t2.begin() + t2.size() - 1 );
            t2.erase( t2.begin() + t2.size() - 1 );
            continue;
        }

        t2.push_back(p1[u]);
        t2.push_back(p1[u+1]);

        v = t2.size();
    }

    p1.clear();
    p2.clear();

    for (u = 0; u < t1.size(); u++)
    {
        p1.push_back(t1[u]);
    }

    for (u = 0; u < t2.size(); u++)
    {
        p2.push_back(t2[u]);
    }
}

void BalancedTree::printPaths(vector<int> p1,vector<int> p2, Graph &graph)
{
    int u = 0;

    this->datas<<p1.size()/2<<" ";
    this->datas<<p1[0]<<" ";
    for (u = 1; u < p1.size(); u+=2)
    {
        
        this->datas<<p1[u]<<" ";
    }

    this->datas<<"\n";
    
    this->datas<<p2.size()/2<<" ";
    this->datas<<p2[0]<<" ";
    for (u = 1; u < p2.size(); u+=2)
    {
      
        this->datas<<p2[u]<<" ";
    }
    this->datas<<"\n";


   if (p1.size() > p2.size())
   {
      this->hopBackup.push_back(p1.size()/2);
      this->hopWorking.push_back( p2.size()/2);
      // graph.setWorkingPath(p2);
      // graph.setBackupPath(p1);
   }
   else
   {
        this->hopBackup.push_back( p2.size()/2);
        this->hopWorking.push_back( p1.size()/2);
        // graph.setWorkingPath(p1);
        // graph.setBackupPath(p2);
   }
}

void BalancedTree::execute(Graph &graph, string file)
{
	file = "output_balanced_"+file;

	this->datas.open(file);
    vector<thread> t;

	/**
	 * Encontra o par de caminhos disjuntos menores e com diferença de hops menor
	 * em relação aos demais caminhos.
	 */
	for (int i = 0; i < graph.getNumberOfNodes()-1; i++)
	{
		for (int j = i+1; j < graph.getNumberOfNodes(); j++)
		{
			t.push_back(thread( [this, graph, i, j] { this->findPairOfBalancedPaths(graph,i,j); }));
		}
	}

    for (unsigned int i = 0; i < t.size(); i++)
    {
        t[i].join();
    }

    averageHops(graph);
}