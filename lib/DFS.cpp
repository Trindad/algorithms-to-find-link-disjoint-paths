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