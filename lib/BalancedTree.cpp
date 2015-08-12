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

int BalancedTree::compareWithOthers(Graph g,vector<int> p1, vector<int> p2) 
{

    vector<int> temp = vector<int> (g.getNumberOfNodes(),-1);
    vector<int> path1;
    vector<int> path2;

    makePathVector(p1,path1,temp);
    makePathVector(p2,path2,temp);

    for (int i = 0; i < path1.size()-1; i+=2)
    {
        for (int j = 0; j < path2.size()-1; j+=2)
        {
           if (path1[i] == path2[j+1] && path1[i+1] == path2[j])
           {
               return -1;
           }
        }
    }

    return abs(p1.size()-p2.size());
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
                paths.push_back(returnPath(child));
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

vector< vector<int> > BalancedTree::findPairOfBalancedPaths(Graph g,int source,int target)
{
    vector< vector<int> > pairOfPaths;
    
    pairOfPaths = findAllPaths(g,source,target);
    
    int sum = g.getNumberOfNodes();//somatório dos caminhos mínimos encontrados pelo algoritmo
    int diff = sum+1, diffAnt, a = 0, b = 0, s = sum+1;
    diffAnt = diff;

    for (int i = 0; i < pairOfPaths.size(); i++)
    {
        for (int j = 0; j < pairOfPaths.size(); j++)
        {
            
            if (i == j)
            {
                continue;
            }
            else
            {
                s = (pairOfPaths[i].size()-1) + (pairOfPaths[j].size()-1);
            }

       
            if ( s <= sum )
            {
                int newDiff = compareWithOthers(g,pairOfPaths[i],pairOfPaths[j]);
                
                /**
                 * Atribui indices dos novos caminhos
                 */
                if ( ( newDiff < diff ) && ( newDiff >= 0 ) )
                {
                    a = i;
                    b = j;

                    diff = newDiff;
                }

            }
            
            s = sum+1;

        }
    }

    vector< vector <int> > pair;

    if (a != b && diff < diffAnt)
    {
        pair.push_back( pairOfPaths[a] );
        pair.push_back( pairOfPaths[b] );
    }

    return pair;
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
      graph.setWorkingPath(p2);
      graph.setBackupPath(p1);
   }
   else
   {
        this->hopBackup.push_back( p2.size()/2);
        this->hopWorking.push_back( p1.size()/2);
        graph.setWorkingPath(p1);
        graph.setBackupPath(p2);
   }
}

void BalancedTree::execute(Graph &graph, string file)
{
	file = "output_balanced_"+file;

	this->datas.open(file);
	/**
	 * Encontra o par de caminhos disjuntos menores e com diferença de hops menor
	 * em relação aos demais caminhos.
	 */
	for (int i = 0; i < graph.getNumberOfNodes()-1; i++)
	{
		for (int j = i+1; j < graph.getNumberOfNodes(); j++)
		{
			findPairOfBalancedPaths(graph,i,j);
		}
	}
}