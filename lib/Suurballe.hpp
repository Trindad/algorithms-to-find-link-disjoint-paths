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

#include "Dijkstra.hpp"
#include "tree.hh"
#include "tree_util.hh"

class Suurballe
{
public:

	Suurballe();
	~Suurballe();

	bool execute(Graph &, string);								//encontra caminhos disjuntos 
	bool findPath(Graph &, vector<Node>, int, int);
	tree<int> makeTree(Graph, vector<int>, int);						//insere nós em uma árvore binária
	bool makeDisjointPaths(vector<int>, vector<int>, Graph &);
	void changeEdgesWeights(Graph &, tree<int>, vector<int>);			//atualiza peso dos nós da árvore
	void makePathVector(vector<int>,vector<int> &,vector<int> &inPath);
	void insertSubtree(Graph, tree<int> &, typename tree<int>::iterator, vector<int>, vector<int> &, int);
	void updateEdgesWeight(const tree<int>& , typename tree<int>::iterator,vector<int>, Graph &, int); 
	vector<int> disjointPath(int);
	bool makeSubgraphDisjointPaths(Graph &, int, int);
	vector<double> averageHops();										//calculo o número médio de saltos para o caminho principal e o de backup
	void discardCommonEdge(vector<int> &, vector<int> &, int, int);

private:

	int numberOfNodes;
	vector<int> visited;
	vector<int> parent;
	int numberOfPaths;
	vector<int> nodeInTree;
	vector< vector<int> > path; 				//armazena caminho mínimo 
	vector< double> hopWorking;		//número de saltos(i,j) do principal
	vector< double> hopBackup;			//número de saltos(i,j) de backup
	vector< vector<int> > distance;				//armazena distância entre um par de nós d(u,v)
	vector< vector<int> > treePath;				//matriz que armazena enlaces da árvore de u a v
	ofstream datas;								//dados obtidos com o algoritmo de Suurballe
};