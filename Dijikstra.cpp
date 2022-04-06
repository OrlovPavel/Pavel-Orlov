#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

const int INF = 100000000;

class Graph{
public:
    std::vector<std::vector<std::pair<int, long long >>> adj;
    std::vector<int> color;
    std::vector<long long> way;
    void dfs(int v);
    explicit Graph(size_t n);

    void dijikstra(int s);

    void addEdge(int u, int v, long long weight);
};

Graph::Graph(size_t n): adj(std::vector<std::vector<std::pair<int, long long >>>(n, std::vector<std::pair<int, long long >>())), color(std::vector<int>(n)),
                        way(std::vector<long long>(n, INF)){
}

void Graph::dfs(int v) {
    color[v] = 1;
    for(auto it = adj[v].begin(); it != adj[v].end(); ++it){
        if(color[it->first] == 0)
            dfs(it->first);
    }
    color[v] = 2;
}

void Graph::addEdge(int u, int v, long long weight) {
    adj[u].push_back(std::pair<int, long long>(v, weight));

}

void Graph::dijikstra(int s) {
    std::set<std::pair<int, int>> unused;
    for(int i = 0; i < adj.size(); ++i){
        unused.insert(std::pair<int, int>(i == s? 0: INF, i));
    }
    way[s] = 0;
    while(!unused.empty()){
        int v = unused.begin()->second;
        unused.erase(unused.begin());
        if(way[v] == INF)
            break;
        for(auto it = adj[v].begin(); it != adj[v].end(); ++it){
            if(way[it->first] > way[v] + it->second){
                unused.erase(std::pair<int, int>(way[it->first], it->first));
                way[it->first] = way[v] + it->second;
                unused.insert(std::pair<int, int>(way[it->first], it->first));
            }
        }
    }
}


int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    int n, m, v, u, s, t, w;
    std::cin >> n;
    Graph graph(n);
    std::cin >> m >> s >> t; --s; --t;
    for(int i = 0; i < m; ++i){
        std::cin >> u >> v >> w; --u; --v;
        graph.addEdge(u, v, w);
        graph.addEdge(v, u, w);
    }
    graph.dfs(s);
    if(graph.color[t] == 0){
        std::cout << -1;
        return 0;
    }
    /*for(int i = 0; i < n; ++i)
        if(graph.color[i] == 0)
            graph.way[i] = 2 * INF;*/
    graph.dijikstra(s);
    std::cout << graph.way[t];
    return 0;
}
