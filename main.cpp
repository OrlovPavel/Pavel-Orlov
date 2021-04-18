#include <iostream>
#include <vector>

const int INF = 2147483647;

void insert(std::vector<int>& a, std::vector<int>& dp, std::vector<int>& p, std::vector<int>& prev){
    int n = a.size();
    for(int i = 0; i < n; ++i){
        dp[i + 1] = -INF;
        std::cin >> a[i];
    }
}

void fill_dp(std::vector<int>& a, std::vector<int>& dp, std::vector<int>& p, std::vector<int>& prev){
    int n = a.size();
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
}

void print_ans(std::vector<int>& dp, std::vector<int>& p, std::vector<int>& prev){
    int n = prev.size();
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
}

int main() {
    int n;
    std::cin >> n;
    std::vector<int> a(n);
    std::vector<int> dp(n + 1, INF);
    std::vector<int> p(n + 1, INF); // восстановление ответа
    std::vector<int> prev(n, INF);
    insert(a, dp, p, prev);
    fill_dp(a, dp, p, prev);
    print_ans(dp, p, prev);
    return 0;
}
