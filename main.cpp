#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    int n;
    long long c;
    std::cin >> n;
    std::vector<long long> a(n / 2 + n % 2);
    std::vector<long long> b(n / 2);
    for(int i = 0; i < a.size(); ++i) {
        std::cin >> a[i];
    }
    for(int i = 0; i < b.size(); ++i) {
        std::cin >> b[i];
    }
    std::cin >> c;
    std::vector<long long> sum1(1<<a.size(), 0);
    int ans = 0;
    for(int i = 1; i < sum1.size(); ++i){
        int j = 0;
        int mask = i;
        while(mask > 0) {
            sum1[i] += a[j] * (mask % 2);
            mask /= 2;
            ++j;
        }
    }
    std::sort(sum1.begin(), sum1.end());
    int rangeB = 1<<b.size();
    for(int i = 0; i < rangeB; ++i){
        int j = 0;
        int mask = i;
        long long sum2 = 0;
        while(mask > 0) {
            sum2 += b[j] * (mask % 2);
            mask /= 2;
            ++j;
        }
        sum2 = c - sum2;
        if(sum2 < 0)
            continue;
        int l = 0, r = sum1.size();
        while(l + 1 < r){
            int m = (l + r) / 2;
            if(sum1[m] > sum2)
                r = m;
            else
                l = m;
        }
        ans += r;
    }
    std::cout << ans;
    return 0;
}
