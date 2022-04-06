#include <iostream>
#include <unordered_map>
#include <vector>
#include <queue>


int n;

long long pow(long long a, int x){
    if(x == 0)
        return 1;
    if(x % 2 == 1)
        return pow(a, x - 1) * a;
    long long t = pow(a, x / 2);
    return t * t;
}

long long getNum(std::vector<int>& p){
    long long res = 0;
    for(int i = 0; i < p.size(); ++i)
        res += p[i] * pow(10, i);
    return res;
}

void fillVector(long long num, std::vector<int>& p){
    while(num > 0){
        p.push_back(num % 10);
        num /= 10;
    }
    while(p.size() < n)
        p.push_back(0);
}

bool fillQueue(int used, std::queue<long long>& q, std::unordered_map<long long, int>& isUsed){
    int f = q.size();
    for(int g = 0; g < f; ++g) {
        long long v = q.front(); q.pop();
        std::vector<int> p;
        fillVector(v, p);
        std::vector<int> t = p; //p->t
        for (int k = 2; k <= n; ++k) {
            for (int i = 0; i <= n - k; ++i) {
                t = p;
                for (int j = 0; j < k / 2; ++j) {
                    std::swap(t[i + j], t[i + k - j - 1]);
                }
                long long num = getNum(t);
                if (isUsed.find(num) == isUsed.end()) {
                    q.push(num);
                    isUsed[num] = used;
                } else {
                    if (isUsed[num] != used)
                        return true;
                }
            }
        }
    }
    return false;
}

long long double_sideBFS(long long s, long long t){
    std::unordered_map<long long, int> isUsed;
    isUsed[s] = 1;
    isUsed[t] = 2;
    std::queue<long long> first;
    std::queue<long long> second;
    first.push(s);
    second.push(t);
    for(int i = 0; i < 4000000; ++i){
        if(i % 2 == 0){
            if(fillQueue(1, first, isUsed))
                return i + 1;
        }
        else{
            if(fillQueue(2, second, isUsed))
                return i + 1;
        }
    }
    return 0;
}

int main() {
    int a;
    std::cin >> n;
    long long s = 0, t = 0;
    for(int i = 0; i < n; ++i){
        std::cin >> a; --a;
        s += a * pow(10, i);
    }
    for(int i = 0; i < n; ++i){
        std::cin >> a; --a;
        t += a * pow(10, i);
    }
    if(s == t)
        std::cout << 0;
    else
        std::cout << double_sideBFS(s, t);
    return 0;
}
