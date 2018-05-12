// Graph.cpp: îïğåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïğèëîæåíèÿ.
//
// AAAA
#include "stdafx.h"
#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "conio.h"
#include <io.h>
#include <utility>
#include <algorithm>
#include <map>

using namespace std;
struct V
{
	int id;
	int weight;
	
	V()
	{
		id = -1;
		weight = -1;
	}

	V(int a, int w = 0)
	{
		id = a;
		weight = w;
	}

	string toString()
	{
		string s = to_string(id);
		if (weight != 0)
			s += " " + to_string(weight);
		return s;
	}
};
struct Edge
{
public:
	int first;
	int second;
	int weight;
	Edge()
	{
		first = -1;
		second = -1;
		weight = 0;
	}
	Edge(int a, int b, int w = 0)
	{
		first = a;
		second = b;
		weight = w;
	}
	string toString()
	{
		string s = to_string(first) + " " + to_string(second) + " ";
		if (weight != 0)
			s += to_string(weight);
		return s;
	}
	int maxId()
	{
		return first > second ? first : second;
	}
	const bool operator > (Edge a)
	{
		return weight > a.weight;
	}

	const bool operator < (Edge a)
	{
		return weight < a.weight;
	}
	const bool operator == (Edge a)
	{
		return weight == a.weight;
	}
	const bool operator != (Edge a)
	{
		return weight != a.weight;
	}
};

class DSU
{
public:
	vector<int> p, rank;
	DSU(int n)
	{
		p = vector<int>(n);
		rank = vector<int>(n);

	}

	

	void makeSet(int x)
	{
		p[x] = x;
		rank[x] = 0;
	}

	int find(int x)
	{
		return (x == p[x] ? x : p[x] = find(p[x]));
	}

	void unite(int x, int y)
	{
		if ((x = find(x)) == (y = find(y)))
			return;

		if (rank[x] <  rank[y])
			p[x] = y;
		else {
			p[y] = x;
			if (rank[x] == rank[y])
				++rank[x];
		}
	}
};

class Graph
{
public:

	bool isOriented = false;
	bool isWeighted = false;
	char graphType;
	// C ÌÀÒĞÈÖÀ ÑÌÅÆÍÎÑÒÈ
	// L ÑÏÈÑÎÊ ÑÌÅÆÍÛÕ ÂÅĞØÈÍ
	// E ÑÏÈÑÎÊ ĞÅÁÅĞ

	int N; // ÊÎËÈ×ÅÑÒÂÎ ÂÅĞØÈÍ

	int M; // ÊÎËÈ×ÅÑÂÎ ĞÅÁÅĞ
	
	vector<vector<int> > adjMatrix;

	vector<vector<V> > adjVert;

	vector<Edge> listOfEdge;	
	
	void readGraph(string fileName)
	{
		ifstream fin(fileName);
		char tempChar;
		tempChar = fin.get();
		graphType = tempChar;
		if (tempChar == 'C')
			readAdjMatrix(fileName);
		if (tempChar == 'L')
			readListAdjVertices(fileName);
		if (tempChar == 'E')
			readEdgeList(fileName);
	}


