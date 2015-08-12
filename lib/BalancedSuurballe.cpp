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

/**
 * Algoritmo de roteamento
 */

#include "BalancedSuurballe.hpp"

BalancedSuurballe::BalancedSuurballe(){}

BalancedSuurballe::~BalancedSuurballe(){}

void BalancedSuurballe::insertSubtree(Graph graph, tree<int> &tr, typename tree<int>::iterator root,vector<int> nodes, vector<int> &controller, int source)
{
    Node node = graph.getNodeAtPosition(source);
    vector<int> adjacents = node.getAdjacentsNodes();
    typename tree<int>::iterator temp;
    typename tree<int>::iterator newRoot;
    int newSource = source;

    if (find(this->nodeInTree.begin(),this->nodeInTree.end(),source) == this->nodeInTree.end())
    {
        this->nodeInTree.push_back(source);
    }

    int it = 0;

    while( adjacents.size() > it )
    {
        if (controller[ adjacents[it] ] == -1)
        {
            temp = tr.append_child( root, adjacents[it] );

            controller[ adjacents[it] ] = adjacents[it];

            if (find(this->nodeInTree.begin(),this->nodeInTree.end(),adjacents[it]) == this->nodeInTree.end())
            {
                this->nodeInTree.push_back(adjacents[it]);
            }

            if( find(nodes.begin(),nodes.end(),adjacents[it]) != nodes.end() )
            {
                newRoot = temp;
                newSource = adjacents[it];
            }
        }


        it++;
    }

    int n = nodes.size()-1;

    if (source == nodes[n] )
    {
        return;
    }

    insertSubtree(graph,tr,newRoot,nodes,controller,newSource);
}

tree<int> BalancedSuurballe::makeTree(Graph graph, vector<int> nodes, int source)
{
    // cout<<"makeTree"<<endl;
    // for (unsigned int i = 0; i < nodes.size(); ++i)
    // {
    //     cout<<nodes[i]<<" ";
    // }
    // cout<<endl;
    vector<Node> auxiliar;
    auxiliar = graph.getNodes();

    tree<int> tr;

    tree<int>::iterator root, top, parent;

    top = tr.begin();
    root = tr.insert( top, source );

    vector<int> controller = vector<int> (this->numberOfNodes,-1);

    controller[source] = source;

    insertSubtree(graph,tr,root,nodes,controller,source);

    // cout<<"----------ARVORE--------------"<<endl;
    // kptree::print_tree_bracketed(tr,cout); //imprime árvore
    // cout<<endl;

    // for (unsigned int i = 0; i < this->nodeInTree.size(); ++i)
    // {
    //     cout<<this->nodeInTree[i]<<" ";
    // }
    // cout<<endl;

    return tr;
}

/**
 * Atualiza peso dos nós da árvore para 0
 * e as demais ligações aplica a função:
 * w'(u,v) = w (w,u) - d(s,v) + d(s,u) 
 */
