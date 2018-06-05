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
#include <queue>
#include <set>
#include <stack>
#include <ctime>

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

	vector<vector<V> > adjVertDuplicate;

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

		if (oldGraphType == 'L' && adjVert.size() != 0)
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
		}
		if (oldGraphType == 'E' && listOfEdge.size() != 0)
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
		}
		graphType = 'C';
	}
	void transformToAdjList()
	{
		//int count = 0;
		char oldGraphType = graphType;
		if (oldGraphType == 'C' && adjMatrix.size() != 0)
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
		}
		else if (oldGraphType == 'E' && listOfEdge.size() != 0)
		{
			
			for (int i = 0; i < N; i++)
			{
				vector<V> temp;
				for (int j = 0; j < M; j++)
				{
					if (listOfEdge[j].first == i + 1)
					{
						temp.push_back(V(listOfEdge[j].first == i + 1 ? listOfEdge[j].second : listOfEdge[j].first, listOfEdge[j].weight));
					}
				}
				//count += temp.size();
				adjVert.push_back(temp);
			}
		}
		graphType = 'L';
		//cout << count << "\n";


	}
	void transformToListOfEdges()
	{
		char oldGraphType = graphType;
		DSU dsu(N + 1);
		for (int i = 0; i < N + 1; i++)
			dsu.makeSet(i);

		if (oldGraphType == 'C' && adjMatrix.size() != 0)
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
		}
		else if (oldGraphType == 'L' && adjVert.size() != 0)
		{
			M = 0;
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < adjVert[i].size(); j++)
				{
					if (find(listOfEdge.begin(), listOfEdge.end(), Edge(i + 1, adjVert[i][j].id, adjVert[i][j].weight)) != listOfEdge.end())
						if (find(listOfEdge.begin(), listOfEdge.end(), Edge(adjVert[i][j].id, i + 1, adjVert[i][j].weight)) != listOfEdge.end())
						{
							listOfEdge.push_back(Edge(i + 1, adjVert[i][j].id, adjVert[i][j].weight));
							M++;
						}
				}
			}
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

	Graph getSpaingTreePrima()
	{
		///////////////////////
	/*	Graph g;
		if (graphType == 'C')
			g = Graph(adjMatrix);
		if (graphType == 'L')
			g = Graph(adjVert, N);
		if (graphType == 'E')
			g = Graph(listOfEdge, N, M);
			*/
		transformToAdjList();
	/*	vector<vector<V>> vertList = g.adjVert;*/
		int edgeCount = 0;
		vector<bool> isUsed(N, false);
		vector<Edge> edgeList;

		isUsed[0] = true;
		int to;
		int cost;
		int totalCost = 0;
		int n = N;
		const int INF = 1000000000;

		vector<int> minEdge(n, INF);
		vector<int> selectedEdge(n, -1);
		minEdge[0] = 0;
		set < pair<int, int> > q;
		q.insert(make_pair(0, 0));
		int k = 0;
		for (int i = 0; i < n; ++i)
		{
			int v = q.begin()->second;
			q.erase(q.begin());
			isUsed[v] = true;
			if (selectedEdge[v] != -1)
			{
				edgeList.push_back(Edge(v + 1, selectedEdge[v] + 1, minEdge[v]));
				totalCost += minEdge[v];
			}
			for (size_t j = 0; j < adjVert[v].size(); ++j)
			{
				to = adjVert[v][j].id - 1;
				cost = adjVert[v][j].weight;
				if (cost < minEdge[to] && !isUsed[to])
				{
					q.erase(make_pair(minEdge[to], to));
					minEdge[to] = cost;
					selectedEdge[to] = v;
					q.insert(make_pair(minEdge[to], to));
				}
			}
		}
		//cout << totalCost;
		return Graph(edgeList, N, N - 1);
	}
	Graph getSpaingTreeKruscal()
	{
		//Graph g;
		//if (graphType == 'C')
		//	g = Graph(adjMatrix);
		//if (graphType == 'L')
		//	g = Graph(adjVert, N);
		//if (graphType == 'E')
		//	g = Graph(listOfEdge, N, M);

		transformToListOfEdges();

		int cost = 0;
		int count = 0;
		vector<Edge> res;
	/*	vector<Edge> edgeList = g.listOfEdge;*/
		sort(listOfEdge.begin(), listOfEdge.end());
		DSU p(N + 1);
		for (int i = 0; i < N + 1; i++)
			p.makeSet(i);
		for (int i = 0; i < M; i++)
		{
			if (p.find(listOfEdge[i].first) != p.find(listOfEdge[i].second))
			{
				res.push_back(listOfEdge[i]);
				p.unite(listOfEdge[i].first, listOfEdge[i].second);
				cost += listOfEdge[i].weight;
			}
		}
		//cout << cost << "\n";
		return Graph(res, N, N - 1);
	}
	Graph getSpaingTreeBoruvka()
	{
		//Graph g;
		//if (graphType == 'C')
		//	g = Graph(adjMatrix);
		//if (graphType == 'L')
		//	g = Graph(adjVert, N);
		//if (graphType == 'E')
		//	g = Graph(listOfEdge, N, M);

		transformToListOfEdges();
		int cost = 0;
		/*vector<Edge> temp = g.listOfEdge;*/
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
				Edge edge = listOfEdge[i];

				if (dsu.find(edge.first) != dsu.find(edge.second))
				{
					if (minEdges[dsu.find(edge.first)] == -1 || edge.weight < listOfEdge[minEdges[dsu.find(edge.first)]].weight)
						minEdges[dsu.find(edge.first)] = i;
					if (minEdges[dsu.find(edge.second)] == -1 || edge.weight < listOfEdge[minEdges[dsu.find(edge.second)]].weight)
						minEdges[dsu.find(edge.second)] = i;
				}
			}
			for (int i = 0; i < minEdges.size(); i++) 
			{
				if (minEdges[i] != -1)
				{
					Edge edge = listOfEdge[minEdges[i]];
					int isSuccess = dsu.unite(edge.first, edge.second);
					if (isSuccess == 1)
					{
						res.push_back(edge);
						cost += edge.weight;
					}
				}
			}
		}
		//cout << cost << "\n";
		return Graph(res, N, res.size());
	}

	char currentMark(char c)
	{
		if (c == 'B')
			return 'A';
		else
			return 'B';
	}
	int checkBipart(vector<char> marks)
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
		bool isAllDone = true;
		int currentV = 0;
		marks[0] = 'A';
		int i = 0;
		vector<bool> isUsed = vector<bool>(N, false);
		vector<bool> isWatched = vector<bool>(N, false);
		int watchedCount = 0;
		isUsed[0] = true;
		int count = 1;
		while (watchedCount < N)
		{
			isAllDone = true;
			for (int i = 0; i < N; i++)
			{
				if (vertList[i].size() == 0 && !isWatched[i])
				{
					isWatched[i] = true;
					marks[i] = 'A';
					watchedCount++;
				}
				if (isUsed[i] && !isWatched[i])
				{
					isAllDone = false;
					isWatched[i] = true;
					watchedCount++;
					for (int j = 0; j < vertList[i].size(); j++)
					{
						if (marks[vertList[i][j].id - 1] == ' ')
						{
							marks[vertList[i][j].id - 1] = currentMark(marks[i]);
							isUsed[vertList[i][j].id - 1] = true;
						}
						else if (marks[vertList[i][j].id - 1] != currentMark(marks[i]))
						{
							return 0;
						}
					}
				}

			}
			if (isAllDone)
				for (int i = 0; i < N; i++)
				{
					if (!isUsed[i])
					{
						isUsed[i] = true;
						break;
					}
				}
		}
		bipartMarks = marks;
		return 1;
	}
	vector<int> currentMatch;
	vector<bool> used;
	vector<char> bipartMarks;
	vector<vector<V>> BHalf;

	vector<pair<int, int> > getMaximumMatchingBipart()
	{
		transformToAdjList();
		int n = 0;
		int k = 0;
		
		vector<pair<int, int> > result;
		for (int i = 0; i < N; i++)
		{
			if (bipartMarks[i] == 'A')
			{
				BHalf.push_back(adjVert[i]);
				n++;
			}
			else if (bipartMarks[i] == 'B')
			{
				BHalf.push_back(vector<V>());
				k++;
			}
			//cout << bipartMarks[i] << " " << i + 1 << endl;
		}
		currentMatch.assign(N, -1);
		vector<bool> usedHalf(N);
		for (int i = 0; i < N; ++i)
			for (int j = 0; j < BHalf[i].size(); ++j)
				if (currentMatch[BHalf[i][j].id - 1] == -1)
				{
					currentMatch[BHalf[i][j].id - 1] = i;
					usedHalf[i] = true;
					break;
				}
		for (int i = 0; i < n; ++i)
		{
			if (usedHalf[i])
				continue;
			used.assign(n, false);
			findIncreaseChain(i);
		}

		for (int i = 0; i < N; ++i)
			if (currentMatch[i] != -1)
				result.push_back(pair<int, int>(currentMatch[i] + 1, i + 1));
		return result;
	}
	bool findIncreaseChain(int v)
	{
		if (used[v])
			return false;
		used[v] = true;
		for (int i = 0; i < BHalf[v].size(); ++i)
		{
			int to = BHalf[v][i].id - 1;
			if (currentMatch[to] == -1 || findIncreaseChain(currentMatch[to]))
			{
				currentMatch[to] = v;
				return true;
			}
		}
		return false;
	}

	//vector<int> getEuleranTourFleri()
	//{
	//	int currentV = eulerPriorityV;
	//	auto vertList = adjVert;
	//	vector<int> path;
	//	while (true)
	//	{
	//		int nextV = -1;
	//		for (int i = 0; i < vertList[currentV].size(); i++)
	//		{
	//			bool vert = isBridge(i, vertList[currentV][i].id - 1, vertList);
	//			if ((vert && vertList[currentV].size() == 1) || !vert)
	//			{
	//				nextV = vertList[currentV][i].id - 1;
	//				vertList[currentV].erase(vertList[currentV].begin + i);
	//				currentV = nextV;
	//				path.push_back(currentV + 1);
	//				break;
	//			}
	//		}
	//		if (nextV == -1)
	//			break;
	//	}
	//}
	//vector<int> getEuleranTourEffective() 
	//{
	//	//TODO
	//}

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
	Graph(vector<vector<V> > vertList, int n)
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

	g.readGraph("test.txt"); //  testBipartCrush		test
	Graph b = g.getSpaingTreeKruscal();
	b.writeGraph("Kruscal.txt");

	Graph a = g.getSpaingTreeBoruvka();
	a.writeGraph("Boruvka.txt");
	
	Graph c = g.getSpaingTreePrima();
	c.writeGraph("Prima.txt");
	
	//Graph d = g;
	//bool b = false;
	//
	//vector<char> marks(1e5, ' ');
	//if (!g.checkBipart(marks))
	//	cout << -1 << endl;
	//else
	//{
	//	vector<pair<int, int> > vec = g.getMaximumMatchingBipart();
	//	cout << vec.size() << endl;
	//	for (auto i : vec)
	//		cout << i.first << " " << i.second << endl;
	//}
	cout << "finished";
	char ch;
	cin >> ch;
	return 0;
}

