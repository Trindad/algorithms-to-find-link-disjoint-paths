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

#include "WorstBalancedPathEdge.hpp"


WorstBalancedPathEdge::WorstBalancedPathEdge(){}
WorstBalancedPathEdge::~WorstBalancedPathEdge(){}

vector< vector<int> > WorstBalancedPathEdge::compareWithOthers(Graph g,vector<int> &p1, vector<int> &p2) 
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
    for (int u = 0; u < (int)path1.size()-1; u+=2)
    {
        for (int v = 0; v < (int)path2.size()-1; v+=2)
        {
            //exclui arestas em comum mas invertidas
            if (path1[u] == path2[v+1] && path1[u+1] == path2[v])
            {
               return paths;
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

bool WorstBalancedPathEdge::findPairOfBalancedPaths(Graph g,int source,int target)
{
    // cout<<" source "<<source<<" target "<<target<<endl;
    vector< vector<int> > pairOfPaths;
    vector<pair<int,int>> distance;
    vector<int> path;

    int sum = g.getNumberOfNodes()+1;//somatório dos caminhos mínimos encontrados pelo algoritmo
    int diff = sum+1; //iniciando com número infinito
    int a = 0, b = 0, m = 0, p = 0, w = 0, z = 0, c = 0;
    vector<vector<int>> pairTemp;

   int limit = 2; //inicia limit com dois nós para tamanho máximo do caminho, ou seja, com 1 hop
   int diameterGraph = g.getNumberOfNodes();
   
   while(limit <= diameterGraph && c <= 1)
   {
        pairOfPaths = findAllPaths(distance,g,source,target,limit);

        if ( (int) pairOfPaths.size() <= 1  )
        {
            limit++;

            distance.clear();
            pairOfPaths.clear();

            continue;
        }
      
        sortDatas(distance);//ordena vetor de pares

        sum = g.getNumberOfNodes()+1;//somatório dos caminhos mínimos encontrados pelo algoritmo
        diff = sum+1; //iniciando com número infinito
        a = 0, b = 0, m = 0, p = 0;

        unsigned int n = distance.size();

        for (unsigned int i = 0; i < n-1; i++)
        {
            if ( ( a != b) && (m != p) )
            {
                if ((int)pairOfPaths[a].size() > (int)pairOfPaths[b].size())
                {
                    n = m;
                }
                else
                {
                    n = p;
                }
            }
            /**
             * Encontrou um caminho de par disjuntos
             * E pelo menos um dos caminhos é menor que o novo
             */
            for (unsigned int j = i+1; j < n; j++)
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
                int s = (int)( (double)paths[0].size() + (double)paths[1].size() )/2;
                
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

                    m = i; p = j;
                }
                else if (s == sum)
                {
                    if (newDiff > diff)
                    {

                        a = u;
                        b = v;

                        diff = newDiff;
                        sum = s;
                        m = i; p = j;
                    }
                }
            }
        }

        //encontrou dois nós
        if (a != b )
        {
            w = a, z = b;
                
            if ((int)pairTemp.size() >= 2)
            {
                pairTemp.clear();
            }

            pairTemp.push_back(pairOfPaths[a]);
            pairTemp.push_back(pairOfPaths[b]);

            diameterGraph = (int)pairTemp[0].size() + (int)pairTemp[1].size();

            c++;
        }

        distance.clear();
        pairOfPaths.clear();
        
        limit++;
    }

    if (w == z)
    {
        return false;
    }
 
    vector< vector<int> > paths = compareWithOthers(g,pairTemp[0],pairTemp[1]);

    printPaths(paths[0],paths[1], g);

    paths[0].clear();paths[1].clear();paths.clear();

    distance.clear();
    pairOfPaths.clear();


    return true;
}
                                     

void WorstBalancedPathEdge::discardCommonEdge(vector<int> &p1, vector<int> &p2, int x, int y)
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

