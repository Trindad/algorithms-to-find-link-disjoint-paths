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

#include "PairOfDisjointPaths.hpp"
// #include "Backtrack.hpp"
#include "DFS.hpp"

PairOfDisjointPaths::PairOfDisjointPaths(){}
PairOfDisjointPaths::~PairOfDisjointPaths(){}

void PairOfDisjointPaths::removeUnnecessaryPaths(vector<int> p1,vector<int> p2, vector<pair<int,int>> &distance)
{
    int t = 0;

    if ((int)p1.size() > (int)p2.size())
    {
       t = (int) p1.size();
    }
    else
    {
        t = (int) p2.size();
    }

    int n = (int) distance.size();
    /**
     * Exclui caminhos desnecessários
     * Ou seja, que são maiores que o maior caminho do ciclo
     */
    for (int i = n-1; i >= 0; i--)
    {
        if (distance[i].second >= t)
        {
            distance.erase(distance.begin()+i);
        }
        else
        {
            break;
        }
    }
}

/**
 * Remove todas as arestas do grafo que pertencem ao caminho
 */
void PairOfDisjointPaths::removePath(Graph &g, vector<int> p)
{
    int n = (int)p.size()-1;

    for (int i = n; i > 0; i--)
    {
        g.removeNode(p[i],p[i-1]);
        g.removeNode(p[i-1],p[i]);
    }
}

void PairOfDisjointPaths::averageHops(Graph g)
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

    cout<<endl;
    avgHops.clear();
}

bool PairOfDisjointPaths::isNodeInPath(TreeNode *node,int index)
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

bool PairOfDisjointPaths::searchPath(vector< vector<int> > paths, vector<int> path)
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

void PairOfDisjointPaths::addChildren(vector<pair<int,int>> &distance,Graph g,TreeNode *root,int source,int target, vector< vector<int> > &paths)
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
                    // for (unsigned int j = 0; j < temp.size(); j++)
                    // {
                    //     cout<<" "<<temp[j];
                    // }
                    // cout<<endl;

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

vector< vector<int> > PairOfDisjointPaths::findAllPaths(vector<pair<int,int>> &distance,Graph g,int source,int target, int limit)
{
    // vector< vector<int> > paths;

    // TreeNode *root = new TreeNode(source);//a raíz inicia pela origem do caminho e as folhas serão sempre o destino

    // addChildren(distance,g,root,source,target,paths);//adiciona filhos na árvore

    // freeTree(root);
    // int a[NMAX];
    
    // for (int p = 0; p < NMAX; p++)
    // {
    //     a[p] = -1;
    // }
    
    // Backtrack b(g,source);

    // b.execute(a, source-1, target, limit);
    // // cout << "PRINTING PAAAAATHS!!!!!!" << source << " " << target << endl;
    // for (int i = 0; i < (int)b.all_paths.size(); i++)
    // {
    //     for (int k = 0; k < (int)b.all_paths[i].size(); k++)
    //     {
    //         cout << b.all_paths[i][k] << " ";
    //     }

    //     cout << endl;
    // }
    // // cout<<"-----------------------------------------"<<endl;
    // int hops = 0;

    // for (int i = 0; i < (int)b.all_paths.size(); i++)
    // {
    //     hops = (int)b.all_paths[i].size();
    //     distance.push_back( pair<int, int>(i,hops-1) );
    // }

    // return b.all_paths;
    
    int n = g.getNumberOfNodes(),hops = 0;
   
    DFS d(n);

    vector<vector<int>> paths;
    d.execute(source,target,0,g,limit,paths); 

    for (int i = 0; i < (int)paths.size(); i++)
    {
        hops = (int)paths[i].size();
        distance.push_back( pair<int, int>(i,hops-1) );
    }

    return paths;
} 

/**
 * Ordena vetor de pares
 */
void PairOfDisjointPaths::sortDatas(vector<pair<int,int>> &distance)
{
    sort(distance.begin(),distance.end(),[](const pair<int,int> &left,const pair<int,int> &right){
        return left.second < right.second;
    });
}

vector<int> PairOfDisjointPaths::returnPath(TreeNode *child)
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

