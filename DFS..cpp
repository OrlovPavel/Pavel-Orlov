#include <iostream>
#include <vector>

class Graph{
public:
    std::vector<std::vector<int>> adj;
    std::vector<int> color;
    std::vector<int> time_in;
    std::vector<int> time_out;
    std::vector<int> vertex_way;
    std::vector<int> cycle;
    int time = 0;
    void dfs(int v);
    explicit Graph(size_t n);

    int getTimeOut(int v);
    int getTimeIn(int v);
    void addEdge(int v, int u);
    void reset();
};

Graph::Graph(size_t n): adj(std::vector<std::vector<int>>(n, std::vector<int>())), color(std::vector<int>(n)), time_in(std::vector<int>(n)), time_out(std::vector<int>(n)){
}

void Graph::reset() {
    time = 0;
    for(size_t i = 0; i < color.size(); ++i) {
        color[i] = 0;
        time_out[i] = 0;
        time_in[i] = 0;
    }
}

void Graph::dfs(int v) {
    color[v] = 1;
    ++time;
    time_in[v] = time;
    vertex_way.push_back(v);
    for(size_t i = 0; i < adj[v].size(); ++i){
        if(color[adj[v][i]] == 0)
            dfs(adj[v][i]);
        else if(color[adj[v][i]] == 1){
            int j = vertex_way.size() - 1;
            cycle.push_back(adj[v][i]);
            while(vertex_way[j] != adj[v][i]) {
                cycle.push_back(vertex_way[j]);
                --j;
            }
        }
        if(!cycle.empty())
            break;
    }
    vertex_way.pop_back();
    ++time;
    time_out[v] = time;
    color[v] = 2;
}

void Graph::addEdge(int u, int v) {
    adj[u].push_back(v);
}

int Graph::getTimeOut(int v) {
    return time_out[v];
}

int Graph::getTimeIn(int v) {
    return time_in[v];
}



int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    int n, m, v, u;
    std::cin >> n;
    Graph graph(n);
    std::cin >> m;
    for(int i = 0; i < m; ++i){
        std::cin >> u >> v; --u; --v;
        graph.addEdge(u, v);
    }
    for(int i = 0; i < n; ++i){
        if(graph.color[i] == 0)
            graph.dfs(i);
        if(!graph.cycle.empty())
            break;
    }
    if(!graph.cycle.empty()){
        std::cout << "YES" << "\n";
        for(auto it = graph.cycle.rbegin(); it != graph.cycle.rend(); ++it)
            std::cout << *it + 1 << " ";
    }
    else{
        std::cout << "NO";
    }
    return 0;
}
