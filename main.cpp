#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <string>

const int INF = 100000000;

class Graph{
public:
    std::vector<std::vector<int>> adj;
    std::vector<int> color;
    std::vector<bool> used;
    std::vector<int> match;
    explicit Graph(size_t n);
    void dfs(int v);

    void addEdge(int v, int u);
};


Graph::Graph(size_t n): adj(std::vector<std::vector<int>>(n)), color(std::vector<int>(n, 0)),
                        used(std::vector<bool>(n, false)), match(std::vector<int>(n)){
}


void Graph::addEdge(int v, int u) {
    adj[v].push_back(u);
}

void Graph::dfs(int v){
    color[v] = 1;
    for(auto it = adj[v].begin(); it != adj[v].end(); ++it){
        if(color[*it] == 0)
            dfs(*it);
    }
}

void print_ans(Graph& graph, int m, int n){
    int ans = 0;
    std::vector<int> left;
    std::vector<int> right;
    for(int i = 0; i < m; ++i){
        if(graph.color[i] == 0){
            ++ans;
            left.push_back(i);
        }
    }
    for(int i = m; i < n + m; ++i){
        if(graph.color[i] == 1){
            ++ans;
            right.push_back(i);
        }
    }
    std::cout << ans << "\n";
    std::cout << left.size() << " ";
    for(int i = 0; i < left.size(); ++i)
        std::cout << left[i] + 1 << " ";
    std::cout << "\n" << right.size() << " ";
    for(int i = 0; i < right.size(); ++i)
        std::cout << right[i] - m + 1 << " ";
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    int n, m, k, v;
    std::cin >> m >> n;
    std::vector<bool> isInverted(m);
    Graph graph(n + m);
    for(int i = 0; i < m; ++i){
        std::cin >> k;
        for(int j = 0; j < k; ++j) {
            std::cin >> v; --v;
            graph.addEdge(i, m + v);
        }
    }
    for(int i = 0; i < m; ++i){
        std::cin >> v;
        if(v != 0){
            isInverted[i] = true; --v;
            graph.addEdge(m + v, i);
        }
    }
    for(int i = 0; i < m; ++i){
        if(!isInverted[i]){
            graph.dfs(i);
        }
    }
    print_ans(graph, m, n);
    return 0;
}