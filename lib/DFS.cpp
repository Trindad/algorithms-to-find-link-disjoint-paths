#include "DFS.hpp"

DFS::DFS(int n)
{
  this->used = vector<bool> (n,false);
}

DFS::~DFS(){}

void DFS::insertPath(vector<vector<int>> &paths)
{
  // for (int i = 0; i < (int)p.size(); i++)
  // {
  //   cout<<" "<<this->p[i];
  // }
  // cout<<endl;
  
  paths.push_back(this->p);
}

void DFS::execute(int v, int target, int depth, Graph &g, int limit,vector<vector<int>> &paths) 
{
    this->p.push_back(v);

    if (v == target) 
    {
       insertPath(paths);
    } 
    else
    {
      if (depth > limit) 
      {
        return;
      }
      Node node = g.getNodeAtPosition(v);
      vector<int > adjacents = node.getAdjacentsNodes();

      for(int u = 0; u < (int)adjacents.size();u++) 
      {
        if (this->used[adjacents[u]]) 
        {
          continue;
        }

        this->used[adjacents[u]] = true;

        execute(adjacents[u], target,depth + 1,g,limit,paths );
 
        this->p.pop_back();

        this->used[adjacents[u]] = false;
      }
    }
}