	void transformToAdjMatrix()
	{
		char oldGraphType = graphType;
		bool isPushed = false;

		if (oldGraphType == 'L')
		{
			for (int i = 0; i < N; i++)
			{
				vector<int> tempRow;
				for (int j = 0; j < N; j++)
				{
					for (int k = 0; k < adjVert[i].size(); k++)
						if (adjVert[i][k].id == j + 1)
						{
							isPushed = true;
							if (isWeighted)
								tempRow.push_back(adjVert[i][k].weight);
							else
								tempRow.push_back(1);
						}
					if (!isPushed)
						tempRow.push_back(0);
					isPushed = false;
				}
				adjMatrix.push_back(tempRow);
			}
			adjVert.clear();
		}
		if (oldGraphType == 'E')
		{
			for (int i = 0; i < N; i++)
			{
				vector<int> tempRow;
				for (int j = 0; j < N; j++)
				{
					tempRow.push_back(0);
				}
				adjMatrix.push_back(tempRow);
			}
			for (int i = 0; i < listOfEdge.size(); i++)
			{
				if (isWeighted)
					adjMatrix[listOfEdge[i].first - 1][listOfEdge[i].second - 1] = listOfEdge[i].weight;
				else
					adjMatrix[listOfEdge[i].first - 1][listOfEdge[i].second - 1] = 1;
			}
			listOfEdge.clear();
		}
		graphType = 'C';
	}
	void transformToAdjList()
	{
		char oldGraphType = graphType;
		if (oldGraphType == 'C')
		{
			for (int i = 0; i < N; i++)
			{
				vector<V> tempListOfV;
				for (int j = 0; j < N; j++)
				{
					if (isWeighted)
					{
						if (adjMatrix[i][j] != 0)
							tempListOfV.push_back(V(j + 1, adjMatrix[i][j]));
					}
					else
					{
						if (adjMatrix[i][j] != 0)
						{
							cout << adjMatrix[i][j] << " ";
							tempListOfV.push_back(V(j + 1));
						}
					}
				}
				adjVert.push_back(tempListOfV);
			}
			adjMatrix.clear();
		}
		else if (oldGraphType == 'E')
		{
			transformToAdjMatrix();
			transformToAdjList();
		}
		graphType = 'L';


	}
	void transformToListOfEdges()
	{
		char oldGraphType = graphType;
		if (oldGraphType == 'C')
		{
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (adjMatrix[i][j] != 0)
					{
						if (isWeighted)
							listOfEdge.push_back(Edge(i + 1, j + 1, adjMatrix[i][j]));
						else
							listOfEdge.push_back(Edge(i + 1, j + 1));
						M++;
					}
				}
			}
			adjMatrix.clear();
		}
		else if (oldGraphType == 'L')
		{
			transformToAdjMatrix();
			transformToListOfEdges();
		}
		graphType = 'E';
	}
	void writeGraph(string fileName)
	{
		string fileString(1, graphType);
		fileString += " " + to_string(N);
		if (graphType == 'E')
			fileString += " " + to_string(M);
		fileString += "\n";
		fileString += to_string(isOriented) + " " + to_string(isWeighted) + "\n";
	

		if (graphType == 'C')
			writeAdjMatrix(fileName, fileString);

		else if (graphType == 'L')
			writeAdjVert(fileName, fileString);

		else if (graphType == 'E')
			writeListOfEdges(fileName, fileString);


	}

	void writeAdjMatrix(string fileName, string fileString)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
				fileString += to_string(adjMatrix[i][j]) + " ";
			fileString += '\n';
		}
		ofstream fout(fileName);
		fout << fileString;
		
	}
	
	void writeAdjVert(string fileName, string fileString)
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < adjVert[i].size(); j++)
			{
				fileString += adjVert[i][j].toString();
				if (j != adjVert[i].size() - 1)
					fileString += " ";
			}
			fileString += "\n";
		}
		ofstream fout(fileName);
		fout << fileString;
	}

	void writeListOfEdges(string fileName, string fileString)
	{
		for (int i = 0; i < M; i++)
		{
			fileString += listOfEdge[i].toString();
			fileString += "\n";
		}
		ofstream fout(fileName);
		fout << fileString;
	}

	vector<int> readInts(string str)
	{
		vector<int> tempInts;
		string tempString;
		for (int i = 0; i < str.length(); i++)
		{
			if (str[i] == '\n')
				break;
			else if (str[i] == ' ')
			{
				tempInts.push_back(stoi(tempString));
				tempString = "";
			}
			else
			{
				tempString += str[i];
			}
		}
		tempInts.push_back(stoi(tempString));
		return tempInts;
	}
	void readAdjMatrix(string s) // ÌÀÒĞÈÖÀ ÑÌÅÆÍÎÑÒÈ
	{
		ifstream fin(s);
		string tempString = "";
		string tempStringInt = "";
		getline(fin, tempString);
		for (int i = 2; i < tempString.length(); i++) // N
			tempStringInt += tempString[i];

		N = stoi(tempStringInt);
		
		tempString = "";
		tempStringInt = "";
		getline(fin, tempString);
		isOriented = tempString[0] - '0';
		isWeighted = tempString[2] - '0';

		for (int i = 0; i < N; i++)
		{
			vector<int> tempInt;
			tempString = "";
			getline(fin, tempString);
			tempInt = readInts(tempString);
			adjMatrix.push_back(tempInt);
		}

	}

	void readListAdjVertices(string s) // ÑÏÈÑÎÊ ÑÌÅÆÍÛÕ ÂÅĞØÈÍ
	{
		ifstream fin(s);
		string tempString = "";
		string tempStringInt = "";
		getline(fin, tempString);
		for (int i = 2; i < tempString.length(); i++) // N
			tempStringInt += tempString[i];

		N = stoi(tempStringInt);

		tempString = "";
		tempStringInt = "";

		getline(fin, tempString);
		isOriented = tempString[0];
		isWeighted = tempString[2];
		
		for (int i = 0; i < N; i++)
		{
			vector<V> tempVert;
			vector<int> tempInt;
			getline(fin, tempString);
			tempInt = readInts(tempString);
			if (isWeighted)
				for (int j = 0; j < tempInt.size(); j+=2)
					tempVert.push_back(V(tempInt[j], tempInt[j+1]));
			else
				for (int j = 0; j < tempInt.size(); j++)
					tempVert.push_back(V(tempInt[j]));
			adjVert.push_back(tempVert);
		}

	}
	void readEdgeList(string s) // ÑÏÈÑÎÊ ĞÅÁÅĞ
	{
		ifstream fin(s);
		string tempString = "";
		string tempStringInt = "";
		getline(fin, tempString);
		for (int i = 2; i < tempString.length(); i++) // N
			tempStringInt += tempString[i];
		N = readInts(tempStringInt)[0];
		M = readInts(tempStringInt)[1];

		tempString = "";
		tempStringInt = "";

		getline(fin, tempString);
		isOriented = tempString[0];
		isWeighted = tempString[2];

		for (int i = 0; i < M; i++)
		{
			vector<int> tempInt;
			getline(fin, tempString);

			tempInt = readInts(tempString);
			cout << "tempInt[0] = " << tempInt[0];
			cout << "\n";
			cout << "tempInt[1] = " << tempInt[1];
			cout << "\n\n";

			if (isWeighted)
				listOfEdge.push_back(Edge(tempInt[0], tempInt[1], tempInt[2]));
			else
				listOfEdge.push_back(Edge(tempInt[0], tempInt[1]));
		}
	}
	Graph getSpaingTreePrima()
	{
		transformToAdjMatrix();
		int n;
		vector < vector<int> > g;
		const int INF = 1000000000;
		vector<bool> used(n);
		vector<int> min_e(n, INF), sel_e(n, -1);
		min_e[0] = 0;
		for (int i = 0; i < n; ++i) 
		{
			int v = -1;
			for (int j = 0; j < n; ++j)
				if (!used[j] && (v == -1 || min_e[j] < min_e[v]))
					v = j;
			if (min_e[v] == INF) 
			{
				cout << "No MST!";
				exit(0);
			}

			used[v] = true;
			if (sel_e[v] != -1)
				cout << v << " " << sel_e[v] << endl;

			for (int to = 0; to<n; ++to)
				if (g[v][to] < min_e[to]) 
				{
					min_e[to] = g[v][to];
					sel_e[to] = v;
				}
			return Graph(g);
		}
	}
	Graph getSpaingTreeKruscal()
	{
		transformToListOfEdges();
		int cost = 0;
		vector<Edge> res;
		vector<Edge> edgeList = listOfEdge;
		sort(edgeList.begin(), edgeList.end());
		DSU p(listOfEdge.size());
		for (int i = 0; i < N; ++i)
			p.makeSet(i);
		for (int i = 0; i<M; ++i)
		{
			int a = edgeList[i].first, b = edgeList[i].second, l = edgeList[i].weight;
			if (p.find(a) != p.find(b))
			{
				cost += l;
				res.push_back(edgeList[i]);
				p.unite(a, b);
			}
		}
		return Graph(res);
	}
	Graph getSpaingTreeBoruvka()
	{
		transformToListOfEdges();
		vector<Edge> temp = listOfEdge;
		vector<Edge> edgeList;
		DSU dsu = DSU(N);
		for (size_t i = 0; i < N; ++i)
			dsu.makeSet(i);

		while (temp.size() < N - 1) {
			map<int, int> minEdges = map<int, int>();
			for (int i = 0; i < N; ++i)
				minEdges[i] = -1;
			for (int i = 0; i < temp.size(); ++i)
			{
				Edge edge = temp[i];
				int from = edge.first;
				int to = edge.second;
				int weight = edge.second;
				int fromComponent = dsu.find(from);
				int toComponent = dsu.find(to);

				if (fromComponent != toComponent) {
					if (minEdges[fromComponent] == -1 || temp[minEdges[fromComponent]].second > weight)
						minEdges[fromComponent] = i;
					if (minEdges[toComponent] == -1 || temp[minEdges[toComponent]].second > weight)
						minEdges[toComponent] = i;
				}
			}
			for (int i = 0; i < minEdges.size(); i++) {
				if (minEdges[i] != -1) {
					Edge edge = temp[minEdges[i]];
					dsu.unite(edge.first, edge.second);
					edgeList.push_back(edge);
				}
			}
		}
		return Graph(edgeList);
	}
	Graph(string s)
	{
		readGraph(s);
	}
	Graph(vector<vector<int> > matrix)
	{
		graphType = 'C';
		N = matrix.size();
		adjMatrix = matrix;
		isWeighted = true;
		isOriented = false;
	}
	Graph(vector<Edge> edgeList)
	{
		graphType = 'E';
		M = edgeList.size();
		for (int i = 0; i < M; i++)
			N = N > edgeList[i].maxId() ? N : edgeList[i].maxId();
		listOfEdge = edgeList;
		isWeighted = true;
		isOriented = false;
	}
	Graph(int N)
	{
		this->N = N;
		for (int i = 0; i < N; i++)
		{
			listOfEdge.push_back(Edge(i+1, i+1));
		}
	}

};
int main()
{
	Graph gMatrix = Graph("adjMatrix.txt");
	gMatrix.transformToListOfEdges();
	gMatrix.transformToAdjList();
	gMatrix.transformToAdjMatrix();
	gMatrix.writeGraph("outputAdjMatrix.txt");

	return 0;
}

