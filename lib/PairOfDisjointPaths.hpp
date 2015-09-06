#ifndef PAIROFDISJOINTPATHS_H
#define PAIROFDISJOINTPATHS_H
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
#include "TreeNode.hpp"
#include "Dijkstra.hpp"
#include <thread>
#include <mutex>
#include <utility>

class PairOfDisjointPaths 
{
public:

	PairOfDisjointPaths();
	~PairOfDisjointPaths();


	bool execute(Graph &,string);
	void averageHops(Graph);
	void dfs(vector<pair<int,int>> &,Graph &, int, int, vector< vector<int> > &, vector<int> &);
	void discardCommonEdge(vector<int> &, vector<int> &, int, int);	//gera dois novos caminhos excluindo os enlaces em comum
	void writeFile();
	/**
	 * Métodos para encontrar caminhos balanceados
	 */
	bool searchPath(vector< vector<int> >, vector<int> );
	virtual bool findPairOfBalancedPaths(Graph,int,int) = 0;		//grafo construído a partir dos caminhos mínimos encontrados pelo algoritmo de BalancedSuurballe, e a origem e destino
	vector< vector<int> > findAllPaths(vector<pair<int,int>> &,Graph,int,int);					//retorna todos os caminhos encontrados da origem ao destino
	vector<int> returnPath(TreeNode *);									//retorna novo caminho, sobe do filho até o pai construindo o caminho
	bool isNodeInPath(TreeNode *,int);									//verifica se o nó já existe em um caminho
	void addChildren(vector<pair<int,int>> &,Graph,TreeNode *,int,int,vector< vector<int> > &); //Adiciona recursivamente os filhos do nó root passado como parâmetro
	void freeTree(TreeNode *);
	void makePathVector(vector<int>,vector<int> &, vector<int> &);											//desaloca memória da árvore
	vector< vector<int> > compareWithOthers(Graph g,vector<int> &, vector<int> &);					//compara dois caminhos, verificando se existe arestas iguais
	void printPaths(vector<int>, vector<int>,Graph &);
	void sortDatas(vector<pair<int,int>> &distance);
	void removeUnnecessaryPaths(vector<int>,vector<int>, vector<pair<int,int>> &);
	vector<vector<int>> shortestPaths(Graph, vector<pair<int,int>> &, int, int);
	void removePath(Graph &, vector<int> );
	void shortestPathEdge(Graph, Graph &, vector< vector <int> > &,int, int);
	void shortestPathNode(Graph, Graph &,vector< vector <int> > &,int, int);
	// void createDigraph(Graph graph);

	mutex m;
	mutex ap;
	// Digraph digraph;
	vector< vector< vector<int> > > allPaths;
	vector< double> hopWorking;					//número de saltos(i,j) do principal
	vector< double> hopBackup;					//número de saltos(i,j) de backup
	ofstream datas;//arquivo para armazenar caminhos de trabalho e backup	
};
#endif