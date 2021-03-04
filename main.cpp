#include <iostream>
#include <vector>

const long long P = 4398042316799;
const long long a = 15432898;
const long long b = 1213467;
const long long m = 1000003;

struct Chain{
    long long key;
    long long value;
    Chain* next = nullptr;
};

long long mod(long long x){
    return (P + x % P) % P;
}

long long hash(long long x){
    return (mod(mod(a * x) + b)) % m;
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
    void insert(long long key, long long value){
        int i = hash(key);
        Chain* c = chains[i];
        bool f = false;
        while(c != nullptr){
            if(key == c->key){
                f = true;
                c->value = value;
                break;
            }
            else{
                c = c->next;
            }
        }
        if(!f){
            c = new Chain;
            c->value = value;
            c->key = key;
            c->next = chains[i];
            chains[i] = c;
        }
    }

    /*void erase(std::string& x){
        int i = hash(x);
        Chain* c = chains[i];
        if(!c)
            return;
        if(isEqual(x, c->name)){
            if(c->next){
                Chain* d = c->next;
                *c = *c->next;
                delete d;
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
    }*/

    long long find(long long key){
        int i = hash(key);
        Chain* c = chains[i];
        while(c != nullptr){
            if(key == c->key){
                return c->value;
            }
            else{
                c = c->next;
            }
        }
        return -1;
    }
};


int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);

    int n; int x, y;
    int i1, i2;
    std::cin >> n;
    MyArray arr;
    for(int i = 0; i < n; ++i){
        std::cin >> x >> y;
        i1 = arr.find(x);
        i2 = arr.find(y);
        if(i1 < 0)
            i1 = x - 1;
        if(i2 < 0)
            i2 = y - 1;
        std::cout << (i2 > i1 ? i2 - i1: i1 - i2) << "\n";
        arr.insert(x, i2);
        arr.insert(y, i1);
    }
    return 0;
}