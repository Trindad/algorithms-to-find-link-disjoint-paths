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
 * Definição de estraura para o algoritmo de Dijkstra
 */

#include "Dijkstra.hpp"

Dijkstra::Dijkstra(){}

Dijkstra::~Dijkstra(){}

/**
 * Insere caminho mínimo ao inverso
 */
vector<int> Dijkstra::shortestPath(int target)
{
    vector<int> path;

    int u = target;

    path.push_back( target );

    while( this->parent[u] != -1 )
    {
        path.push_back( this->parent[u]);
        u = this->parent[u];
    }
    reverse( path.begin(),path.end() );//inverte caminho


    // for (int i = 0; i < path.size() ; i++)
    // {
    //     cout<<" "<<path[i];
    // }
    // cout<<endl;
    return path;
}

/**
 * Implementação baseada no livro The Algorithm Design Manual -- Skiena
 */
int Dijkstra::execute( Graph graph, int source, int target)
{
    Node p; 					//vetor temporário
    vector<bool> inTree;		//O nó já esta na árvore?
    vector<double> distance;	//armazena distância para source
    int v;						//nó atual
    int w;						//candidato a próximo nó
    int n;						//número de nós adjacentes
    double weight;				//peso da aresta
    double dist;				//melhor distância atual para o nó de partida


    inTree = vector<bool> ( graph.getNumberOfNodes(), false);
    distance = vector<double> ( graph.getNumberOfNodes(), std::numeric_limits<double>::max() );
    this->parent = vector<int> ( graph.getNumberOfNodes(), -1);

    v = source;
    distance[v] = 0.0f;

    while( inTree[target] == false && inTree[v] == false)
    {
        inTree[v] = true;

        p = graph.getNodeAtPosition(v);

        n = p.getDegree();

        if (n == 0)
        {
            // cout<<"Topologia com nó "<<v<<" desconexo."<<endl;

            return -std::numeric_limits<double>::max() ;
        }

        int iterator = 0;

        while( iterator < n )
        {
            w = p.getAdjacentNode(iterator);
            weight = p.getWeightEdge(iterator); //obtêm peso da aresta

            /**
             * Verificação de caminho
             */
            if ( distance[w] > ( distance[v] + weight ) )
            {
                distance[w] = distance[v] + weight;
                this->parent[w] = v;
            }

            iterator++;
        }

        v = 0;

        dist = std::numeric_limits<double>::max();

        for (int i = 0; i < graph.getNumberOfNodes(); i++)
        {
            if ( ( inTree[i] == false ) && ( dist > distance[i] ) )
            {
                dist = distance[i];
                v = i;
            }
        }

        if (v == target)
        {
            break;
        }
    }

    return distance[target];//retorna distância
}


/**
 * Implementação baseada no livro The Algorithm Design Manual -- Skiena
 */
int Dijkstra::executeMaximum( Graph graph, int source, int target)
{
    Node p;                     //vetor temporário
    vector<bool> inTree;        //O nó já esta na árvore?
    vector<double> distance;    //armazena distância para source
    int v;                      //nó atual
    int w;                      //candidato a próximo nó
    int n;                      //número de nós adjacentes
    double weight;              //peso da aresta
    double dist;                //melhor distância atual para o nó de partida


    inTree = vector<bool> ( graph.getNumberOfNodes(), false);
    distance = vector<double> ( graph.getNumberOfNodes(), -std::numeric_limits<double>::max());
    this->parent = vector<int> ( graph.getNumberOfNodes(), -1);

    v = source;
    distance[v] = 0.0f;

    while( inTree[target] == false && inTree[v] == false)
    {
        inTree[v] = true;

        p = graph.getNodeAtPosition(v);

        n = p.getDegree();

        if (n == 0)
        {
            return -std::numeric_limits<double>::max() ;
        }

        int iterator = 0;

        while( iterator < n )
        {
            w = p.getAdjacentNode(iterator);
            weight = p.getWeightEdge(iterator); //obtêm peso da aresta

            /**
             * Verificação de caminho
             */
            printf("sum %d\n",distance[v] + weight  );
            if ( distance[w] < ( distance[v] + weight ) )
            {
                distance[w] = distance[v] + weight;
                this->parent[w] = v;
            }

            iterator++;
        }

        v = 0;

        dist = -std::numeric_limits<double>::max();

        for (int i = 0; i < graph.getNumberOfNodes(); i++)
        {
            if ( ( inTree[i] == false ) && ( dist < distance[i] ) )
            {
                dist = distance[i];
                v = i;
            }
        }

        if (v == target)
        {
            break;
        }
    }

    return distance[target];//retorna distância
}

