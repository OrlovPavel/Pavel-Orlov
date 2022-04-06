#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

struct Edge{
    int from = 0;
    int to = 0;
    Edge(int from, int to) : from(from), to(to){}
};

class Graph{
public:
    std::vector<std::vector<int>> adj; // список инцидентных ребер для каждой вершины
    std::vector<Edge> edges;
    std::vector<int> used_edges; //количество использованных ребер из вершины
    std::vector<bool> isUsed;// про ребра
    std::vector<int> way;
    int time = 0;
    void euler(int v);
    explicit Graph(size_t n);

    void addEdge(int u, int v);
};

Graph::Graph(size_t n): adj(std::vector<std::vector<int>>(n, std::vector<int>())), used_edges(std::vector<int>(n, 0))
{
}


void Graph::euler(int v) {
    while(used_edges[v] < adj[v].size()){
        int edge = adj[v][used_edges[v]];
        ++used_edges[v];
        if(isUsed[edge]){
            continue;
        }
        else{
            isUsed[edge] = true;
            euler(edges[edge].to);
        }
    }
    way.push_back(v);
}

void Graph::addEdge(int u, int v) {
    adj[u].push_back(edges.size());
    edges.emplace_back(u, v);
    isUsed.push_back(false);
}




int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    int n, m, v, a;
    std::cin >> n >> a;
    Graph graph(n);
    for(int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            std::cin >> v;
            if (v == 0 && i != j)
                graph.addEdge(i, j);
        }
    }
    graph.euler(a - 1);
    int k = graph.way.size();
    for(int i = k - 1; i > 0; --i){
        std::cout << graph.way[i] + 1 << " " << graph.way[i - 1] + 1 << "\n";
    }
    return 0;
}