void BalancedSuurballe::updateEdgesWeight(const tree<int>& t, typename tree<int>::iterator iRoot, vector<int> nodes, Graph & graph, int source)
{
    if( t.empty() )
    {
        return;
    }

    if (t.number_of_children(iRoot) == 0)
    {
        return;
    }
    else
    {
        // child1, ..., childn
        int siblingCount = t.number_of_siblings( t.begin(iRoot) );
        int siblingNum;

        typename tree<int>::sibling_iterator iChildren;
        double weight = 0.0f ;//w'(u,v) = w (w,u) - d(s,v) + d(s,u)

        for (iChildren = t.begin(iRoot), siblingNum = 0; iChildren != t.end(iRoot); ++iChildren, ++siblingNum)
        {
            weight = 0.0f;
            int u = *iRoot, v = *iChildren;
              // cout<<"\n----------------------------------------"<<endl;   
             /**
             * Remove arestas do caminho mínimo de ida
             * Deixando somente as arestas de volta
             */
            // cout<<" ( "<<u<<" , "<<v<<" )"<<endl;
            if ( (nodes[ u ] == u || u == source) && ( nodes[ v ] == v || v == source) )
            {
                graph.setWeightEdgeDirected(u,v,weight);
                // cout<<"w("<<u<<" , "<<v<<" ) = "<<weight<<endl;

                graph.setWeightEdgeDirected(v,u,weight);
                // cout<<"w("<<v<<" , "<<u<<" ) = "<<weight<<endl;

                graph.removeNode(u,v);
            }
            if (graph.getUseEdgeWeight())
            {
                /* code */
            }
            if (find(this->nodeInTree.begin(),this->nodeInTree.end(),u) != this->nodeInTree.end() && find(this->nodeInTree.begin(),this->nodeInTree.end(),v) != this->nodeInTree.end() )
            {
                graph.setWeightEdgeDirected(u,v,weight);
                // cout<<"w'("<<u<<" , "<<v<<" ) = "<<weight<<endl;

                weight = this->distance[v][u] - this->distance[source][u] + this->distance[source][v];
                graph.setWeightEdgeDirected(v,u,weight);
                // cout<<"w'("<<v<<" , "<<u<<" ) = "<<weight<<endl;
                
            }
            else
            {
                weight = this->distance[u][v] - this->distance[source][v] + this->distance[source][u];
                // cout<<"w'("<<u<<" , "<<v<<" ) = "<<weight<<endl;
                graph.setWeightEdgeDirected(u,v,weight);

                u = *iChildren, v = *iRoot;

                weight = this->distance[u][v] - this->distance[source][v] + this->distance[source][u];

                graph.setWeightEdgeDirected(u,v,weight);

                // cout<<"w''("<<u<<" , "<<v<<" ) = "<<weight<<endl;
            }
            // cout<<"( "<<u<<", "<<adjacents[w]<<" )"<<endl;

            this->treePath[v][u] = 1;
            this->treePath[u][v] = 1;

            updateEdgesWeight(t,iChildren,nodes,graph,source);
        }
    }
    // cout<<"tamanho "<<this->nodeInTree.size()<<endl;
    // for (unsigned int i = 0; i < this->nodeInTree.size(); i++)
    // {
    //     cout<<" "<<this->nodeInTree[i];
    // }
    // cout<<endl;
}

/**
 * Todas as arestas da árvore receberão peso 0
 * Os demais nós será aplicado a fórmula proposta:
 * 		w'(u,v) = w (w,u) - d(s,v) + d(s,u)
 */
void BalancedSuurballe::changeEdgesWeights(Graph & graph, tree<int> tr, vector<int> nodes)
{
    int source = *tr.begin();

    /**
     * Percore árvore atualizando o peso das ligações u --> v
     * Se o nodo esta no caminho o peso passa a ser 0 (zero)
     * Do contrário aplica a equação proposta por BalancedSuurballe
     * Sendo s o source e u e v nós pertencentes a árvore
     * 		w'(u,v) = w (u,v) - d(s,v) + d(s,u)
     */

    int headCount = tr.number_of_siblings(tr.begin());//número de cabeças da árvore

    typename tree<int>::sibling_iterator iRoot = tr.begin();


    unsigned int count = 0;

    vector<int> temp = vector<int> (this->numberOfNodes,-1);

    while( count < nodes.size() )
    {
        temp[ nodes[count] ] = nodes[count];

        count++;
    }

    updateEdgesWeight(tr,iRoot,temp,graph,source);//atualiza peso e remove ligações

    temp.clear();
    // nodes.clear();
    /**
     * Atualiza nós não pertencentes a árvore
     */
    // vector<Node> n = graph.getNodes();
    // for (unsigned int u = 0; u < this->nodeInTree.size(); u++)
    // {
    //     vector<int> adjacents = n[u].getAdjacentsNodes();

    //     for (unsigned int v = 0; v < adjacents.size(); v++)
    //     {
    //         double weight = 0.0f;
    //         int w = adjacents[v];

    //         if (this->treePath[u][v] == 0)
    //         {
    //             weight = this->distance[u][w] - this->distance[source][w] + this->distance[source][u];
                
    //             graph.setWeightEdgeDirected(u,w,weight);
    //             // cout<<"w'("<<u<<" , "<<w<<" ) = "<<weight<<endl;
    //             weight = this->distance[w][u] - this->distance[source][u] + this->distance[source][w];
                
    //             graph.setWeightEdgeDirected(w,u,weight);
    //             // cout<<"w'("<<v<<" , "<<u<<" ) = "<<weight<<endl;
    //             this->treePath[w][u] = 1;
    //             this->treePath[u][w] = 1;
    //         }
    //     }
    // }
}

