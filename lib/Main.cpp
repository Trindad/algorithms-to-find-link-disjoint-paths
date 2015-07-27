#include "Graph.hpp"
#include "Suurballe.hpp"
// #include "TreeNode.hpp"
#include <sstream>

vector<string> &split(const string &s, char delim, vector<string> &elems);

vector<string> split(const string &s, char delim);

int main(int argc, char const *argv[])
{

	if (argc <= 1)
	{
		return 0;
	}

	ifstream file;

	file.open(argv[1]);

	vector<string> pathFile = split(argv[1],'/');

	
	if (file.is_open())
	{
		Graph g;

		string line = " ";

		getline (file,line);

		int n = stoi(line);//obtêm o número de nós

		g.setNumberOfNodes(n);
		g.setMinimumDegree(2);
		g.setMaximumDegree(n-1);

		// cout<<"Número de nós: "<<n<<endl;
		vector<string> nodes;

		while( getline (file,line) )
		{
			nodes = split(line.c_str(),' ');
			// cout<<" "<<nodes[0]<<" "<<nodes[1]<<endl;

			int u = stoi(nodes[0])-1;
			int v = stoi(nodes[1])-1;
			// g.setEdgeDirected(u,v);
			g.setEdge(u,v);
			g.setWeight(u,v,1.0);
			g.setWeight(v,u,1.0);
		}
		
		// g.setWeightEdgeDirected(1-1,8-1,2.0f);
		// g.setWeightEdgeDirected(1-1,7-1,8.0f);
		// g.setWeightEdgeDirected(1-1,2-1,3.0f);
		// g.setWeightEdgeDirected(2-1,3-1,1.0f);
		// g.setWeightEdgeDirected(2-1,6-1,6.0f);
		// g.setWeightEdgeDirected(2-1,7-1,4.0f);
		// g.setWeightEdgeDirected(3-1,4-1,5.0f);
		// g.setWeightEdgeDirected(5-1,4-1,7.0f);
		// g.setWeightEdgeDirected(6-1,5-1,2.0f);
		// g.setWeightEdgeDirected(5-1,8-1,3.0f);
		// g.setWeightEdgeDirected(8-1,6-1,5.0f);
		// g.setWeightEdgeDirected(7-1,4-1,1.0f);

		Suurballe s;

		bool sobrevivente = s.execute(g,pathFile[pathFile.size()-1]);

		cout<<"sobrevivente "<<sobrevivente<<endl;

		file.close();

	}
	else
	{
		cout<<"ERROR"<<endl;
	}
	
	// TreeNode *t = new TreeNode(5);
	// TreeNode *n3 = new TreeNode(3);
	// TreeNode *n6_1 = new TreeNode(6);
	// TreeNode *n6_2 = new TreeNode(6);
	// TreeNode *n7_1 = new TreeNode(7);
	// TreeNode *n7_2 = new TreeNode(7);
	// TreeNode *n8_1 = new TreeNode(8);
	// TreeNode *n8_2 = new TreeNode(8);
	// TreeNode *n8_3 = new TreeNode(8);
	// TreeNode *n8_4 = new TreeNode(8);
	// n3->parent = t;
	// n6_1->parent = t;
	// n7_1->parent = n6_1;
	// n8_1->parent = n6_1;
	// n8_2->parent = n7_1;
	// n6_2->parent = n3;
	// n7_2->parent = n6_2;
	// n8_3->parent = n6_2;
	// n8_4->parent = n7_2;

	// t->addChild(n6_1);
	// t->addChild(n3);
	// n3->addChild(n6_2);
	// n6_2->addChild(n7_2);
	// n6_2->addChild(n8_3);
	// n7_2->addChild(n8_4);
	// n6_1->addChild(n7_1);
	// n6_1->addChild(n8_1);
	// n7_1->addChild(n8_2);

	// TreeNode *temp = n8_4;
	// while (temp) {
	// 	printf("%d\n", temp->index);
	// 	temp = temp->parent;
	// }
	
	return 0;
}


vector<string> &split(const string &s, char delim, vector<string> &elems) {
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}


vector<string> split(const string &s, char delim) {
   vector<string> elems;
    split(s, delim, elems);
    return elems;
}