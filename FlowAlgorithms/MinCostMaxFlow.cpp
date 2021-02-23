// Mincost-Maxflow with potentials
// Running Dijkstra after choosing a correct potential function
// Complexity: non-polynomial if the edge capacities are large
// If edge capacities are constant, then NlogN if potentials are fixed
// If potentials not fixed, read fix_potentials function
// References:
// 1) https://cp-algorithms.com/graph/min_cost_flow.html
// 2) https://cses.fi/paste/3525f456fb3bd9b1172866/
// 3) https://cp-algorithms.com/graph/bellman_ford.html
// Sample Submission: https://codeforces.com/gym/102759/submission/108205885
// Usage: MCMF m(n), add_edge(a,b,cap,cost), mincost_maxflow(s, t)
// In case edge weights are negative, read comments on fix_potential below

struct edge{
	int v, cap, flow, cost;
};

struct MCMF{
	int n;
	vector<edge> edges;
	vector<vector<int>> adj;
	vector<int> dist, pot, par, flow;
	vector<bool> vis;

	// Note: you might need to fix potentials beforehand
	// For this, just run fix_potentials(s)
	// If graph is something like line, instead just do pot[i] = -i * 1e12
	// Make sure that initially the edge weights are positive: d(u,v) = pot(u)-pot(v)+w(u,v)
	// Thanks to Monogon for providing the code this was based on!

	int inf = 2e18; // max distance from source to destination

	MCMF(int n){
		this->n = n;
		adj.resize(n);
		dist.assign(n, 0);
		pot.assign(n, 0);
		vis.resize(n);
		par.resize(n);
		flow.resize(n);
	}
	
	void fix_potentials(int s){
			
		// Run SPFA to fix the potentials
		// Complexity: O(nm) worst case, O(m) average case
		// In general, run if the edges might have negative costs

		fill(pot.begin(), pot.end(), inf);
		pot[s] = 0;
		vector<int> cnt(n, 0);
		vector<bool> in_queue(n, 0);
		queue<int> q;
		q.push(s);
		in_queue[s] = 1;
		while(!q.empty()){
			int v = q.front();
			q.pop();
			in_queue[v] = 0;
			forstl(r, adj[v]){
				if(edges[r].cap == edges[r].flow) continue;
				int to = edges[r].v;
				int len = edges[r].cost;
				if(pot[v] + len < pot[to]){
					pot[to] = pot[v] + len;
					if(!in_queue[to]){
						q.push(to);
						in_queue[to] = 1;
						cnt[to]++;
						assert(cnt[to] <= n); // negative cycle check
					}
				}
			}
		}

	}

	void add_edge(int u, int v, int cap, int cost){
		int sz = edges.size();
		edges.push_back({v, cap, 0, cost});
		edges.push_back({u, cap, cap, -cost});
		adj[u].push_back(sz);
		adj[v].push_back(sz^1);
	}

	pair<int,int> mincost_maxflow(int s, int t, int amt){
		int curr_flow = 0, cost = 0;
		while(curr_flow < amt){
			priority_queue<pair<int,int>> q;
			fill(dist.begin(), dist.end(), inf);
			fill(vis.begin(), vis.end(), 0);
			q.push({0, s});
			dist[s] = 0;
			flow[s] = amt - curr_flow;
			while(!q.empty()){
				// running dijkstra to find path
				int curr_dist, v;
				tie(curr_dist, v) = q.top();
				q.pop();
				curr_dist = -curr_dist;
				if(vis[v]) continue;
				vis[v] = 1;
				forstl(r, adj[v]){
					int nxt = edges[r].v;
					int new_dist = curr_dist + edges[r].cost + pot[v] - pot[nxt];
					if(vis[nxt]) continue;
					if(edges[r].flow == edges[r].cap) continue;
					if(new_dist >= dist[nxt]) continue;
					dist[nxt] = new_dist;
					par[nxt] = r;
					flow[nxt] = min(flow[v], edges[r].cap - edges[r].flow);
					q.push({-new_dist, nxt});
				}
			}
			if(!vis[t]) break;
			for(int i = 0; i < n; i++){
				dist[i] += pot[i] - pot[s];
			}
			cost += dist[t] * flow[t];
			curr_flow += flow[t];
			int last = t;
			while(last != s){
				edges[par[last]].flow += flow[t];
				edges[par[last]^1].flow -= flow[t];
				last = edges[par[last]^1].v;
			}
			dist.swap(pot);
		}
		return {curr_flow, cost};	
	}
};