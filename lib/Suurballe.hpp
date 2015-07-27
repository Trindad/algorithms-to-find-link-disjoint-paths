/**
 * Definições de função-membro que aparecem em Graph.cpp
 */

#include "Dijkstra.hpp"
#include "tree.hh"
#include "tree_util.hh"
#include "TreeNode.hpp"
 
class Suurballe
{
public:

	Suurballe();
	~Suurballe();

	bool execute(Graph &, string);										//encontra caminhos disjuntos 
	bool findPath(Graph &, vector<Node>, int, int);
	tree<int> makeTree(Graph, vector<int>, int);						//insere nós em uma árvore binária
	bool makeDisjointPaths(vector<int>, vector<int>);
	void changeEdgesWeights(Graph &, tree<int>, vector<int>);			//atualiza peso dos nós da árvore
	void makePathVector(vector<int>,vector<int> &,vector<int> &inPath);
	void insertSubtree(Graph, tree<int> &, typename tree<int>::iterator, vector<int>, vector<int> &, int);
	void updateEdgesWeight(const tree<int>& , typename tree<int>::iterator,vector<int>, Graph &, int); 
	vector<int> disjointPath(int);
	bool makeSubgraphDisjointPaths(Graph &, int, int);
	vector<double> averageHops();										//calculo o número médio de saltos para o caminho principal e o de backup
	
	/**
	 * Métodos para encontrar caminhos balanceados
	 */
	vector< vector<int> > findPairOfBalancedPaths(Graph,int,int, int, int);		//grafo construído a partir dos caminhos mínimos encontrados pelo algoritmo de Suurballe, e a origem e destino
	vector< vector<int> > findAllPaths(Graph,int,int);					//retorna todos os caminhos encontrados da origem ao destino
	vector<int> returnPath(TreeNode *);									//retorna novo caminho, sobe do filho até o pai construindo o caminho
	bool isNodeInPath(TreeNode *,int);									//verifica se o nó já existe em um caminho
	void addChildren(Graph,TreeNode *,int,int,vector< vector<int> > &); //Adiciona recursivamente os filhos do nó root passado como parâmetro
	void freeTree(TreeNode *);											//desaloca memória da árvore
	int compareWithOthers(vector<int>, vector<int>);					//compara dois caminhos, verificando se existe arestas iguais

private:

	int numberOfNodes;
	vector<int> visited;
	vector<int> parent;
	int numberOfPaths;
	vector<int> nodeInTree;
	vector< vector<int> > path; 				//armazena caminho mínimo 
	vector< double> hopWorking;					//número de saltos(i,j) do principal
	vector< double> hopBackup;					//número de saltos(i,j) de backup
	vector< vector<int> > distance;				//armazena distância entre um par de nós d(u,v)
	vector< vector<int> > treePath;				//matriz que armazena enlaces da árvore de u a v
	ofstream datas;								//dados obtidos com o algoritmo de Suurballe
};