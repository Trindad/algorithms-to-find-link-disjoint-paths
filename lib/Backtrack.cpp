/*	
	backtrack.c
    	A generic backtracking implementation

	begun: March 27, 2002
        by: Steven Skiena
*/

/*
Copyright 2003 by Steven S. Skiena; all rights reserved. 

Permission is granted for use in non-commerical applications
provided this copyright notice remains intact and unchanged.

This program appears in my book:

"Programming Challenges: The Programming Contest Training Manual"
by Steven Skiena and Miguel Revilla, Springer-Verlag, New York 2003.

See our website www.programming-challenges.com for additional information.

This book can be ordered from Amazon.com at

http://www.amazon.com/exec/obidos/ASIN/0387001638/thealgorithmrepo/

*/

#include "Backtrack.hpp"

Backtrack::Backtrack(Graph &graph, int s) {
    
    this->source = s;
    finished = false;
    g = graph;

    this->limitNodesInPath = 50;
    this->nPaths = 0;

    this->solution_count = 0;
}

Backtrack::~Backtrack() {
}

void Backtrack::remove(int a[], int c[])
{
    for (int i = 0; i < NMAX ; i++) a[i] = -1;

    for (int i = 0; i < MAXCANDIDATES ; i++) c[i] = -1;


    int n = (int)this->adjacentsSource.size();

    for (int i = 0; i < n; i++)
    {
        if (this->adjacentsSource[i] >= 0)
        {
            
            c[0] = this->source;
            c[1] = this->adjacentsSource[i];

            a[0] = this->source;
            a[1] = c[1];

            return;
        }
    }

    this->finished = true;
}

bool Backtrack::adjacentsInPaths()
{
    int n = (int)this->adjacentsSource.size();

    for (int i = 0; i < n; i++)
    {
        if (this->adjacentsSource[i] >= 0)
        {
            return false;
        }
    }

    return true;
}

void Backtrack::execute(int a[], int k, int input, int limit)
{
    int c[MAXCANDIDATES];           /* candidates for next position */
    int ncandidates = 0;                /* next position candidate count */
    int i = 0;                          /* counter */

    bool found = is_a_solution(a,k,input);
    
    if ( found )
    {
        process_solution(a,k,input);

    }
    else 
    {

        if (k > limit)
        {
            return;
        }
        k = k+1;

        construct_candidates(a,k,input,c,&ncandidates);

        for (i = 0; i < ncandidates; i++) {
            a[k] = c[i];
            
            execute(a,k,input,limit);

            if (this->finished) return;   /* terminate early */

        }
    }
}

void Backtrack::process_solution(int a[], int k, int t)
{
    int i = 0;              /* counter */
    vector<int> path;

   // printf("{");
    for (i = this->source; i <= k; i++) {
       // printf(" %d",a[i]);
        path.push_back(a[i]);
    }

    // printf(" }\n");
    all_paths.push_back(path);
}

bool Backtrack::is_a_solution(int a[], int k, int t)
{

    return ( ( k >= 0 ) && (a[k] == t) );
}


/*  What are possible elements of the next slot in the path
*/
void Backtrack::construct_candidates(int a[], int k, int n, int c[], int *ncandidates)
{

    int i;              /* counters */
    bool in_sol[NMAX];  /* what's already in the solution? */
    int last;           /* last vertex on current path */

    for (i = 0; i < NMAX; i++) 
    {
        in_sol[i] = false;
    }

    for (i = 0; i < k; i++) 
    {
        in_sol[ a[i] ] = true;
    }

    /* always start from vertex source */
    if ( k == this->source) 
    {   
        c[0] = this->source;
        *ncandidates = 1;
    }
    else 
    {
        *ncandidates = 0;
        last = a[k-1];

        vector<int> adjacents;
        Node node = this->g.getNodeAtPosition(last);

        adjacents = node.getAdjacentsNodes();

        int n = (int)adjacents.size();
        sort(adjacents.begin(),adjacents.end());

        for (int w = 0; w < n; w++)
        {
            if (in_sol[ adjacents[w] ] == false) 
            {
                c[*ncandidates] = adjacents[w];
                *ncandidates = *ncandidates + 1;
            }
        }
    }
}