#include <iostream>
#include <string>
#include <set>
#include <vector>

std::pair<std::multiset<std::string>, std::vector<std::string>> fill_words_points(const std::string& s){
    std::string word;
    std::multiset<std::string> words;
    std::vector<std::string> points(1, "");
    int i = 0;
    bool f = false;
    while(s[i] == '.'){
        points[0] += '.';
        ++i;
    }
    for(; i != s.size(); ++i){
        if(s[i] == '.'){
            points[points.size() - 1] += '.';
            if(f) {
                words.insert(word);
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
        words.insert(word);
    } else{
        points.emplace_back("");
    }
    return std::make_pair(words, points);
}

int main() {
    std::string s;
    std::cin >> s;
    auto pair = fill_words_points(s);
    std::cout << pair.second[0];
    int i = 1;
    for(auto it = pair.first.begin(); it != pair.first.end(); ++it, ++i){
        std::cout << *it;
        std::cout << pair.second[i];
    }
    return 0;
}