void PairOfDisjointPaths::makePathVector(vector<int> p1,vector<int> &p2, vector<int> &inPath)
{
    for ( unsigned int u = 0; u < p1.size()-1; u++)
    {
        p2.push_back( p1[u] );
        p2.push_back( p1[u+1] );

        inPath[ p1[u] ] = p1[u];
        inPath[ p1[u+1] ] = p1[u+1];
    }
}


void PairOfDisjointPaths::freeTree(TreeNode *root)
{
    for (unsigned int i = 0; i < root->children.size(); i++)
    {
        freeTree(root->children[i]);
    }

    delete root;
}                                          

void PairOfDisjointPaths::printPaths(vector<int> p1,vector<int> p2, Graph &graph)
{

    vector<vector<int>> paths;

    paths.push_back(p1);
    paths.push_back(p2);

    this->ap.lock();
    this->allPaths.push_back(paths);
    this->ap.unlock();

    // this->datas<<p1.size()/2<<" ";
    // this->datas<<p1[0]<<" ";

    // for (u = 1; u < p1.size(); u+=2)
    // {
        
    //     this->datas<<p1[u]<<" ";
    // }

    // this->datas<<"\n";
    
    // this->datas<<p2.size()/2<<" ";
    // this->datas<<p2[0]<<" ";
    // for (u = 1; u < p2.size(); u+=2)
    // {
      
    //     this->datas<<p2[u]<<" ";
    // }
    // this->datas<<"\n";


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


bool PairOfDisjointPaths::execute(Graph &graph, string file)
{

    this->datas.open(file);
    // createDigraph(graph);
    // vector<thread> t;

	/**
	 * Encontra o par de caminhos disjuntos menores e com diferença de hops menor
	 * em relação aos demais caminhos.
	 */
	for (int i = 0; i < graph.getNumberOfNodes()-1; i++)
	{
		
		// t.push_back(thread( [this, &graph, i] {
  //           for (int j = i+1; j < graph.getNumberOfNodes(); j++)
  //           { 
  //               this->findPairOfBalancedPaths(graph,i,j); 
  //           }
  //       })); 
        for (int j = i+1; j < graph.getNumberOfNodes(); j++)
        {
            // cout<<" "<<i<<" "<<j<<" "<<endl;
            bool survivor = findPairOfBalancedPaths(graph,i,j);

            if (survivor == false)
            {
                return false;
            }
        }
	}

    // for (unsigned int i = 0; i < t.size(); i++)
    // {
    //     t[i].join();
    // }

    writeFile();
    averageHops(graph);

    return true;
}

void PairOfDisjointPaths::writeFile()
{
    for (unsigned int i = 0; i < this->allPaths.size(); i++)
    {
        this->datas<<(int)this->allPaths[i][0].size()/2<<" ";
        this->datas<<this->allPaths[i][0][0]<<" ";
        for (unsigned int j = 1; j < this->allPaths[i][0].size(); j+=2)
        {
            this->datas<<this->allPaths[i][0][j]<<" ";
        }
        this->datas<<endl;
        this->datas<<(int)this->allPaths[i][1].size()/2<<" ";
        this->datas<<this->allPaths[i][1][0]<<" ";
        for (unsigned int j = 1; j < this->allPaths[i][1].size(); j+=2)
        {
            this->datas<<this->allPaths[i][1][j]<<" ";
        }
        this->datas<<endl;
    }
}

// // Create a Digraph given in the above diagram
// void PairOfDisjointPaths::createDigraph(Graph graph)
// {
//     int nNodes = graph.getNumberOfNodes();

//     this->digraph.setNumberOfNodes(nNodes);

//     vector<int> adjacents;
//     vector<Node> node = graph.getNodes();

//     for (int u = 0; u < nNodes ; u++)
//     {
//         adjacents = node[u].getAdjacentsNodes();

//         for (int v = 0; v < (int)adjacents.size(); v++)
//         {
//             this->digraph.addEdge(u, adjacents[v]);
//             this->digraph.addEdge(adjacents[v],u);
//         }

//     }
    
//     adjacents.clear();
// }