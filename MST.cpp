#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

const int INF = 100000000;

class Graph{
public:
    std::vector<std::pair<int, std::pair<int, int>>> edg;
    int size = 0;
    std::vector<int> color;
    explicit Graph(size_t n, size_t m);

    int kruskal();

    void addEdge(int v, int u, int weight);
};

struct DisjointSet{
    std::vector<int> sets;
    std::vector<int> size;
    std::vector<int> prev;
    DisjointSet(int n) : sets(std::vector<int>(n)), size(std::vector<int>(n, 1)), prev(std::vector<int>(n)){
        for(int i = 0; i < n; ++i){
            sets[i] = i;
            prev[i] = i;
        }
    }

    int getSet(int k){
        int res = k;
        while(prev[res] != res){
            res = prev[res];
        }
        sets[k] = res;
        return res;
    }

    void union_sets(int a, int b){
        int set1 = getSet(a);
        int set2 = getSet(b);
        if(size[set1] < size[set2])
            std::swap(set1, set2);
        size[set1] += size[set2];
        prev[set2] = set1;
    }
};


Graph::Graph(size_t n, size_t m): size(n), edg(std::vector<std::pair<int, std::pair<int, int>>>(m)), color(std::vector<int>(n)){
}


void Graph::addEdge(int v, int u, int weight) {
    edg.emplace_back(weight, std::make_pair(u, v));
}

int Graph::kruskal() {
    DisjointSet set(size);
    int res = 0;
    std::sort(edg.begin(), edg.end());
    for(auto it = edg.begin(); it != edg.end(); ++it){
        int w = it->first, u = it->second.first, v = it->second.second;
        if(set.getSet(u) != set.getSet(v)){
            res += w;
            set.union_sets(u, v);
        }
    }
    return res;
}


int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    int n, m, v, u, w;
    std::cin >> n >> m;
    Graph graph(n, m);
    for(int i = 0; i < m; ++i){
        std::cin >> v >> u >> w; --v; --u;
        graph.addEdge(v, u, w);
    }
    std::cout << graph.kruskal();
    return 0;
}
