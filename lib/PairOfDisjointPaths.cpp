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
#include "Backtrack.hpp"


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



void PairOfDisjointPaths::shortestPathNode(Graph g, Graph &t,vector< vector <int> > &p,int source, int target)
{
    t = g;//grafo temporario
    Dijkstra dijkstra;

    dijkstra.execute(t,source,target);

    vector<int> temp = dijkstra.shortestPath(target);

    if (temp.size() >= 2 )
    {
        p.push_back( temp );

        removePath(t,p[0]);
    }
    else
    {
        cout<<"Topologia não sobrevivente"<<endl;
        exit(1);
    }
}

void PairOfDisjointPaths::shortestPathEdge(Graph g, Graph &t,vector< vector <int> > &p,int source, int target)
{
    // cout<<"AQUI"<<endl;
    t = g;//grafo temporario
    Dijkstra dijkstra;

    int h =  dijkstra.execute(t,source,target);

    vector<int> temp = dijkstra.shortestPath(target);

    if (temp.size() >= 2 )
    {
        p.push_back( temp );

        t.removeNode(source,p[1][1]);
    }
    else
    {
        cout<<"Topologia não sobrevivente"<<endl;
        exit(1);
    }
}

vector<vector<int>> PairOfDisjointPaths::shortestPaths(Graph g, vector<pair<int,int>> &distance, int source, int target)
{
    Dijkstra dijkstra;
    Graph t = g;
    int n = 0, u = 0, v = 0, h = 0, a = 0, b = 0;

    vector< vector <int> > p;

    shortestPathNode(g,t,p,source,target);//encontra dois caminhos minimos se existir
    /**
     * Encontra segundo caminho mínimo disjunto por nó
    */

    h = dijkstra.execute(t,source,target);

    vector<int> temp = dijkstra.shortestPath(target);
    
    if (temp.size() >= 2 )
    {
        p.push_back( temp );
        
        v = (int)p[1].size();
        u = (int)p[0].size();
        a = 0;
        b = 1;
    }
    else
    {
        shortestPathEdge(g,t,p,source,target);
        
        u = (int)p[1].size();

        shortestPathNode(g,t,p,source,target);//encontra dois caminhos minimos se existir
        /**
         * Encontra segundo caminho mínimo disjunto por nó
        */

        h = dijkstra.execute(t,source,target);

        vector<int> temp = dijkstra.shortestPath(target);
        
        if (temp.size() >= 2 )
        {
            p.push_back( temp );
        }

        v = (int)p[2].size();
        a = 1;
        b = 2;
    }

    /**
     * Encontrando novos pares de caminhos minimos a partir da remoção das arestas do target
     */
    t = g;


    t.removeNode(p[a][u-2], target);
    t.removeNode(target,p[a][u-2]);

    t.removeNode(p[b][1], source);
    t.removeNode(source,p[b][1]);

    h = dijkstra.execute(t,source,target);

    temp = dijkstra.shortestPath(target);
    
    if (temp.size() >= 2 )
    {
        p.push_back( temp );
    }
    
    /**
     * ------------------------------------------------------
     */

    t = g;

    t.removeNode(p[b][v-2], target);
    t.removeNode(target,p[b][v-2]);

    t.removeNode(p[a][1], source);
    t.removeNode(source,p[a][1]);

    h = dijkstra.execute(t,source,target);

    temp = dijkstra.shortestPath(target);

    if (temp.size() >= 2 )
    {
        p.push_back( temp );
    }

    /**
     * ------------------------------------------------------
     */
    
    t = g;

    removePath(t,p[a]);

    t.setEdge(p[a][0],p[a][1]);

    if ((int)p[a].size() > 3)
    {
        t.setEdge(p[a][1],p[a][2]);

        t.setWeight(p[a][1],p[a][2],1.0);
        t.setWeight(p[a][2],p[a][1],1.0);
    }
    t.setWeight(p[a][0],p[a][1],1.0);
    t.setWeight(p[a][1],p[a][0],1.0);

    t.removeNode(p[b][1], source);
    t.removeNode(source,p[b][1]);

    h = dijkstra.execute(t,source,target);

    temp = dijkstra.shortestPath(target);

    if (temp.size() >= 2 )
    {
        p.push_back( temp );
    }
    else
    {
        cout<<"AQUI "<<p[a][0]<<" "<<p[a][1]<<endl;
    }

     /**
     * ------------------------------------------------------
     */
    
    t = g;

    removePath(t,p[b]);

    t.setEdge(p[b][0],p[b][1]);

    if ((int)p[b].size() > 3)
    {
        t.setEdge(p[b][1],p[b][2]);

        t.setWeight(p[b][1],p[b][2],1.0);
        t.setWeight(p[b][2],p[b][1],1.0);
    }

    t.setWeight(p[b][0],p[b][1],1.0);
    t.setWeight(p[b][1],p[b][0],1.0);

    t.removeNode(p[a][1], source);
    t.removeNode(source,p[a][1]);

    h = dijkstra.execute(t,source,target);

    temp = dijkstra.shortestPath(target);

    if (temp.size() >= 2 )
    {
        p.push_back( temp );
    }
    else
    {
        cout<<"AQUI "<<p[b][0]<<" "<<p[b][1]<<endl;
    }



    /**
     * ------------------------------------------------------
     */
    cout<<endl;
    for (unsigned int i = 0; i < p.size(); i++)
    {
        for (unsigned int j = 0; j < p[i].size(); j++)
        {
            cout<<" "<<p[i][j];
        }
        cout<<endl;
    }
    

    Graph graph;

    graph.setUseEdgeWeight(false);

    n = g.getNumberOfNodes();
    graph.setNumberOfNodes(n);

    graph.setMinimumDegree(2);
    graph.setMaximumDegree(n -1);

    n = (int)p.size();

    for (int i = 0; i < n; i++)
    {
        int N = (int)p[i].size();

        for (int j = N-1; j > 0; j--)
        {
            u = p[i][j];
            v = p[i][j-1];

            if (graph.getEdge(u,v) == false)
            {
                graph.setEdge(u,v);
                graph.setWeight(u,v,1.0);
                graph.setWeight(v,u,1.0);
            }
        }
    }

    for (int i = 0; i < n; i++)
    {
        int N = (int)p[i].size();

        for (int j = N-1; j > 0; j--)
        {
            u = p[i][j];
            v = p[i][j-1];

            if (graph.getEdge(u,v) == false)
            {
                graph.setEdge(u,v);
                graph.setWeight(u,v,1.0);
                graph.setWeight(v,u,1.0);
            }
        }
    }

    p.clear();


    vector<vector<int>> paths;

    paths = findAllPaths(distance,graph,source,target);

    // cout<<endl;
    // for (unsigned int i = 0; i < paths.size(); i++)
    // {
    //     for (unsigned int j = 0; j < paths[i].size(); j++)
    //     {
    //         cout<<" "<<paths[i][j];
    //     }
    //     cout<<endl;
    // }

    return paths;
    // return p;
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

vector< vector<int> > PairOfDisjointPaths::findAllPaths(vector<pair<int,int>> &distance,Graph g,int source,int target)
{
    // vector< vector<int> > paths;

    // TreeNode *root = new TreeNode(source);//a raíz inicia pela origem do caminho e as folhas serão sempre o destino

    // addChildren(distance,g,root,source,target,paths);//adiciona filhos na árvore

    // freeTree(root);
    int a[NMAX];
    for (int p = 0; p < NMAX; p++)
    {
        a[p] = -1;
    }
    
    Backtrack b(g,source);
    // cout<<endl;
    b.execute(a, source-1, target);
    // cout << "PRINTING PAAAAATHS!!!!!!" << source << " " << target << endl;
    // for (int i = 0; i < b.all_paths.size(); i++)
    // {
    //     for (int k = 0; k < b.all_paths[i].size(); k++)
    //     {
    //         cout << b.all_paths[i][k] << " ";
    //     }

    //     cout << endl;
    // }
    int hops = 0;

    for (int i = 0; i < b.all_paths.size(); i++)
    {
        hops = (int)b.all_paths[i].size();
        distance.push_back( pair<int, int>(i,hops-1) );
    }
    
    // paths = this->digraph.printAllPaths(distance,source,target);

    return b.all_paths;
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

void PairOfDisjointPaths::dfs(vector<pair<int,int>> &distance,Graph &graph, int start, int end, vector< vector<int> > &paths, vector<int> &path)
{
    Node n;
    n = graph.getNodeAtPosition(start);

    vector <int> adjacents = n.getAdjacentsNodes();

    for (unsigned int i = 0; i < adjacents.size(); i++)
    {
        int node = adjacents[i];

        vector<int>::iterator it;
        it = find (path.begin(), path.end(), node);
        
        if (it == path.end())
        {
            path.push_back(start);

            if (node == end)
            {
                for (unsigned int i = 0; i < path.size(); i++)
                {
                    cout<<" "<<path[i];
                }
                cout<<endl;
                paths.push_back(path);

                int index = (int)paths.size()-1;
                int h = (int)path.size()-1;//número de hops
                
                distance.push_back( pair<int, int>(index,h) );

                path.clear();
            }
            else
            {
                adjacents.clear();
                dfs(distance,graph,node,end,paths,path);
            }   
        }
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