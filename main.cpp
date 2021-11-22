#include <iostream>
#include <vector>
#include <string>

const int ABC = 26;
const char FIRST = 97;

class Bor{
    struct Node{
        std::vector<int> to = std::vector<int>(ABC, -1);
        int number = 0;
        int depth = 0;
    };
    std::vector<Node> trie = std::vector<Node>(1);
    int sz = 0;
public:
    void add(const std::string& s){
        int v = 0;
        for(auto it = s.begin(); it != s.end(); ++it){
            int c = *it - FIRST;
            if(trie[v].to[c] == -1){
                trie.emplace_back(Node());
                trie[trie.size() - 1].depth = trie[v].depth + 1;
                trie[v].to[c] = trie.size() - 1;
            }
            v = trie[v].to[c];
        }
        ++trie[v].number;
        ++sz;
    }

    void sort(std::vector<std::string>& sorted, std::string& word, int v = 0) {
        if(v != 0) {
            for(int i = 0; i < trie[v].number; ++i)
                sorted.push_back(word);
        }
        for(char i = 0; i < trie[v].to.size(); ++i) {
            char c = i + FIRST;
            int to = trie[v].to[i];
            if(to != -1) {
                word += c;
                sort(sorted, word, to);
                word.pop_back();
            }
        }
    }
};
int main() {
    std::string s;
    std::string word;
    Bor words;
    std::vector<std::string> points(1, "");
    int i = 0;
    bool f = false;
    std::cin >> s;
    while(s[i] == '.'){
        points[0] += '.';
        ++i;
    }
    for(; i != s.size(); ++i){
        if(s[i] == '.'){
            points[points.size() - 1] += '.';
            if(f) {
                words.add(word);
                f = false;
                word = "";
            }
        }
        else{
            if(!f) {
                points.emplace_back("");
                f = true;
            }
            word += s[i];
        }
    }
    if(f) {
        words.add(word);
    } else{
        points.emplace_back("");
    }
    std::cout << points[0];
    std::vector<std::string> sorted;
    word = "";
    words.sort(sorted, word);
    for(int i = 1, j = 0; j < sorted.size(); ++i, ++j){
        std::cout << sorted[j];
        std::cout << points[i];
    }
    return 0;
}