/**
 * Todas as arestas da árvore receberão peso 0
 * Os demais nós será aplicado a fórmula proposta:
 *      w'(u,v) = w (w,u) - d(s,v) + d(s,u)
 */
void BalancedSuurballe::changeEdgesWeightsAll(Graph & graph, tree<int> tr, vector<int> nodes)
{
    int source = *tr.begin();

    /**
     * Percore árvore atualizando o peso das ligações u --> v
     * Se o nodo esta no caminho o peso passa a ser 0 (zero)
     * Do contrário aplica a equação proposta por BalancedSuurballe
     * Sendo s o source e u e v nós pertencentes a árvore
     *      w'(u,v) = w (u,v) - d(s,v) + d(s,u)
     */

    int headCount = tr.number_of_siblings(tr.begin());//número de cabeças da árvore

    typename tree<int>::sibling_iterator iRoot = tr.begin();


    unsigned int count = 0;

    vector<int> temp = vector<int> (this->numberOfNodes,-1);

    while( count < nodes.size() )
    {
        temp[ nodes[count] ] = nodes[count];

        count++;
    }

    updateEdgesWeight(tr,iRoot,temp,graph,source);//atualiza peso e remove ligações

    temp.clear();
    // nodes.clear();
    /**
     * Atualiza nós não pertencentes a árvore
     */
    vector<Node> n = graph.getNodes();
    for (unsigned int u = 0; u < this->nodeInTree.size(); u++)
    {
        vector<int> adjacents = n[u].getAdjacentsNodes();

        for (unsigned int v = 0; v < adjacents.size(); v++)
        {
            double weight = 0.0f;
            int w = adjacents[v];

            if (this->treePath[u][v] == 0)
            {
                weight = this->distance[u][w] - this->distance[source][w] + this->distance[source][u];
                
                graph.setWeightEdgeDirected(u,w,weight);
                // cout<<"w'("<<u<<" , "<<w<<" ) = "<<weight<<endl;
                weight = this->distance[w][u] - this->distance[source][u] + this->distance[source][w];
                
                graph.setWeightEdgeDirected(w,u,weight);
                // cout<<"w'("<<v<<" , "<<u<<" ) = "<<weight<<endl;
                this->treePath[w][u] = 1;
                this->treePath[u][w] = 1;
            }
        }
    }
}

void BalancedSuurballe::makePathVector(vector<int> p1,vector<int> &p2, vector<int> &inPath)
{
    for ( unsigned int u = 0; u < p1.size()-1; u++)
    {
        p2.push_back( p1[u] );
        p2.push_back( p1[u+1] );

        inPath[ p1[u] ] = p1[u];
        inPath[ p1[u+1] ] = p1[u+1];
    }
}

vector<int> BalancedSuurballe::disjointPath(int target)
{
    vector<int> path;

    int u = target;

    path.push_back( target );

    while( this->parent[u] != -1 )
    {
        path.push_back( this->parent[u] );
        u = this->parent[u];
    }

    reverse( path.begin(),path.end() );//inverte caminho

    // for (unsigned int i = 0; i < path.size(); i++)
    // {
    // 	cout<<" "<<path[i];
    // }
    // cout<<endl;

    return path;
}

/**
 * armazena nós visitados em visited
 */
bool BalancedSuurballe::findPath(Graph &g, vector<Node> nodes,int source, int target)
{
    if( this->visited[source] == 1 || source == target ) return true;

    this->visited[source] = 1;

    vector<int> adjacents = nodes[source].getAdjacentsNodes();

    for( int u = 0; u < this->numberOfNodes; u++)
    {
        //verifica se existe ligação
        if ( find(adjacents.begin(),adjacents.end(),u) != adjacents.end() )
        {
            // cout<<"u "<<source<<" v "<<u<<endl;

            g.removeNode(source,u);
            this->parent[u] = source;

            if( findPath(g, nodes,u,target) ) return true;
        }
    }

    return false;
}

