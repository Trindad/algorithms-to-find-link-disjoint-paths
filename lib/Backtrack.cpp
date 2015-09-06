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
}

Backtrack::~Backtrack() {
}

void Backtrack::execute(int a[], int k, int input)
{
    // cout<<"source "<<k<<" target "<<input<<endl;
    int c[MAXCANDIDATES];           /* candidates for next position */
    int ncandidates;                /* next position candidate count */
    int i;                          /* counter */

    if (is_a_solution(a,k,input))
    {
        // cout << "WTF" << endl;
        process_solution(a,k,input);
    }
    else 
    {
        k = k+1;
        // cout<<"-----------------------"<<endl;
        construct_candidates(a,k,input,c,&ncandidates);
        // cout<<"-----------------------"<<endl;
        for (i = 0; i < ncandidates; i++) {
            a[k] = c[i];
            execute(a,k,input);
            if (finished) return;   /* terminate early */
        }
    }
}

void Backtrack::process_solution(int a[], int k, int t)
{
    int i;              /* counter */

    solution_count++;
    vector<int> path;
    
    // cout<<"source "<<this->source<<" target "<<t<<endl;
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
    return (a[k] == t);
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
        // cout<<" "<<a[i];
    }
    // cout<<endl;
    if ( k == this->source) {         /* always start from vertex 1 */
        // cout<<"ENTROU "<<endl;
        c[0] = this->source;
        *ncandidates = 1;
    }
    else 
    {
        *ncandidates = 0;
        last = a[k-1];
        // cout<<"LAST "<<last<<" "<<k<<endl;

        vector<int> adjacents;
        Node node = this->g.getNodeAtPosition(last);

        adjacents = node.getAdjacentsNodes();

        for (int w = 0; w < (int)adjacents.size(); w++)
        {
            
            if (in_sol[ adjacents[w] ] == false) 
            {
                // cout<<" "<<last<<" "<<adjacents[w]<<endl;

                c[*ncandidates] = adjacents[w];
                *ncandidates = *ncandidates + 1;
            }
        }
        // cout<<endl;
    }
}