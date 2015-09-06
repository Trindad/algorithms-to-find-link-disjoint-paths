#include "Graph.hpp"

#define MAXCANDIDATES   10000		/* max possible next extensions */
#define NMAX            1000		/* maximum solution size */

class Backtrack
{
public:
	Backtrack(Graph &g, int);
	~Backtrack();
	void execute(int a[], int k, int input);
	void process_solution(int a[], int k, int t);
	bool is_a_solution(int a[], int k, int t);
	void construct_candidates(int a[], int k, int n, int c[], int *ncandidates);
	
	int source;
	int target;
	bool finished;
	vector<vector<int>> all_paths;
	int solution_count;
	Graph g;
};