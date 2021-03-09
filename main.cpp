#include <iostream>
#include <vector>

const long long P = 4398042316799;
const long long a = 15432898;
const long long b = 1213467;
const long long m = 1000003;

struct Chain{
    std::string name;
    std::string value;
    Chain* next = nullptr;
    ~Chain(){
        if(next){
            delete next;
        }
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

class MyArray{
private:
    std::vector<Chain*> chains = std::vector<Chain*>(m, nullptr);
public:
    void insert(std::string& x, std::string& y){
        int i = hash(x);
        Chain* c = chains[i];
        bool f = false;
        while(c != nullptr){
            if(isEqual(x, c->name)){
                f = true;
                c->value = y;
                break;
            }
            else{
                c = c->next;
            }
        }
        if(!f){
            c = new Chain;
            c->value = y;
            c->name = x;
            c->next = chains[i];
            chains[i] = c;
        }
    }

    void erase(std::string& x){
        int i = hash(x);
        Chain* c = chains[i];
        if(!c)
            return;
        if(isEqual(x, c->name)){
            if(c->next){
                Chain* copy = c->next;
                *c = *c->next;
                copy->next = nullptr;
                delete copy;
                return;
            }
            else{
                delete c;
                chains[i] = nullptr;
                return;
            }
        }
        while(c != nullptr && c ->next != nullptr){
            if(isEqual(x, c->next->name)){
                Chain* d = c->next;
                if(c->next->next)
                    c->next = c->next->next;
                else
                    c->next = nullptr;
                delete d;
                return;
            }
            else{
                c = c->next;
            }
        }
    }

    void find(std::string x){
        int i = hash(x);
        Chain* c = chains[i];
        while(c != nullptr){
            if(isEqual(x, c->name)){
                std::cout << c->value << "\n";
                return;
            }
            else{
                c = c->next;
            }
        }
        std::cout << "none" << "\n";
    }
};

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    freopen("map.in", "r", stdin);
    freopen("map.out", "w", stdout);

    std::string q;
    std::string name;
    std::string value;
    MyArray arr;
    while (std::cin >> q){
        std::cin >> name;
        if(q == "put"){
            std::cin >> value;
            arr.insert(name, value);
        }
        if(q == "delete"){
            arr.erase(name);
        }
        if(q == "get"){
            arr.find(name);
        }
    }
    return 0;
}