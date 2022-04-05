#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>

class SparseTable{
private:
    std::vector<std::vector<std::pair<int, int>>> table;
    std::vector<std::vector<std::pair<int, int>>> index;
    std::vector<int> log;
    std::vector<int> pows;
public:
    static bool comp(std::pair<int, int> a, std::pair<int, int> b){
        return a.first < b.first;
    }
    SparseTable(int* array, int n);
    int getAns(int l, int r);
    void print();
};

void SparseTable::print(){
    for(int i = 0; i < table.size(); ++i) {
        for (int j = 0; j < table[i].size(); ++j)
            std::cout << "(" << table[i][j].first << ", " << table[i][j].second << ") ";
        std::cout << "\n";
    }
}

SparseTable::SparseTable(int* array, int n){
    log.push_back(0); std::pair<int, int> p;
    for(int i = 1; i < n + 1; ++i) {
        log.push_back(log2(i));
    }
    for(int i = 0; i <= log[n]; ++i){
        pows.push_back(pow(2, i));
    };
    table.emplace_back(std::vector<std::pair<int, int>>(log[n] + 1));
    index.emplace_back(std::vector<std::pair<int, int>>(log[n] + 1));
    for(int i = 1; i < n + 1; ++i){
        table.emplace_back(std::vector<std::pair<int, int> >(log[n] + 1));
        index.emplace_back(std::vector<std::pair<int, int>>(log[n] + 1));
        //p.first = array[i]; p.second = array[i] - 1;
        table[i][0] = std::pair<int, int>(array[i], array[i] - 1);
        index[i][0] = std::pair<int, int>(array[i], -1);
    }
    for(int j = 1; j <= log[n]; ++j){
        for(int i = 1; i < n + 1; ++i) {
            if(i + pows[j - 1] <= n) {
                std::vector<std::pair<int, int>> arr;
                arr.push_back(std::pair<int, int>(table[i][j - 1].first, index[i][j - 1].first));
                if(table[i][j - 1].first <= table[i][j - 1].second)
                    arr.push_back(std::pair<int, int>(table[i][j - 1].second, index[i][j - 1].second));
                arr.push_back(std::pair<int, int>(table[i + pows[j - 1]][j - 1].first, index[i + pows[j - 1]][j - 1].first));
                if(table[i + pows[j - 1]][j - 1].first <= table[i + pows[j - 1]][j - 1].second)
                    arr.push_back(std::pair<int, int>(table[i + pows[j - 1]][j - 1].second, index[i + pows[j - 1]][j - 1].second));
                sort(arr.begin(), arr.end(), comp);
                table[i][j] = std::pair<int, int>(arr[0].first, arr[1].first);
                index[i][j] = std::pair<int, int>(arr[0].second, arr[1].second);
            }
            else{
                //p.first = a; p.second = b;
                table[i][j] = table[i][j - 1];
                index[i][j] = index[i][j - 1];
            }
        }
    }
}

int SparseTable::getAns(int l, int r) {
    int k = log[r - l + 1];
    int t = pows[k];
    if(table[l][k].first < table[r - t + 1][k].first){
        if(table[r - t + 1][k].first <= table[l][k].second)
            return table[r - t + 1][k].first;
        else
            return table[l][k].second;
    }
    if(table[l][k].first > table[r - t + 1][k].first){
        if(table[r - t + 1][k].second >= table[l][k].first)
            return table[l][k].first;
        else
            return table[r - t + 1][k].second;
    }
    if(index[l][k].first == index[r - t + 1][k].first){
        if(table[l][k].second < table[l][k].first)
            return table[r - t + 1][k].second;
        if(table[r - t + 1][k].second < table[r - t + 1][k].first)
            return table[l][k].second;
        if(table[r - t + 1][k].second >= table[l][k].second)
            return table[l][k].second;
        return table[r - t + 1][k].second;
    }
    return table[l][k].first;
}


int main() {
    int n, m;
    int l, r;
    std::cin >> n >> m;
    int* arr = new int[n + 1];
    for(int i = 1; i <= n; ++i)
        std::cin >> arr[i];
    SparseTable sparse(arr, n);
    //sparse.print();
    for(int i = 0; i < m; ++i){
        std::cin >> l >> r;
        std::cout << sparse.getAns(l, r) << "\n";
    }
    return 0;
}
