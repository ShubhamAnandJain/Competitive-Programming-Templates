// Edmond-Karp algorithm for max flow
// Basically Ford Fulkerson with BFS
// Complexity: O(VE*(V+E))
// Reference: https://cp-algorithms.com/graph/edmonds_karp.html
// Sample Submission: https://codeforces.com/contest/653/submission/108166750
// Usage: 0-indexed. Just call EdmondKarp name(n), then addEdge and call maxflow(s,t)

struct EdmondKarp{
	int n; 
	vector<vector<int>> cap;
	vector<vector<int>> adj;

	EdmondKarp(int n){
		this->n = n;
		cap.assign(n, vector<int>(n, 0));
		adj.assign(n, vector<int>(n, 0));
		for(int i = 0; i < n; i++){
			for(int j = 0; j < n; j++){
				adj[i][j] = j;
			}
		}
	}

	void addEdge(int u, int v, int w){
		cap[u][v] = w;
	}

	int bfs(int s, int t, vector<int>& parent){
		fill(parent.begin(), parent.end(), -1);
		parent[s] = -2;
		queue<pair<int,int>> q;
		q.push({s, 2e9});

		while(!q.empty()){
			int cur = q.front().first;
			int flow = q.front().second;
			q.pop();
			forstl(nxt, adj[cur]){
				if(parent[nxt] == -1 && cap[cur][nxt] > 0){
					parent[nxt] = cur;
					int new_flow = min(flow, cap[cur][nxt]);
					if(nxt == t){
						return new_flow;
					}
					q.push({nxt, new_flow});
				}
			}
		}
		return 0;
	}

	int maxflow(int s, int t){
		int flow = 0;
		vector<int> parent(n);
		int new_flow;

		while(new_flow = bfs(s, t, parent)){
			flow += new_flow;
			int cur = t;
			while(cur != s){
				int prev = parent[cur];
				cap[prev][cur] -= new_flow;
				cap[cur][prev] += new_flow;
				cur = prev;
			}
		}
		return flow;
	}
};