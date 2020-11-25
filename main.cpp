#include <iostream>
#include <set>
#include <vector>
#include <map>
#include <string>

class Sets{
private:
    std::map<int, std::set<long long> > sets;
    std::map<long long, std::set<int>> elements;
public:
    void add(long long element, int pos){
        sets[pos].insert(element);
        elements[element].insert(pos);
    }
    void del(long long element, int pos){
        sets[pos].erase(element);
        elements[element].erase(pos);
    }
    void clear(int pos){
        if(sets[pos].empty())
            return;
        for(auto it = sets[pos].begin(); it != sets[pos].end(); ++it) {
            elements[*it].erase(pos);
        }
        sets.erase(pos);
    }
    void printElementsOf(int pos){
        if(sets.find(pos) == sets.end() || sets[pos].empty()) {
            std::cout << -1 << "\n";
        }
        else {
            for (auto it = sets[pos].begin(); it != sets[pos].end(); ++it) {
                std::cout << *it << " ";
            }
            std::cout << "\n";
        }
    }
    void printSetsOf(long long element){
        if(elements.find(element) == elements.end() || elements[element].empty()){
            std::cout << -1 << "\n";
        }
        else{
            for(auto it = elements[element].begin(); it != elements[element].end(); ++it){
                std::cout << *it << " ";
            }
            std::cout << "\n";
        }
    }
};

int main() {
    long long element;
    int m, k;
    std::string s;
    std::cin >> element >> m >> k;
    Sets sets;
    for(int i = 0; i < k; ++i){
        std::cin >> s;
        if(s[0] == 'A'){
            std::cin >> element >> m;
            sets.add(element, m);
        }
        if(s[0] == 'D'){
            std::cin >> element >> m;
            sets.del(element, m);
        }
        if(s[0] == 'C'){
            std::cin >> m;
            sets.clear(m);
        }
        if(s[s.size() - 1] == 'T'){
            std::cin >> m;
            sets.printElementsOf(m);
        }
        if(s[s.size() - 1] == 'F'){
            std::cin >> element;
            sets.printSetsOf(element);
        }
    }
    return 0;
}
