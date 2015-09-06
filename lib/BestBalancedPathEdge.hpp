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

class BestBalancedPathEdge : public PairOfDisjointPaths
{
public:

	BestBalancedPathEdge();
	~BestBalancedPathEdge();

	void discardCommonEdge(vector<int> &, vector<int> &, int, int);	//gera dois novos caminhos excluindo os enlaces em comum
	
	/**
	 * Métodos para encontrar caminhos balanceados
	 */
	bool findPairOfBalancedPaths(Graph,int,int);									//grafo construído a partir dos caminhos mínimos encontrados pelo algoritmo de BalancedSuurballe, e a origem e destino
	vector< vector<int> > compareWithOthers(Graph g,vector<int> &, vector<int> &);	//compara dois caminhos, verificando se existe arestas iguais
};