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


int main() { // нуждо было делать с использованием хеш балицы. Зачем остались функции для создания хеш таблицы, если они не используются?
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);


    long long w, h, n; long long x, y;
    std::cin >> w >> h >> n;
    if(w * h > 5 * n) {
        std::cout << "No";
        return 0;
    }
    bool** holst = new bool*[w];
    for(int i = 0; i < w; ++i){
        holst[i] = new bool[h];
        for(int j = 0; j < h; ++j)
            holst[i][j] = false;
    }
    for(int i = 0; i < n; ++i){
        std::cin >> x >> y;
        --x; --y;
        if(x >= 0 && x < w && y >= 0 && y < h)
            holst[x][y] = true;
        if(x - 1 >= 0 && x - 1 < w && y >= 0 && y < h)
            holst[x - 1][y] = true;
        if(x + 1 >= 0 && x + 1 < w && y >= 0 && y < h)
            holst[x + 1][y] = true;
        if(x >= 0 && x < w && y - 1 >= 0 && y - 1 < h)
            holst[x][y - 1] = true;
        if(x >= 0 && x < w && y + 1 >= 0 && y + 1 < h)
            holst[x][y + 1] = true;
    }
    for(int i = 0; i < w; ++i){
        for(int j = 0; j < h; ++j)
            if(!holst[i][j]){
                std::cout << "No";
                return 0;
            }
    }
    std::cout << "Yes";
    return 0;
}
