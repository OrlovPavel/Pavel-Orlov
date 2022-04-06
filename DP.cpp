#include <iostream>

void insert(int& n, int& m, int* a, int* b){
    std::cin >> n;
    for(int i = 0; i < n; ++i)
        std::cin >> a[i];
    std::cin >> m;
    for(int i = 0; i < m; ++i)
        std::cin >> b[i];
}

void fill_dp(int& n, int& m, int* a, int* b, int** dp){
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
    for(int i = 1; i < n; ++i) {
        for (int j = 1; j < m; ++j) {
            if (a[i] == b[j])
                dp[i][j] = dp[i - 1][j - 1] + 1;
            else
                dp[i][j] = std::max(dp[i - 1][j], dp[i][j - 1]);
        }
    }
}

void free_memory(int* a, int* b, int& n, int** dp){
    delete[] a;
    delete[] b;
    for(int i = 0; i < n; ++i)
        delete[] dp[i];
    delete[] dp;
}

int main() {
    int n, m;
    int* a = new int[1000];
    int* b = new int[1000];
    insert(n, m, a, b);
    int** dp = new int*[n];
    fill_dp(n, m, a, b, dp);
    std::cout << dp[n - 1][m - 1];
    free_memory(a, b, n, dp);
    return 0;
}
