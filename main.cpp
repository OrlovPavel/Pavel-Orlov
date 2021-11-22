#include <iostream>
#include <vector>

const int MOD = 1000000007;
std::vector<int> fact;
std::vector<int> rev_fact;

int mod(int a, int mod = MOD) {
    return (mod + a % mod) % mod;
}

int pow(int x, int n) {
    int ans = 1;
    x = mod(x);
    n = mod(n, MOD - 1);
    while(n > 0) {
        if(n % 2 == 1) {
            ans = int((1ll * ans * x) % MOD);
        }
        x = int((1ll * x * x) % MOD);
        n /= 2;
    }
    return ans;
}

void fill_fact(int n) {
    fact = std::vector<int>(n);
    rev_fact = std::vector<int>(n);
    fact[0] = 1;
    rev_fact[0] = 1;
    for(int i = 1; i < fact.size(); ++i) {
        fact[i] = (1ll * fact[i - 1] * i) % MOD;
        rev_fact[i] = pow(fact[i], -1);
    }
}

int c(int n, int k) {
    if(k > n)
        return 0;
    return int((((1ll * fact[n] * rev_fact[n - k]) % MOD) * rev_fact[k]) % MOD);
}

int S(int n, int k) {
    if(n == 0) return 0;
    int ans = 0;
    for(int m = 0; m <= k; ++m) {
        int a = pow(m, n);
        a = mod(a * ((k + m) % 2 == 0 ? 1 : -1));
        a = int((1ll * a * c(k, m)) % MOD);
        ans = (ans + a) % MOD;
    }
    ans = int((1ll * ans * rev_fact[k]) % MOD);
    return ans;
}

int main() {
    int n, k;
    std::cin >> n >> k;
    long long sum = 0, a = 0;
    for(int i = 0; i < n; ++i) {
        std::cin >> a;
        sum = (sum + a) % MOD;
    }
    fill_fact(n > k ? (n + 1) : (k + 1));
    int ans = S(n, k);
    std::vector<int>& debug = fact;
    ans = int(ans + (1ll * (n - 1) * S(n - 1, k)) % MOD);
    std::cout << (1ll * ans * sum) % MOD;
    return 0;
}