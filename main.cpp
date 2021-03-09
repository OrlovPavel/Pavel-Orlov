#include <iostream>
#include <vector>
#include <unordered_set>
#include <iomanip>

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    int n, m; int x, y; int k = 0;
    std::cin >> n >> m;
    std::vector<std::vector<int> > vertex(n);
    std::vector<std::unordered_set<int> > svertex(n);  // Во всех заданиях из этого раздела нужно было реализовать свою хеш таблицу, нельзя использовать unordered_set
    for(int i = 0; i < m; ++i){
        std::cin >> x >> y; --x; --y;
        vertex[x].push_back(y);
        vertex[y].push_back(x);
        svertex[x].insert(y);
        svertex[y].insert(x);
    }
    for(int i = 0; i < n; ++i) {
        for (int j = 0; j < vertex[i].size(); ++j) {
            int g = vertex[i][j];
            if (g > i) {
                if (vertex[i].size() < vertex[g].size()) {
                    for (int t = 0; t < vertex[i].size(); ++t)
                        if (vertex[i][t] > i && vertex[i][t] < g && svertex[g].find(vertex[i][t]) != svertex[g].end())
                            k++;
                } else {
                    for (int t = 0; t < vertex[g].size(); ++t) {
                        if (vertex[g][t] > i && vertex[g][t] < g && svertex[i].find(vertex[g][t]) != svertex[i].end())
                            k++;
                    }
                }
            }
        }
    }
    double ans = k;
    ans /= 4;
    std::cout << std::setprecision(17) << ans;
    return 0;
}
