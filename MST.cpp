#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

const int INF = 100000000;

class Graph{
public:
    std::vector<std::vector<std::pair<int, int>>> adj;
    std::vector<int> color;
    std::set<std::pair<int, int>> heap;
    std::vector<int> min_edg;
    void dfs(int v);
    explicit Graph(size_t n);

    int Prim();

    void addEdge(int v, int u, int weight);
};

Graph::Graph(size_t n): adj(std::vector<std::vector<std::pair<int, int>>>
        (n, std::vector<std::pair<int, int>>())), color(std::vector<int>(n)), min_edg(std::vector<int>(n, INF)){
}

void Graph::dfs(int v) {
    color[v] = 1;
    for(auto it = adj[v].begin(); it != adj[v].end(); ++it){
        if(color[it->first] == 0)
            dfs(it->first);
    }
    color[v] = 2;
}

void Graph::addEdge(int v, int u, int weight) {
    adj[v].push_back(std::pair<int, int>(weight, u));
    adj[u].push_back(std::pair<int, int>(weight, v));
}

int Graph::Prim() {
    heap.insert(std::make_pair(0, 0));
    min_edg[0] = 0;
    int ans = 0;
    for(int i = 0; i < adj.size(); ++i){
        int v = heap.begin()->second;
        ans += heap.begin()->first;
        color[v] = 1;
        heap.erase(heap.begin());
        for(auto it = adj[v].begin(); it != adj[v].end(); ++it){
            int to = it->second, cost = it->first;
            if(cost < min_edg[to] && color[to] == 0){
                heap.erase(std::make_pair(min_edg[to], to));
                heap.insert(std::make_pair(cost, to));
                min_edg[to] = cost;
            }
        }
    }
    return ans;
}


int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    int n, m, v, u, w;
    std::cin >> n >> m;
    Graph graph(n);
    for(int i = 0; i < m; ++i){
        std::cin >> v >> u >> w; --v; --u;
        graph.addEdge(v, u, w);
    }
    std::cout << graph.Prim();
    return 0;
}
