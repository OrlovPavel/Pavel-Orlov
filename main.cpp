#include <iostream>
#include <vector>

const long long P = 4398042316799;
const long long a = 15432898;
const long long b = 1213467;
const long long m = 1000003;

struct Chain{
    long long value = 0;
    std::string name;
    Chain* next = nullptr;
    ~Chain(){
        if(!next)
            delete next;
    }
};

long long mod(long long x){
    return (P + x % P) % P;
}

long long hash(std::string x){
    long long value = 0;
    for(int i = 0; i < x.size(); ++i){
        value = mod(31 * value + (x[i] - 'a' + 1));
    }
    return (mod(a * value + b)) % m;
};

bool isEqual(std::string& s1, std::string& s2){
    if(s1.size() != s2.size())
        return false;
    for(int i = 0; i < s1.size(); ++i) {
        if (s1[i] != s2[i])
            return false;
    }
    return true;
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    std::vector<Chain*> chains(m, nullptr);
    std::string name; long long value; long long i;
    while(std::cin >> name){
        std::cin >> value;
        i = hash(name);
        bool name_in_Chain = false;
        Chain* c = chains[i];
        while(c != nullptr){
            if(isEqual(name, c->name)){
                name_in_Chain = true;
                c->value += value;
                break;
            }
            else{
                c = c->next;
            }
        }
        if(!name_in_Chain){
            c = new Chain;
            c->value += value;
            c->name = name;
            c->next = chains[i];
            chains[i] = c;
        }
        std::cout << c->value << "\n";
    }
    for(int j = 0; j < m; ++j)
        delete chains[i];
    return 0;
}