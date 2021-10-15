#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <queue>

const size_t ABC = 2;

class Bor{
    struct Node{
        std::vector<int> to = std::vector<int>(ABC, -1);
        std::vector<int> orders;
        int depth = 0;
        int link = -1;
    };
    std::vector<Node> trie = std::vector<Node>(1);
    std::vector<int> used = std::vector<int>(1);
    int sz = 0;
public:
    void add(const std::string& s){
        int v = 0;
        for(auto it = s.begin(); it != s.end(); ++it){
            int c = *it - 48;
            if(trie[v].to[c] == -1){
                trie.emplace_back(Node());
                used.push_back(0);
                trie[trie.size() - 1].depth = trie[v].depth + 1;
                trie[v].to[c] = trie.size() - 1;
            }
            v = trie[v].to[c];
        }
        trie[v].orders.push_back(sz++);
    }

    void Acho_Korasik(){
        std::queue<int> q;
        trie[0].link = 0;
        for(int c = 0; c < ABC; ++c){
            if(trie[0].to[c] == - 1){
                trie[0].to[c] = 0;
            }
        }
        q.push(0);
        while(!q.empty()){
            int v = q.front(); q.pop();
            for(int c = 0; c < ABC; ++c){
                int u = trie[v].to[c];
                if(trie[u].link != -1) {
                    continue;
                }
                trie[u].link = v == 0 ? 0 : trie[trie[v].link].to[c];
                for(int s = 0; s < ABC; ++s){
                    if(trie[u].to[s] == -1){
                        trie[u].to[s] = trie[trie[u].link].to[s];
                    }
                }
                q.push(u);
            }
        }
    }

    std::vector<std::set<int>> find(const std::string& text){
        int v = 0;
        std::vector<std::set<int>> ans(sz, std::set<int>());
        for(int i = 0; i < text.size(); ++i){
            int c = text[i] - 48;
            v = trie[v].to[c];
            int u = v;
            while(u != 0){
                if(!trie[u].orders.empty()) {
                    for(int j: trie[u].orders)
                        ans[j].insert(i - trie[u].depth + 2);
                }
                u = trie[u].link;
            }
        }
        return ans;
    }

    bool dfs(int v){
        used[v] = 1;
        bool res = false;
        for(int to: trie[v].to){
            int u = to;
            bool empty = true;
            while(u != 0){
                if(!trie[u].orders.empty()) {
                    empty = false;
                    break;
                }
                u = trie[u].link;
            }
            if(empty){
                if(used[to] == 1){
                    res = true;
                }
                if(used[to] == 0){
                    res = dfs(to);
                }
                if(res)
                    break;
            }
        }
        used[v] = 2;
        return res;
    }
};

int main() {
    int n;
    std::string word;
    std::cin >> n;
    Bor bor;
    for(int i = 0; i < n; ++i){
        std::cin >> word;
        bor.add(word);
    }
    bor.Acho_Korasik();
    bool ans = bor.dfs(0);
    if(ans){
        std::cout << "TAK";
    }
    else{
        std::cout << "NIE";
    }
    return 0;
}
