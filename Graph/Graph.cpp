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

	int unite(int x, int y)
	{
		if ((x = find(x)) == (y = find(y)))
			return 0;

		if (rank[x] <  rank[y])
			p[x] = y;
		else {
			p[y] = x;
			if (rank[x] == rank[y])
				++rank[x];
		}
		return 1;
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
			for (int i = 0; i < M; i++)
			{
				vector<V> temp;
				for (int j = 0; j < M; j++)
				{
					if (listOfEdge[j].first == i + 1 || listOfEdge[j].second == i + 1)
					{
						temp.push_back(V(listOfEdge[j].first == i + 1 ? listOfEdge[j].second : listOfEdge[j].first, listOfEdge[j].weight));
					}
				}
				adjVert.push_back(temp);
			}
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

			if (isWeighted)
				listOfEdge.push_back(Edge(tempInt[0], tempInt[1], tempInt[2]));
			else
				listOfEdge.push_back(Edge(tempInt[0], tempInt[1]));
		}
	}
	Graph getSpaingTreePrima()
	{
		transformToAdjList();
		vector<vector<V>> vertList = adjVert;

		DSU dsu = DSU(N + 1);

		vector<vector<V>> temp;
		int edgeCount = 0;
		vector<int> usedV;
		vector<V> secondV;
		vector<bool> isUsed;
		vector<Edge> edgeList;
		bool isInited = false;
		for (int i = 0; i < N; i++)
			isUsed.push_back(false);

		usedV.push_back(1);
		while (edgeCount < N - 1)
		{
			int minWeight = -1;
			int minId = -1;
			for (int i = 0; i < usedV.size(); i++)
			{
				for (int j = 0; j < vertList[usedV[i] - 1].size(); j++)
				{
					if ((vertList[usedV[i] - 1][j].weight < minWeight || minWeight == -1) && !isUsed[vertList[usedV[i] - 1][j].id - 1])
					{
						minWeight = vertList[usedV[i] - 1][j].weight;
						minId = vertList[usedV[i] - 1][j].id;
					}
				}
			}
			secondV.push_back(V(minId, minWeight));
			usedV.push_back(minId);
			isInited = true;
			isUsed[minId - 1] = true;
			edgeCount++;
		}
		for (int i = 0; i < N - 1; i++)
		{
			edgeList.push_back(Edge(usedV[i], secondV[i].id, secondV[i].weight));
		}
		return Graph(edgeList, N, N - 1);
	}
	Graph getSpaingTreeKruscal()
	{
		transformToListOfEdges();
		int cost = 0;
		vector<Edge> res;
		vector<Edge> edgeList = listOfEdge;
		sort(edgeList.begin(), edgeList.end());
		for (int i = 0; i < edgeList.size(); i++)
		{
			cout << edgeList[i].weight << " ";
		}
		DSU p(listOfEdge.size());
		for (int i = 0; i < M; ++i)
			p.makeSet(i);
		for (int i = 0; i < M; ++i)
		{
			if (p.find(edgeList[i].first) != p.find(edgeList[i].second))
			{
				res.push_back(edgeList[i]);
				p.unite(edgeList[i].first, edgeList[i].second);
			}
		}
		return Graph(res, N, res.size());
	}
	Graph getSpaingTreeBoruvka()
	{
		transformToListOfEdges();
		vector<Edge> temp = listOfEdge;
		vector<Edge> res;
		DSU dsu = DSU(N + 1);
		for (int i = 0; i < N; ++i)
			dsu.makeSet(i);

		while (res.size() < N - 1)
		{
			map<int, int> minEdges = map<int, int>();
			for (int i = 0; i < N; i++)
				minEdges[i] = -1;
			for (int i = 0; i < M; i++)
			{
				Edge edge = temp[i];
				int from = edge.first;
				int to = edge.second;
				int weight = edge.weight;
				int fromComponent = dsu.find(from);
				int toComponent = dsu.find(to);

				if (fromComponent != toComponent)
				{
					if (minEdges[fromComponent] == -1 || temp[minEdges[fromComponent]].weight > weight)
						minEdges[fromComponent] = i;
					if (minEdges[toComponent] == -1 || temp[minEdges[toComponent]].weight > weight)
						minEdges[toComponent] = i;
				}
			}
			for (int i = 0; i < minEdges.size(); i++) 
			{
				if (minEdges[i] != -1)
				{
					Edge edge = temp[minEdges[i]];
					int isSuccess = dsu.unite(edge.first, edge.second);
					if (isSuccess == 1)
						res.push_back(edge);
				}
			}
		}
		return Graph(res, N, res.size());
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
	Graph(vector<Edge> edgeList, int n, int m)
	{
		graphType = 'E';
		M = m;
		N = n;
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
	Graph()
	{

	}

};
int main()
{
	Graph g;
	g.readGraph("listOfEdges.txt");
	
	Graph b = g.getSpaingTreeKruscal();
	b.writeGraph("Kruscal.txt");

	Graph a = g.getSpaingTreeBoruvka();
	a.writeGraph("Boruvka.txt");

	Graph c = g.getSpaingTreePrima();
	c.writeGraph("Prima.txt");


	char ch;
	cin >> ch;
	return 0;
}

