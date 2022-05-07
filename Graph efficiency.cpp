#include <bits/stdc++.h>

using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::high_resolution_clock;
using namespace std;

void resetTime();
void showTime();
void bfs(vector<vector<bool>>& graph, vector<vector<int>>& dist, int v);
void distances(vector<vector<bool>>& graph, vector<vector<int>>& dist);
double evaluate(vector<vector<int>>& dist, int a, int b);
double efficiency(vector<vector<int>>& dist);
vector<double> frac(201);
void greedSolve(int n, int m, int k, vector<vector<bool>> graph, vector<vector<int>> dist);
void randomSolve(int n, int m, int k, vector<vector<bool>> graph, vector<vector<int>> dist);
pair<int, int> generateEdge(int n, vector<vector<bool>>& graph);
void tree(int n, vector<vector<bool>>& graph);
void addEdge(int n, vector<vector<bool>>& graph, vector<vector<int>>& dist);


int main()
{
	ios_base::sync_with_stdio(false); 
	cin.tie(NULL);
	
	for (int i = 1; i < 201; i++)
		frac[i] = 1.0 / double(i);
	while (true)
	{
		int n, m, k;
		cin >> n >> m >> k;
		vector<vector<bool>> graph(n, vector<bool>(n, false));
		vector<vector<int>> dist(n, vector<int>(n));
		/*
		tree(n, graph);
		for (int i = 0; i < m - n + 1; i++)
		{
			pair<int, int> edge = generateEdge(n, graph);
			graph[edge.first][edge.second] = true;
			graph[edge.second][edge.first] = true;
			int u, v;
			cin >> u >> v; u--; v--;
			graph[u][v] = true;
			graph[v][u] = true;
		}
		resetTime();
		showTime();
		*/
		for (int i = 0; i < m; i++)
		{
			int u, v;
			cin >> u >> v; u--; v--;
			graph[u][v] = true;
			graph[v][u] = true;
		}		
		
		resetTime();
		greedSolve(n, m, k, graph, dist);
		showTime();	
		resetTime();
		randomSolve(n, m, k, graph, dist);
		showTime();
		cout << endl;
	}
}

void greedSolve(int n, int m, int k, vector<vector<bool>> graph, vector<vector<int>> dist)
{
	distances(graph, dist);
	
	///cout << "efficiency without edges: "<< efficiency(dist) << endl;
	
	for (int k_ = 0; k_ < k; k_++)
	{
		addEdge(n, graph, dist);
	}	
	///cout << "greedy efficiency: "<< efficiency(dist) << endl;
}

void randomSolve(int n, int m, int k, vector<vector<bool>> graph, vector<vector<int>> dist)
{
	vector<pair<int, int>> newEdges(k);
	for (int i = 0; i < k; i++)
	{
		newEdges[i] = generateEdge(n, graph);
		graph[newEdges[i].first][newEdges[i].second] = true;
		graph[newEdges[i].second][newEdges[i].first] = true;
	}
	distances(graph, dist);
	for (int k_ = 0; k_ < k; k_++)
	{
		graph[newEdges[k_].first][newEdges[k_].second] = false;
		graph[newEdges[k_].second][newEdges[k_].first] = false;
		distances(graph, dist);
		addEdge(n, graph, dist);
	}	
	///cout << "greedy random efficiency: "<< efficiency(dist) << endl;

}


void tree(int n, vector<vector<bool>>& graph)
{
	for (int i = 1; i < n; i++)
	{
		int v = rand() % i;
		graph[i][v] = true;
		graph[v][i] = true;
	}
}


pair<int, int> generateEdge(int n, vector<vector<bool>>& graph)
{
	int e1 = rand() % n, e2 = rand() % n;
	if (e1 == e2 || graph[e1][e2] == true)
		return generateEdge(n, graph);
	return make_pair(e1, e2);
}

void addEdge(int n, vector<vector<bool>>& graph, vector<vector<int>>& dist)
{	
	double maxsum = 0;	
	int edge1, edge2;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{	
			if (graph[i][j])
				continue;
			double cursum = evaluate(dist, i, j);
			if (cursum > maxsum)
			{	
				edge1 = i;
				edge2 = j;
				maxsum = cursum;	
			}
		}
	}
	cout << edge1 + 1 << " " << edge2 + 1 << "\n";
	graph[edge1][edge2] = true;
	graph[edge2][edge1] = true;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{
			int newl = min(min((dist[i][edge1] + dist[j][edge2] + 1), (dist[i][edge2] + dist[j][edge1] + 1)), dist[i][j]);
			dist[i][j] = newl;
			dist[j][i] = newl;
		}
	}
}

double evaluate(vector<vector<int>>& dist, int a, int b)
{
	int n = dist.size();
	double s = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < i; j++)
		{	
			s += frac[min(dist[i][j], min(dist[i][a] + dist[j][b] + 1, dist[j][a] + dist[i][b] + 1))];
		}
	}
	return s;
}

double efficiency(vector<vector<int>>& dist)
{
	int n = dist.size();
	double s = 0;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{	
			s += frac[dist[i][j]];
		}
	}
	s /= (n - 1) * n;
	return s;
}



void bfs(vector<vector<bool>>& graph, vector<vector<int>>& dist, int v)
{
	int n = graph.size();
	vector<bool> visited(n, false);
	queue<int> q;
	q.push(v);
	visited[v] = true;
	int level = 1, c = 1, newc = 0;
	while (!q.empty())
	{
		int vert = q.front();
		q.pop();
		for (int  i = 0; i < n; i++)
		{
			if (visited[i] == true || graph[vert][i] == false)
				continue;
			q.push(i);
			dist[v][i] = level;
			dist[i][v] = level;
			visited[i] = true;
			newc++;
		}
		c--;
		if (c == 0)
		{
			level++;
			c = newc;
			newc = 0;
		}
	}
}
void distances(vector<vector<bool>>& graph, vector<vector<int>>& dist)
{
	int n = graph.size();
	for (int i = 0; i < n; i++)
	{
		bfs(graph, dist, i);
	}
}

std::chrono::time_point<std::chrono::high_resolution_clock> resettime;

void resetTime()
{
	resettime = high_resolution_clock::now();
}

void showTime()
{
	auto currtime = high_resolution_clock::now();
	duration<double, std::milli> ms = currtime - resettime;
	cout << ms.count() << " ms ";
}