bool BalancedSuurballe::makeSubgraphDisjointPaths(Graph &g, int source, int target)
{
    vector<Node> nodes = g.getNodes();

    //nó origem com mais de uma aresta de saída
    if ( nodes[source].getAdjacentsNodes().size() >= 3 )
    {
        return false;
    }

    //nó destino possui aresta de saída
    if ( nodes[target].getAdjacentsNodes().size() >= 1 )
    {
        return false;
    }

    int count = 0;

    vector<vector<int>> p;

    while(count < 2)
    {

        this->visited = vector<int> (g.getNumberOfNodes(),0);
        this->parent = vector<int> (g.getNumberOfNodes(),-1);

        findPath(g,nodes,source,target);

        p.push_back( disjointPath(target) );

        nodes = g.getNodes();
        count++;
    }

    nodes = g.getNodes();

    //verifica se os nós não possuem mais nenhuma ligação
    for (int u = 0; u < g.getNumberOfNodes(); u++)
    {
        if (nodes[u].getAdjacentsNodes().size() >= 1 )
        {
            return false;
        }
    }


    return true;
}

int BalancedSuurballe::compareWithOthers(vector<int> p1, vector<int> p2) 
{

    vector<int> temp = vector<int> (this->numberOfNodes,-1);
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

vector< vector<int> > BalancedSuurballe::findPairOfBalancedPaths(Graph g,int source,int target, int p1, int p2)
{
    // cout<<"( "<<source<<" , "<<target<<" ) "<<" p1 "<<p1<<" p2 "<<p2<<endl;
    vector< vector<int> > pairOfPaths;
    // cout<<"AQUI deu crepe crepe"<<endl;
    pairOfPaths = findAllPaths(g,source,target);
    // cout<<"AQUI Não deu crepe crepe findPairOfBalancedPaths"<<endl;
    int sum = p1 + p2;//somatório dos caminhos mínimos encontrados pelo algoritmo
    int diff = abs( p2 - p1 ), diffAnt, a = 0, b = 0, s = sum+1;
    diffAnt = diff;
    // printf("source %d target %d\n",source,target);

    // for (int i = 0; i < pairOfPaths.size(); i++)
    // {
    //     for (int j = 0; j < pairOfPaths[i].size(); j++)
    //     {
    //         cout<<pairOfPaths[i][j]<<" ";
    //     }

    //     cout<<endl;
    // }
    // cout<<endl;
    // cout<<"number of paths "<<pairOfPaths.size()<<endl;
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

            // for (int k = 0; k < pairOfPaths[i].size(); k++)
            // {
            //     cout<<pairOfPaths[i][k]<<" ";
            // }
            // cout<<endl;
            
            // for (int k = 0; k < pairOfPaths[j].size(); k++)
            // {
            //     cout<<pairOfPaths[j][k]<<" ";
            // }
            // cout<<endl;

            // cout<<"( "<<source<<" , "<<target<<" ) "<<" "<<(pairOfPaths[i].size()-1)<<" "<< (pairOfPaths[j].size()-1)<<" s "<<s<<" sum "<<sum<<endl;
            if ( s <= sum )
            {
                int newDiff = compareWithOthers(pairOfPaths[i],pairOfPaths[j]);
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

vector<int> BalancedSuurballe::returnPath(TreeNode *child)
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

void BalancedSuurballe::addChildren(Graph g,TreeNode *root,int source,int target, vector< vector<int> > &paths)
{
    // cout<<"children "<<root->index<<" pai "<<(root->parent ? root->parent->index : -1)<<endl;
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

vector< vector<int> > BalancedSuurballe::findAllPaths(Graph g,int source,int target)
{
    vector< vector<int> > paths;

    TreeNode *root = new TreeNode(source);//a raíz inicia pela origem do caminho e as folhas serão sempre o destino

    addChildren(g,root,source,target,paths);//adiciona filhos na árvore

    return paths;
} 

bool BalancedSuurballe::isNodeInPath(TreeNode *node,int index)
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

void BalancedSuurballe::freeTree(TreeNode *root)
{
    for (int i = 0; i < root->children.size(); i++)
    {
        freeTree(root->children[i]);
    }

    delete root;
}                                          

void BalancedSuurballe::discardCommonEdge(vector<int> &p1, vector<int> &p2, int x, int y)
{
    vector<int> t1, t2;
    unsigned int u = 0, v = 0;
    // cout<<"\n-----------D---------------"<<endl;
    
    // cout<<endl;
    for (u = 0; u < p1.size(); u += 2)
    {
        if( u == x )
        {
            break;
        }

        t1.push_back(p1[u]);
        t1.push_back(p1[u+1]);
    }
    // cout<<"\n"<<endl; v = 0;   
    for (u = 0; u < p2.size(); u += 2)
    {
        if( u == y ) {
            break;
        }
        
        t2.push_back(p2[u]);
        t2.push_back(p2[u+1]);
    }

    v = 0;
    // cout<<"\nsegunda parte "<<u<<" "<<v<<"\n"<<endl;
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

    // for (u = 0; u < t1.size(); u++)
    // {
    //     cout<<" "<<t1[u];
    // }
    // cout<<endl;
    // for (u = 0; u < t2.size(); u++)
    // {
    //     cout<<" "<<t2[u];
    // }
    // cout<<endl;

    // cout<<"\n---------------------------"<<endl;
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

void BalancedSuurballe::printPaths(vector<int> p1,vector<int> p2, Graph &graph)
{
    int u = 0;

    this->datas<<p1.size()/2<<" ";
    this->datas<<p1[0]<<" ";
    for (u = 1; u < p1.size(); u+=2)
    {
        
        this->datas<<p1[u]<<" ";
    }
    
    // cout<<" number of hops "<<p1.size()/2<<endl;
    this->datas<<"\n";
    
    
    // this->datas<<"Backup path ["<<source<<" , "<<target<<" ]"<<" number of hops = "<<p2.size()/2<<endl;
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

bool BalancedSuurballe::makeDisjointPaths(vector<int> path1, vector<int> path2, Graph &graph, bool print)
{

    vector<int> p1,p2;
    int u = 0;

    vector<int> temp = vector<int> (this->numberOfNodes,-1);

    makePathVector(path1,p1,temp);
    makePathVector(path2,p2,temp);

    // cout<<p1.size()/2<<" ";
    cout<<p1[0]<<" ";
    for (u = 1; u < p1.size(); u+=2)
    {
        
        cout<<p1[u]<<" ";
    }
    cout<<endl;
    // // cout<<" number of hops "<<p1.size()/2<<endl;
    // cout<<"\n";
    
    
    // // cout<<"Backup path ["<<source<<" , "<<target<<" ]"<<" number of hops = "<<p2.size()/2<<endl;
    // cout<<p2.size()/2<<" ";
    cout<<p2[0]<<" ";
    for (u = 1; u < p2.size(); u+=2)
    {
      
        cout<<p2[u]<<" ";
    }
    cout<<"\n";
    /**
     * Remover arestas invertidas
     * Dos caminhos mínimos p1 e p2
     */
    for ( unsigned int u = 0; u < p1.size()-1; u+=2)
    {
        for (unsigned int v = 0; v < p2.size()-1; v+=2)
        {
            //exclui arestas em comum mas invertidas
            if (p1[u] == p2[v+1] && p1[u+1] == p2[v])
            {
                discardCommonEdge(p1,p2,u,v);
            }

        }
    }

    

    /**
     * Contruir sub-grafo
     */
    vector< vector<int> > subGraph = vector< vector<int> > ( this->numberOfNodes, vector<int> (this->numberOfNodes,0) );

    Graph g;

    g.setNumberOfNodes(this->numberOfNodes);

    g.memsetGraph();

    /**
     * Insere as arestas e seus respectivos pesos
     */
    g.setMaximumDegree(g.getNumberOfNodes()-1);
    g.setMinimumDegree(1);

    for (u = 0; u < p1.size(); u+=2)
    {
        g.setEdgeDirected(p1[u],p1[u+1]);

    }

    for (u = 0; u < p2.size(); u+=2)
    {
        g.setEdgeDirected(p2[u],p2[u+1]);

    }
    
    // cout<<"tamanho p1 "<<p1.size()/2<<" ";
    // cout<<p1[0]<<" ";
    // for (u = 1; u < p1.size(); u+=2)
    // {
        
    //     cout<<p1[u]<<" ";
    // }
    // cout<<endl;
    // // // cout<<" number of hops "<<p1.size()/2<<endl;
    // // cout<<"\n";
    
    
    // // // cout<<"Backup path ["<<source<<" , "<<target<<" ]"<<" number of hops = "<<p2.size()/2<<endl;
    // cout<<"tamanho p2 "<<p2.size()/2<<" ";
    // cout<<p2[0]<<" ";
    // for (u = 1; u < p2.size(); u+=2)
    // {
      
    //     cout<<p2[u]<<" ";
    // }
    // cout<<"\n";


    int source = path1[0];
    int target = path1[ path1.size()-1 ];
    vector< vector<int> > pairOfPaths;

    // cout<<p1.size()<<" "<<p2.size()<<" "<<path1.size()<<" "<<path2.size()<<endl;
    if( p1.size() != p2.size() && abs(path2.size()-path1.size()) >= 2) 
    {
        pairOfPaths = findPairOfBalancedPaths(g,source,target,p1.size()/2,p2.size()/2);
        
        unsigned sum = (p1.size()/2)+(p2.size()/2), sum2 = (p1.size()/2)+(p2.size()/2)+1;
        
        if( pairOfPaths.size() == 2) 
        {
            sum = (p1.size()/2)+(p2.size()/2);
            sum2 = (pairOfPaths[0].size()-1)+(pairOfPaths[1].size()-1);
        }
        if ( (pairOfPaths.size() == 2) && (sum == sum2 ))
        {
            /**
             * Verifica se existem duas arestas de saída no source
             * E duas arestas de entrada no target, além disso deve
             * haver uma de entrada e uma de saída nos nós restantes
             */
            vector<int> p_1,p_2;

            vector<int> _temp = vector<int> (this->numberOfNodes,-1);

            makePathVector(pairOfPaths[0],p_1,_temp);
            makePathVector(pairOfPaths[1],p_2,_temp);

            if (print == false)
            {
                this->pathOne = p_1;
                this->pathTwo = p_2;
            }
            else
            {
                this->pathOne_ = p_1;
                this->pathTwo_ = p_2;
            }
        }
        else
        {
            /**
             * Verifica se existem duas arestas de saída no source
             * E duas arestas de entrada no target, além disso deve
             * haver uma de entrada e uma de saída nos nós restantes
             */
            if (print == false)
            {
                this->pathOne = p1;
                this->pathTwo = p2;
            }
            else
            {
                this->pathOne_ = p1;
                this->pathTwo_ = p2;
            }
        }

    }
    else
    {
        /**
         * Verifica se existem duas arestas de saída no source
         * E duas arestas de entrada no target, além disso deve
         * haver uma de entrada e uma de saída nos nós restantes
         */
        if (print == false)
        {
            this->pathOne = p1;
            this->pathTwo = p2;
        }
        else
        {
            this->pathOne_ = p1;
            this->pathTwo_ = p2;
        }
    }

   // cout<<" Número de enlaces de "<<source<<" até "<<target<< " = "<<g.getNumberOfEdges()<<endl;
    return makeSubgraphDisjointPaths(g,source,target);
}

void BalancedSuurballe::choiceNewPath(Graph graph,int iterator,int u,int v,bool useEdgeWeight)
{
    Graph auxiliar = graph;
    bool survivor = false;
    double dist = 0.0f;
    Dijkstra dijkstra;

    /**
     * mudança de peso nas arestas
     * Monta árvore a partir do nó u
     */
    
    cout<<"\nU "<<u<<" V "<<v<<endl;
    this->treePath = vector<vector<int>> (this->numberOfNodes,vector<int>(this->numberOfNodes,0)); 
    tree<int> tr = makeTree(auxiliar, this->path[iterator], u);
    
    if (useEdgeWeight)
    {
        changeEdgesWeightsAll(auxiliar, tr, this->path[iterator]);
    }
    else
    {
        changeEdgesWeights(auxiliar, tr, this->path[iterator]);
    }

    // cout<<"Passou changeEdgesWeights"<<endl;
    // cout<<"----------------------------------------------------------"<<endl;
    // for (int ll = 0; ll < this->numberOfNodes; ll++)
    // {
    //     auxiliar.printAdjacents(ll);
    // }
    // cout<<"----------------------------------------------------------"<<endl;
    
    dist =  dijkstra.execute(auxiliar,u,v);

    if ( dist == std::numeric_limits<double>::max() )
    {
        exit(1);
    }

    vector<int> newPath = dijkstra.shortestPath(v);

    //não encontrou caminho
    if (newPath.size() == 1)
    {
       exit(1);
    }

    survivor = makeDisjointPaths(path[iterator],newPath,graph,useEdgeWeight);

    if (survivor == false)
    {
        exit(1);
    }

    tr.clear();

    this->nodeInTree.empty();
}

bool BalancedSuurballe::execute(Graph & graph, string nameFile)
{
    nameFile = "output_balanced_"+nameFile;
  
    this->datas.open(nameFile);

    bool survivor = false;
    if (this->datas.is_open())
    {

        this->numberOfPaths = 0;
        Dijkstra dijkstra;

        this->numberOfNodes = graph.getNumberOfNodes();

        this->distance = vector<vector<int>> (this->numberOfNodes,vector<int>( this->numberOfNodes,0) );

        int n = 0;
       
        
        /**
         * Para cada par de nós (u,v)
         * Obtêm caminho mínimo
         */
        for (unsigned int u = 0; u < this->numberOfNodes-1; u++)
        {
            for(unsigned int v = u+1; v < this->numberOfNodes; v++)
            {
                this->distance[u][v] = this->distance[v][u] = dijkstra.execute(graph,u,v);
                // cout<<"distance entre "<<u<<" e "<<v<<" = "<<this->distance[u][v]<<endl;
                this->path.push_back( dijkstra.shortestPath(v) );

                if ( path[n].size() <= 1)
                {
                    return survivor;
                }
                this->numberOfPaths++;
                n++;
            }
        }

        int iterator = 0;

        for (unsigned int u = 0; u < this->numberOfNodes-1; u++)
        {
            for (unsigned int v = u+1; v < this->numberOfNodes; v++)
            {

                choiceNewPath(graph,iterator,u,v,false);
                choiceNewPath(graph,iterator,u,v,true);

                if ( (this->pathTwo_.size()+this->pathOne_.size()) > (this->pathTwo.size()+this->pathOne.size()))
                {
                    printPaths(this->pathTwo,this->pathOne,graph);
                }
                else if ((this->pathTwo_.size()+this->pathOne_.size()) == (this->pathTwo.size()+this->pathOne.size()))
                {
                    int diff1 = abs(this->pathTwo.size()-this->pathOne.size());
                    int diff2 =  abs(this->pathTwo_.size()-this->pathOne_.size());
                    
                    if (diff1 <= diff2)
                    {
                        printPaths(this->pathTwo,this->pathOne,graph);
                    }
                    else
                    {
                        printPaths(this->pathTwo_,this->pathOne_,graph);
                    }
                }
                else if ( (this->pathTwo_.size()+this->pathOne_.size()) < (this->pathTwo.size()+this->pathOne.size()))
                {
                    printPaths(this->pathTwo_,this->pathOne_,graph);
                }

                this->pathTwo_.clear();
                this->pathOne_.clear();
                this->pathTwo.clear();
                this->pathOne.clear();

                iterator++;
            }
        }

        BalancedSuurballe::averageHops();
    }

    return true;
}

/**
 * Calcula a média dos saltos dos caminhos de trabalho 
 * Calcula a media dos saltos dos caminhos de backup
 * Retorna o valor de cada um
 */
vector<double> BalancedSuurballe::averageHops()
{
    vector<double> avgHops;
    double kp = 0;

    double sumWorkingHops = 0, sumBackupHops = 0;

    for (unsigned int u = 0; u < this->hopBackup.size(); u++)
    {
        // for (int v = u+1; v < this->numberOfNodes; v++)
        // {
            sumWorkingHops = sumWorkingHops + this->hopWorking[u];
            sumBackupHops = sumBackupHops + this->hopBackup[u];
            // this->datas<<this->hopWorking[u]<<" "<<this->hopBackup[u]<<endl<<endl;
        // }
    }

    this->datas<<"sum Working = "<<sumWorkingHops<<endl;
    this->datas<<"sum Backup = "<<sumBackupHops<<endl;
    int N = this->numberOfNodes;

    cout<<"sumBackupHops "<<sumBackupHops<<" sumWorkingHops "<<sumWorkingHops<<" diff = "<<abs(sumWorkingHops-sumBackupHops)<<endl;
    double avgWorkingHops = ( 2* sumWorkingHops )/ ( N*(N-1) );
    double averageBackupHops = (2 *sumBackupHops )/ ( N*(N-1) );

    avgHops.push_back(avgWorkingHops);
    avgHops.push_back(averageBackupHops);

    kp = avgHops[1]/avgHops[0];
    this->datas<<"protection coefficient = "<<kp<<endl;
    cout<<"Kp  = "<<kp<<endl;
    this->datas.close();
    return avgHops;
}