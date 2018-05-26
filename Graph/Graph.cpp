// Graph.cpp: îïðåäåëÿåò òî÷êó âõîäà äëÿ êîíñîëüíîãî ïðèëîæåíèÿ.
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
	// C ÌÀÒÐÈÖÀ ÑÌÅÆÍÎÑÒÈ
	// L ÑÏÈÑÎÊ ÑÌÅÆÍÛÕ ÂÅÐØÈÍ
	// E ÑÏÈÑÎÊ ÐÅÁÅÐ

	long int N; // ÊÎËÈ×ÅÑÒÂÎ ÂÅÐØÈÍ

	long int M; // ÊÎËÈ×ÅÑÂÎ ÐÅÁÅÐ
	
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
		DSU dsu(N + 1);
		for (int i = 0; i < N + 1; i++)
			dsu.makeSet(i);

		if (oldGraphType == 'C')
		{
			M = 0;
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
			M = 0;
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < adjVert[i].size(); j++)
				{
					if (dsu.find(adjVert[i][j].id) != dsu.find(i + 1))
					{
						dsu.unite(adjVert[i][j].id, i + 1);
						listOfEdge.push_back(Edge(i + 1, adjVert[i][j].id, adjVert[i][j].weight));
						M++;
					}
				}
			}
			adjVert.clear();
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
		sort(listOfEdge.begin(), listOfEdge.end());
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
		if (tempString != "")
			tempInts.push_back(stoi(tempString));
		return tempInts;
	}
	void readAdjMatrix(string s) // ÌÀÒÐÈÖÀ ÑÌÅÆÍÎÑÒÈ
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

	void readListAdjVertices(string s) // ÑÏÈÑÎÊ ÑÌÅÆÍÛÕ ÂÅÐØÈÍ
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
	void readEdgeList(string s) // ÑÏÈÑÎÊ ÐÅÁÅÐ
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
		isOriented = tempString[0] - '0';
		isWeighted = tempString[2] - '0';

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
	char currentMark(bool b)
	{
		if (b)
			return 'A';
		else
			return 'B';
	}
	int CheckBipart(vector<char> marks)
	{
		Graph g;
		if (graphType == 'C')
			g = Graph(adjMatrix);
		if (graphType == 'L')
			g = Graph(adjVert, N);
		if (graphType == 'E')
			g = Graph(listOfEdge, N, M);
		g.transformToAdjList();
		vector<vector<V>> tempV = g.adjVert;
		bool isA = true;
		int currentV = 0;
		while (true)
		{
			if (marks[currentV] != 'A' || marks[currentV] != 'B')
				marks[currentV] = currentMark(isA);
			for (int j = 0; j < tempV[currentV].size(); j++)
			{
				if (marks[tempV[currentV][j].id - 1] == 'A' || marks[tempV[currentV][j].id - 1] == 'B')
				{ 
					if (marks[tempV[currentV][j].id - 1] == marks[currentV])
					{
						cout << tempV[currentV][j].id - 1 << " " << currentV;
						return 0;
					}
				}
				else
					marks[tempV[currentV][j].id - 1] = currentMark(!isA);
			}
			isA = !isA;
		}
		return 1;
	}

	vector<pair<int, int> >getMaximumMatchingBipart()
	{

	}
	Graph getSpaingTreePrima()
	{
		Graph g;
		if (graphType == 'C')
			g = Graph(adjMatrix);
		if (graphType == 'L')
			g = Graph(adjVert, N);
		if (graphType == 'E')
			g = Graph(listOfEdge, N, M);

		g.transformToAdjList();
		vector<vector<V>> vertList = g.adjVert;
		vector<vector<V>> temp;
		int edgeCount = 0;
		vector<int> usedV;
		vector<V> secondV;
		vector<bool> isUsed(N, false);
		vector<Edge> edgeList;

		usedV.push_back(1);
		isUsed[0] = true;
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
			isUsed[minId - 1] = true;
			edgeCount++;
		}
		int cost = 0;
		for (int i = 0; i < N - 1; i++)
		{
			cost += secondV[i].weight;
			edgeList.push_back(Edge(usedV[i], secondV[i].id, secondV[i].weight));
		}
		cout << cost << "\n";
		return Graph(edgeList, N, N - 1);
	}
	Graph getSpaingTreeKruscal()
	{
		Graph g;
		if (graphType == 'C')
			g = Graph(adjMatrix);
		if (graphType == 'L')
			g = Graph(adjVert, N);
		if (graphType == 'E')
			g = Graph(listOfEdge, N, M);

		g.transformToListOfEdges();

		int cost = 0;
		vector<Edge> res;
		vector<Edge> edgeList = g.listOfEdge;
		sort(edgeList.begin(), edgeList.end());
		DSU p(N + 1);
		for (int i = 0; i < g.N + 1; i++)
			p.makeSet(i);
		for (int i = 0; i < g.M; i++)
		{
			if (p.find(edgeList[i].first) != p.find(edgeList[i].second))
			{
				res.push_back(edgeList[i]);
				p.unite(edgeList[i].first, edgeList[i].second);
				cost += edgeList[i].weight;
			}
		}
		cout << cost << "\n";
		return Graph(res, N, N - 1);
	}
	Graph getSpaingTreeBoruvka()
	{
		Graph g;
		if (graphType == 'C')
			g = Graph(adjMatrix);
		if (graphType == 'L')
			g = Graph(adjVert, N);
		if (graphType == 'E')
			g = Graph(listOfEdge, N, M);

		g.transformToListOfEdges();
		int cost = 0;
		vector<Edge> temp = g.listOfEdge;
		vector<Edge> res;
		DSU dsu = DSU(N + 1);
		for (int i = 0; i < N; ++i)
			dsu.makeSet(i);

		while (res.size() < g.N - 1)
		{
			map<int, int> minEdges = map<int, int>();
			for (int i = 0; i < g.N; i++)
				minEdges[i] = -1;
			for (int i = 0; i < g.M; i++)
			{
				Edge edge = temp[i];

				if (dsu.find(edge.first) != dsu.find(edge.second))
				{
					if (minEdges[dsu.find(edge.second)] == -1 || temp[minEdges[dsu.find(edge.second)]].weight > edge.weight)
						minEdges[dsu.find(edge.second)] = i;
					if (minEdges[dsu.find(edge.first)] == -1 || temp[minEdges[dsu.find(edge.first)]].weight > edge.weight)
						minEdges[dsu.find(edge.first)] = i;
				}
			}
			for (int i = 0; i < minEdges.size(); i++) 
			{
				if (minEdges[i] != -1)
				{
					Edge edge = temp[minEdges[i]];
					int isSuccess = dsu.unite(edge.first, edge.second);
					if (isSuccess == 1)
					{
						res.push_back(edge);
						cost += edge.weight;
					}
				}
			}
		}
		cout << cost << "\n";
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
		listOfEdge = edgeList;
		isWeighted = true;
		isOriented = false;
	}
	Graph(vector<vector<V>> vertList, int n)
	{
		graphType = 'L';
		N = n;
		adjVert = vertList;
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
	g.readGraph("test.txt");
	Graph b = g.getSpaingTreeKruscal();
	b.writeGraph("Kruscal.txt");

	Graph a = g.getSpaingTreeBoruvka();
	a.writeGraph("Boruvka.txt");

	Graph c = g.getSpaingTreePrima();
	c.writeGraph("Prima.txt");
	//vector<char> marks(1e5, ' ');
	//cout << "CHECK BIPART " << d.CheckBipart(marks) ;
	cout << "finished";
	char ch;
	cin >> ch;
	return 0;
}

