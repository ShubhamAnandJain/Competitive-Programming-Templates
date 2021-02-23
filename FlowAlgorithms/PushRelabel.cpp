// Implementation from cp-algo: Works really slow.
// Need to fix ASAP

struct PushRelabel{
 
	const int inf = 1e9;
	int n;
	vector<vector<int>> capacity, flow;
	vector<int> height, excess;
 
	PushRelabel(int n){
		this->n = n;
		capacity.assign(n, vector<int>(n,0));
	}
 
	void add_edge(int u, int v, int w){
		capacity[u][v] = w;
	}
 
	void push(int u, int v){
		int d = min(excess[u], capacity[u][v] - flow[u][v]);
		flow[u][v] += d;
		flow[v][u] -= d;
		excess[u] -= d;
		excess[v] += d;
	}
 
	void relabel(int u){
		int d = inf;
		for(int i = 0; i < n; i++){
			if(capacity[u][i] - flow[u][i] > 0){
				d = min(d, height[i]);
			}
		}
		if(d < inf){
			height[u] = d+1;
		}
	}
 
	vector<int> find_max_height_vertices(int s, int t){
		vector<int> max_height;
		for(int i = 0; i < n; i++){
			if(i != s && i != t && excess[i] > 0){
				if(!max_height.empty() && height[i] > height[max_height[0]]){
					max_height.clear();
				}
				if(max_height.empty() || height[i] == height[max_height[0]]){
					max_height.push_back(i);
				}
			}
		}
		return max_height;
	}	
 
	int max_flow(int s, int t){
		height.assign(n, 0);
		height[s] = n;
		flow.assign(n, vector<int>(n, 0));
		excess.assign(n, 0);
		excess[s] = inf;
		for(int i = 0; i < n; i++){
			if(i!=s) push(s, i);
		}
		vector<int> curr;
		while(!(curr = find_max_height_vertices(s,t)).empty()){
			forstl(i, curr){
				bool pushed = 0;
				for(int j = 0; j < n && excess[i]; j++){
					if(capacity[i][j] - flow[i][j] > 0 && height[i] == height[j]+1){
						push(i, j);
						pushed = 1;
					}
				}
				if(!pushed){
					relabel(i);
					break;
				}
			}
		}
		int max_flow = 0;
		for(int i = 0; i < n; i++){
			max_flow += flow[i][t];
		}
		return max_flow;
	}
};