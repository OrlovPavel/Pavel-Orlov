#include <iostream>
#include <vector>

const int INF = 2147483647;

int main() { // вся функциональность в main
    int n;
    std::cin >> n;
    int* a = new int[n]; // выделенная память не освобождается
    int* dp = new int[n + 1];
    int* p = new int[n + 1];
    int* prev = new int[n];
    dp[0] = INF; p[0] = INF;
    for(int i = 0; i < n; ++i){
        dp[i + 1] = -INF;
        std::cin >> a[i];
        prev[i] = INF;
        p[i + 1] = INF;
    }
    for(int i = 0; i < n; ++i){
        int l = 0, r = n + 1;
        while(l < r - 1){
            int m = (l + r) / 2;
            if(dp[m] < a[i])
                r = m;
            else
                l = m;
        }
        if(l != n){
            dp[r] = a[i];
            p[r] = i;
            prev[i] = p[l];
        }
        //std::cout << r << " " << a[p[r]] <<"gh\n";
    }
    int ans = n;
    for(int i = n; dp[i] == -INF; --i){
        ans = i - 1;
    }
    std::cout << ans << "\n";
    std::vector<int> index;
    ans = p[ans];
    while(ans != INF){
        index.push_back(ans);
        ans = prev[ans];
    }
    for(auto it = index.rbegin(); it != index.rend(); ++it){
        std::cout << *it + 1 << " ";
    }
    return 0;
}
