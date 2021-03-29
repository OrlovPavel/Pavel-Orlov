#include <iostream>

int main() {
    int n, m;
    std::cin >> n >> m;
    int* a = new int[n];
    int* b = new int[m];
    for(int i = 0; i < n; ++i){
        std::cin >> a[i];
    }
    for(int i = 0; i < m; ++i){
        std::cin >> b[i];
    }
    int** dp = new int*[n];
    for(int i = 0; i < n; ++i){
        dp[i] = new int[m];
    }
    for(int j = 0; j < m; ++j){
        if(a[0] == b[j])
            dp[0][j] = 1;
        else
            dp[0][j] = 0;
    }

    for(int i = 1; i < n; ++i){
        int maxL = 0; //длина максималньой подпоследовательности с концом < a[i]
        for(int j = 0; j < m; ++j) {
            dp[i][j] = dp[i - 1][j];
            if(a[i] == b[j] && maxL + 1 > dp[i][j])
                dp[i][j] = maxL + 1;
            if(b[j] < a[i] && dp[i][j] > maxL)
                maxL = dp[i][j];
        }
    }
    int ans = 0;
    for(int j = 0; j < m; ++j){
        if(dp[n - 1][j] > ans)
            ans = dp[n - 1][j];
    }
    std::cout << ans;
    return 0;
}
