#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

class Graph{
public:
    std::vector<std::vector<int>> adj;
    std::vector<int> color;
    std::vector<int> time_in;
    std::vector<int> ret;
    std::set<int> connection_vertex;
    int time = 0;
    void dfs(int v, int parent = -1);
    explicit Graph(size_t n);

    void addEdge(int u, int v);
};

Graph::Graph(size_t n): adj(std::vector<std::vector<int>>(n, std::vector<int>())), color(std::vector<int>(n)),
time_in(std::vector<int>(n, 1000000)), ret(std::vector<int>(n)){
}


void Graph::dfs(int v, int parent) {;
    color[v] = 1;
    ++time;
    time_in[v] = time;
    ret[v] = time;
    int childs = 0;
    for(auto it = adj[v].begin(); it != adj[v].end(); ++it){
        int to = *it;
        if(to == parent){
            continue;
        }
        if(color[to] != 0){
            ret[v] = std::min(ret[v], time_in[to]);
        }
        else{
            dfs(to, v);
            ++childs;
            if(parent!= -1 && ret[to] >= time_in[v]) {
                connection_vertex.insert(v);
            }
            else if(parent == -1 && childs > 1){
                connection_vertex.insert(v);
            }
            ret[v] = std::min(ret[to], ret[v]);
        }
    }
    color[v] = 2;
}

void Graph::addEdge(int u, int v) {
    adj[u].push_back(v);
}




int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    int n, m, v, u;
    std::cin >> n;
    Graph graph(n);
    std::cin >> m;
    std::vector<std::pair<int, int>> edges;
    for(int i = 0; i < m; ++i){
        std::cin >> u >> v; --u; --v;
        edges.emplace_back(u, v);
        graph.addEdge(u, v);
        graph.addEdge(v, u);
    }
    for(int i = 0; i < n; ++i){
        if(graph.color[i] == 0) {
            graph.dfs(i);
        }
    }
    std::cout << graph.connection_vertex.size() << "\n";
    //std::sort(graph.connection_vertex.begin(), graph.connection_vertex.end());
    for(auto it = graph.connection_vertex.begin(); it != graph.connection_vertex.end(); ++it)
        std::cout << *it + 1 << " ";
    return 0;
}
