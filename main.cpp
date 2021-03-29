#include <iostream>

int main() {
    int n, m;
    std::cin >> n;
    int* a = new int[n];
    for(int i = 0; i < n; ++i)
        std::cin >> a[i];
    std::cin >> m;
    int* b = new int[m];
    for(int i = 0; i < m; ++i)
        std::cin >> b[i];
    int** dp = new int*[n];
    for(int i = 0; i < n; ++i){
        dp[i] = new int[m];
        for(int j = 0; j < m; ++j){
            if(j == 0 && b[0] == a[i])
                dp[i][j] = 1;
            else
                if(i == 0 && a[0] == b[j])
                    dp[i][j] = 1;
                else
                    dp[i][j] = 0;
        }
    }
    for(int i = 1; i < n; ++i)
        for(int j = 1; j < m; ++j){
            if(a[i] == b[j])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }
    std::cout << dp[n - 1][m - 1];
    return 0;
}