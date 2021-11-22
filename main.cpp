#include <iostream>
#include <vector>

std::vector<int> find_primes(int n) {
    std::vector<int> min_pr(n + 1);
    std::vector<int> primes;
    for(int i = 2; i < n + 1; ++i) {
        if(min_pr[i] == 0) {
            primes.push_back(i);
            min_pr[i] = i;
        }
        for(int j = 0; j < primes.size() && primes[j] <= min_pr[i] && i * primes[j] <= n; ++j) {
            min_pr[i * primes[j]] = primes[j];
        }
    }
    return min_pr;
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> min_pr = find_primes(n);
    long long ans = 0;
    for(int i = 2; i < n + 1; ++i) {
        if(i != min_pr[i]) {
            ans += (long long)min_pr[i];
        }
    }
    std::cout << ans;
    return 0;
}
