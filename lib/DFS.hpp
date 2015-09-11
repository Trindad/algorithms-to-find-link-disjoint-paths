#include "Graph.hpp"

class DFS
{
public:
  
  DFS(int);
  ~DFS();
  
  void insertPath(vector<vector<int>> &);

  void execute(int, int , int, Graph &, int,vector<vector<int>> &); 

  vector<bool> used;
  vector<int> p;//caminho
};