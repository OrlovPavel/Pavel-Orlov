#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

const long long INF = 4000000000000000000;

class Graph{
public:
    std::vector<std::vector<std::pair<int, long long >>> adj;
    std::vector<int> color;
    std::vector<long long> way;
    void dfs(int v);
    explicit Graph(size_t n);

    void fordBelman(int s);

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

void Graph::fordBelman(int s) {
    way[s] = 0;
    for(int i = 0; i < adj.size(); ++i){
        int t = 0;
        for(auto v = adj.begin(); v != adj.end(); ++v, ++t){
            for(auto u = v->begin(); u != v->end(); ++u){
                if(way[t] != 2 * INF)
                    way[u->first] = std::min(way[u->first], way[t] + u->second);
            }
        }
    }
    int t = 0;
    std::vector<int> cycled;
    for (auto v = adj.begin(); v != adj.end(); ++v, ++t) {
        for (auto u = v->begin(); u != v->end(); ++u) {
            if (way[t] != 2 * INF && way[u->first] > way[t] + u->second) {
                cycled.push_back(u->first);
            }
        }
    }
    color = std::vector<int>(adj.size());
    while(!cycled.empty()){
        if(color[*cycled.rbegin()] == 0)
            dfs(*cycled.rbegin());
        cycled.pop_back();
    }
}


int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    int n, m, v, u, s;
    long long w;
    std::cin >> n;
    Graph graph(n);
    std::cin >> m >> s; --s;
    for(int i = 0; i < m; ++i){
        std::cin >> u >> v >> w; --u; --v;
        graph.addEdge(u, v, w);
    }
    graph.dfs(s);
    for(int i = 0; i < n; ++i){
        if(graph.color[i] == 0)
            graph.way[i] = 2 * INF;
    }
    graph.fordBelman(s);
    for(int i = 0; i < n; ++i){
        if(graph.way[i] == 2 * INF)
            std::cout << "*" << "\n";
        else{
            if(graph.color[i] == 2)
                std::cout << "-" << "\n";
            else
                std::cout << graph.way[i] << "\n";
        }
    }
    return 0;
}
