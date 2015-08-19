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

    for (unsigned int u = 0; u < paths.size(); u++)
    {
        if (paths[u].size() != path.size() )
        {
            continue;
        }

        for (unsigned int i = 0; i < paths[u].size(); i++)
        {
            for (unsigned int v = 0; v < path.size(); v++)
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

void BalancedTree::addChildren(vector<pair<int,int>> &distance,Graph g,TreeNode *root,int source,int target, vector< vector<int> > &paths)
{
    Node node;
    node = g.getNodeAtPosition(root->index);

    vector <int> adjacents = node.getAdjacentsNodes();

    for (unsigned int i = 0; i < adjacents.size(); i++)
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

                    int n = (int)paths.size()-1;
                    int h = (int)temp.size();//número de hops

                    distance.push_back( pair<int, int>(n,h-1) );
                }
            }
            else
            {
                addChildren(distance,g,child,source,target,paths);
            }
        }
    }

    adjacents.clear();
}

vector< vector<int> > BalancedTree::findAllPaths(vector<pair<int,int>> &distance,Graph g,int source,int target)
{
    vector< vector<int> > paths;

    TreeNode *root = new TreeNode(source);//a raíz inicia pela origem do caminho e as folhas serão sempre o destino

    addChildren(distance,g,root,source,target,paths);//adiciona filhos na árvore


    return paths;
} 

/**
 * Ordena vetor de pares
 */
void BalancedTree::sortDatas(vector<pair<int,int>> &distance)
{
    sort(distance.begin(),distance.end(),[](const pair<int,int> &left,const pair<int,int> &right){
        // cout<<" "<<left.second<<" "<<right.second<<endl;
        return left.second < right.second;
    });
}

void BalancedTree::findPairOfBalancedPaths(Graph g,int source,int target)
{
    vector< vector<int> > pairOfPaths;
    vector<pair<int,int>> distance;
    

    pairOfPaths = findAllPaths(distance,g,source,target);
    
    sortDatas(distance);//ordena vetor de pares
    
    int sum = g.getNumberOfNodes()+1;//somatório dos caminhos mínimos encontrados pelo algoritmo
    int diff = sum+1; //iniciando com número infinito
    int a = 0, b = 0;

    for (unsigned int i = 0; i < distance.size()-1; i++)
    {
        for (unsigned int j = i+1; j < distance.size(); j++)
        {
            int u = distance[i].first;
            int v = distance[j].first;

            if (pairOfPaths[u][1] == pairOfPaths[v][1])
            {
                continue;
            }

            if (pairOfPaths[u][(int)pairOfPaths[u].size()-2] == pairOfPaths[v][(int)pairOfPaths[v].size()-2])
            {
                continue;
            }
            
            vector< vector<int> > paths = compareWithOthers(g,pairOfPaths[u],pairOfPaths[v]);
            
            if ((int)paths.size() <= 0)
            {
               continue;
            }

            int newDiff = abs( (int)paths[0].size() - (int)paths[1].size() );
            int s = ( (int)paths[0].size() + (int)paths[1].size() )/2;
            
            if ( s > sum && (a != b) )
            {
                break;
            }
            else if ( s < sum )
            {  
                a = u;
                b = v;

                diff = newDiff;
                sum = s;
            }
            else if (s == sum)
            {
                if (newDiff < diff)
                {
                    a = u;
                    b = v;

                    diff = newDiff;
                    sum = s;
                }
            }
        }
    }

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

    distance.clear();
    pairOfPaths.clear();
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
    for (unsigned int i = 0; i < root->children.size(); i++)
    {
        freeTree(root->children[i]);
    }

    delete root;
}                                          

void BalancedTree::discardCommonEdge(vector<int> &p1, vector<int> &p2, int x, int y)
{
    vector<int> t1, t2;
    unsigned int u = 0;
  
    for (u = 0; u < p1.size(); u += 2)
    {
        if( (int)u == x )
        {
            break;
        }

        t1.push_back(p1[u]);
        t1.push_back(p1[u+1]);
    }
   
    for (u = 0; u < p2.size(); u += 2)
    {
        if( (int)u == y ) {
            break;
        }
        
        t2.push_back(p2[u]);
        t2.push_back(p2[u+1]);
    }

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
    unsigned int u = 0;

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
        this->m.lock();
        this->hopBackup.push_back( (double)p1.size()/2);
        this->hopWorking.push_back(  (double)p2.size()/2);
        this->m.unlock();
   }
   else
   {
        this->m.lock();
        this->hopBackup.push_back(  (double)p2.size()/2);
        this->hopWorking.push_back(  (double)p1.size()/2);
        this->m.unlock();
   }
}

void BalancedTree::execute(Graph &graph, string file)
{
	file = "output_best_balanced_"+file;

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