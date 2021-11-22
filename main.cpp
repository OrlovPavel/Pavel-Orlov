#include <iostream>
#include <vector>

const size_t ALPHABET = 26;
const char FIRST = 'a';

char get_symbol(char c) {
    if(c >= 'a' && c <= 'z') {
        return c;
    } else {
        return char(c + 32);
    }
}

class SufAutomaton {
    struct Node{
        std::vector<int> to = std::vector<int>(ALPHABET, -1);
        bool terminate = false;
        int len = 0;
        int link = -1;
        Node() : to(std::vector<int>(ALPHABET, -1)), terminate(false), len(0), link(-1) {}
        Node(bool terminate, int len) : to(std::vector<int>(ALPHABET, -1)), terminate(terminate), len(len), link(-1) {}
    };
    std::vector<Node> graph;
    int last = 0;
public:
    SufAutomaton() : graph(std::vector<Node>(1)), last(0) {}

    explicit SufAutomaton(const std::string& s) : graph(std::vector<Node>(1)), last(0) {
        for(char c : s) {
            add(get_symbol(c));
        }
    }

    void add(const std::string& s) {
        for(char c : s) {
            add(get_symbol(c));
        }
    }

    void add(char symbol) {
        char c = symbol - FIRST;
        int p = last;
        graph.emplace_back(false, graph[last].len + 1);
        int cur = graph.size() - 1;
        while(p != -1 && graph[p].to[c] == -1) {
            graph[p].to[c] = cur;
            p = graph[p].link;
        }
        if(p == -1) {
            graph[cur].link = 0;
            last = cur;
            return;
        }
        int q = graph[p].to[c];
        if(graph[q].len == graph[p].len + 1) {
            graph[cur].link = q;
            last = cur;
            return;
        }
        graph.emplace_back(true, graph[p].len + 1);
        int clone = graph.size() - 1;
        while(p != -1 && graph[p].to[c] == q) {
            graph[p].to[c] = clone;
            p = graph[p].link;
        }
        graph[clone].to = graph[q].to;
        graph[clone].link = graph[q].link;
        graph[q].link = clone;
        graph[cur].link = clone;
        last = cur;
    }

    bool isWord(const std::string& word) {
        int v = 0;
        int i = 0;
        for(; i < word.size(); ++i) {
            char c = get_symbol(word[i]) - FIRST;
            v = graph[v].to[c];
            if(v == -1)
                break;
        }
        if(v == -1) {
            return false;
        }
        return true;
    }

};

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    std::string s;
    char c;
    SufAutomaton aut;
    while(std::cin >> c) {
        std::cin >> s;
        if(c == '?') {
            std::cout << (aut.isWord(s) ? "YES" : "NO") << "\n";
        }
        if(c == 'A') {
            aut.add(s);
        }
    }
    return 0;
}