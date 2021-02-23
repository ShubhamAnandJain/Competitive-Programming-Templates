// Biconnected Component Decomposition of a graph
// Complexity: O(V+E)
// References:
// 1) https://cp-algorithms.com/graph/bridge-searching.html
// 2) https://www.quora.com/q/threadsiiithyderabad/The-Bridge-Tree-of-a-graph
// Sample Submission: https://codeforces.com/contest/652/submission/108176064
// Usage: 0-indexed. Just call BCC(n), add_edge(u, v) and then find_bcc()
// tree will store the final compressed graph representation
// components[i] will store the vertices in component # i (could be empty)
// For actual number of components, find number of i in 0..n-1 such that components[i].size() > 0

struct BCC{
	int n;
	vector<vector<int>> adj;
	vector<vector<int>> edge_num; // to convert from adj list to edge representation 
	int num_edges; // initially 0 edges
	vector<int> is_bridge;
	vector<bool> visited;
	vector<int> tin, low;
	int timer;
	int cmp_no;
	vector<vector<int>> tree; // Final graph representation
	vector<vector<int>> components;
	vector<queue<int>> Q;

	BCC(int n){
		this->n = n;
		adj.assign(n, vector<int>());
		edge_num.assign(n, vector<int>());
		num_edges = 0;
	}

	void add_edge(int a, int b, int arti){
		// add undirected edge a --- b
		adj[a].push_back(b);
		adj[b].push_back(a);
		edge_num[a].push_back(num_edges);
		edge_num[b].push_back(num_edges);
		num_edges++;
	}

	void dfs(int v, int p = -1){
		visited[v] = true;
		tin[v] = low[v] = timer++;
		for(int iter = 0; iter < adj[v].size(); iter++){
			int to = adj[v][iter];
			if(to == p) continue; // parent of vertex
			if(visited[to]){
				low[v] = min(low[v], tin[to]);
			}
			else{
				dfs(to, v);
				low[v] = min(low[v], low[to]);
				if(low[to] > tin[v]){
					// mark this edge as bridge edge
					is_bridge[edge_num[v][iter]] = 1;
				}
			}
		}
	}

	void find_bridges(){
		timer = 0;
		visited.assign(n, 0);
		tin.assign(n, -1);
		low.assign(n, -1);
		is_bridge.assign(num_edges, 0);
		for(int i = 0; i < n; i++){
			if(!visited[i]){
				dfs(i);
			}
		}
	}

	void component_dfs(int v){
		int curr_cmp = cmp_no;
		Q[curr_cmp].push(v);
		visited[v] = 1;
		while(!Q[curr_cmp].empty()){
			int u = Q[curr_cmp].front();
			Q[curr_cmp].pop();
			components[curr_cmp].push_back(u);
			for(int i = 0; i < adj[u].size(); i++){
				int edge = edge_num[u][i];
				int nxt = adj[u][i];
				if(visited[nxt]) continue;
				if(is_bridge[edge]){
					cmp_no++;
					tree[curr_cmp].push_back(cmp_no);
					tree[cmp_no].push_back(curr_cmp);
					component_dfs(nxt);
				}
				else{
					Q[curr_cmp].push(nxt);
					visited[nxt] = 1;
				}
			}
		}
	}

	void find_bcc(){
		find_bridges();
		Q.assign(n, queue<int>());
		components.assign(n, vector<int>());
		tree.assign(n, vector<int>());
		cmp_no = 0;
		visited.assign(n, 0);
		for(int i = 0; i < n; i++){
			if(!visited[i]){
				component_dfs(i);
				cmp_no++;
			}
		}
	}
};
