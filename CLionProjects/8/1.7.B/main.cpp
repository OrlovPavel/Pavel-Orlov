#include <iostream>
#include <vector>

class Graph{
private:
    std::vector<std::vector<int>> adj;
    std::vector<bool> used;
    std::vector<int> time_in;
    std::vector<int> time_out;
    int time = 0;
    void true_dfs(int v);
public:
    explicit Graph(size_t n);

    int getTimeOut(int v);
    int getTimeIn(int v);
    void addEdge(int v, int u);
    void dfs(int v);
};

Graph::Graph(size_t n): adj(std::vector<std::vector<int>>(n, std::vector<int>())), used(std::vector<bool>(n)), time_in(std::vector<int>(n)), time_out(std::vector<int>(n)){
}

void Graph::dfs(int v) {
    time = 0;
    for(size_t i = 0; i < used.size(); ++i) {
        used[i] = false;
        time_out[i] = 0;
        time_in[i] = 0;
    }
    true_dfs(v);
}

void Graph::true_dfs(int v) {
    used[v] = true;
    ++time;
    time_in[v] = time;
    for(size_t i = 0; i < adj[v].size(); ++i){
        if(!used[adj[v][i]])
            true_dfs(adj[v][i]);
    }
    ++time;
    time_out[v] = time;
}

void Graph::addEdge(int v, int u) {
    adj[v].push_back(u);
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
    int n, m, a, b;
    std::cin >> n;
    Graph graph(n);
    int root = 0, v = 0;
    for(int i = 0; i < n; ++i){
        std::cin >> v;
        if(v != 0)
            graph.addEdge(v - 1, i);
        else
            root = i;
    }
    graph.dfs(root);
    std::cin >> m;
    for(int i = 0; i < m; ++i){
        std::cin >> a >> b; --a; --b;
        if(graph.getTimeIn(b) > graph.getTimeIn(a) && graph.getTimeOut(a) > graph.getTimeOut(b))
            std::cout << '1' << "\n";
        else
            std::cout << '0' << "\n";
    }
    return 0;
}