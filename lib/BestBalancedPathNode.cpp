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

#include "BestBalancedPathNode.hpp"


BestBalancedPathNode::BestBalancedPathNode(){}
BestBalancedPathNode::~BestBalancedPathNode(){}

 vector< vector<int> > BestBalancedPathNode::compareWithOthers(Graph g,vector<int> &p1, vector<int> &p2) 
{
    vector<vector<int>> paths;

    /**
     * Remover arestas invertidas
     * Dos caminhos mínimos p1 e p2
     */
    for ( unsigned int u = 1; u < p1.size()-1; u++)
    {
        for (unsigned int v = 1; v < p2.size()-1; v++)
        {
            if (find(p2.begin(),p2.end(),p1[u]) != p2.end())
            {
                return paths;
            }

        }
    }

    vector<int> temp = vector<int> (g.getNumberOfNodes(),-1);
    vector<int> path1;
    vector<int> path2;

    makePathVector(p1,path1,temp);
    makePathVector(p2,path2,temp);

    paths.push_back(path1);
    paths.push_back(path2);

    return paths;
}

void BestBalancedPathNode::findPairOfBalancedPaths(Graph g,int source,int target)
{
    vector< vector<int> > pairOfPaths;
    vector<pair<int,int>> distance;
    

    pairOfPaths = findAllPaths(distance,g,source,target);

    // cout<<"------------------------------------"<<endl;
    // for (unsigned int i = 0; i < pairOfPaths.size(); i++)
    // {
    //     for (unsigned int j = 0; j < pairOfPaths[i].size(); j++)
    //     {
    //         cout<<" "<<pairOfPaths[i][j];
    //     }
    //     cout<<endl;
    // }
    // cout<<"\n------------------------------------"<<endl;
    
    sortDatas(distance);//ordena vetor de pares
    
    int sum = g.getNumberOfNodes()*2;//somatório dos caminhos mínimos encontrados pelo algoritmo
    int diff = sum+1; //iniciando com número infinito
    int a = 0, b = 0, m = 0, p = 0;

    for (unsigned int i = 0; i < distance.size()-1; i++)
    {
         /**
         * Encontrou um caminho de par disjuntos
         * E pelo menos um dos caminhos é menor que o novo
         */
        if (a != b && (a != m || b != p) )
        {
            removeUnnecessaryPaths(pairOfPaths[a],pairOfPaths[b],distance);
            m = a;
            p = b;
        }

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
        vector<int> temp = vector<int> (g.getNumberOfNodes(),-1);
        vector<int> path1;
        vector<int> path2;
        vector<vector<int>> paths;

        makePathVector(pairOfPaths[a],path1,temp);
        makePathVector(pairOfPaths[b],path2,temp);

        paths.push_back(path1);
        paths.push_back(path2);

        printPaths(paths[0],paths[1], g);
    }
    else
    {
        cout<<"Topologia não sobrevivente."<<" "<<source<<" "<<target<<endl;
        exit(1);
    }

    distance.clear();
    pairOfPaths.clear();
}
                                     

void BestBalancedPathNode::discardCommonEdge(vector<int> &p1, vector<int> &p2, int x, int y)
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

