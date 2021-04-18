#include <iostream>

void insert(int& n, int* w){
    for(int i = 1; i < n + 1; ++i){
        std::cin >> w[i];
    }
}

void fill_dp(int& s, int& n, int* w, int** dp){
    for(int i = 0; i < n + 1; ++i){
        dp[i] = new int[s + 1];
        dp[i][0] = 0;
    }
    for(int j = 0; j < s + 1; ++j)
        dp[0][j] = 0;

    for(int i = 1; i < n + 1; ++i){
        for(int j = 1; j < s + 1; ++j){
            dp[i][j] = dp[i - 1][j];
            if(w[i] <= j && dp[i - 1][j - w[i]] + w[i] > dp[i][j])
                dp[i][j] = dp[i - 1][j - w[i]] + w[i];
        }
    }
}
void free_memory(int* w, int& n, int** dp){
    delete[] w;
    for(int i = 0; i < n + 1; ++i){
        delete[] dp[i];
    }
    delete[] dp;
}

int main() {
    int s, n;
    std::cin >> s >> n;
    int* w = new int[n + 1];
    insert(n, w);
    int** dp = new int*[n + 1];
    fill_dp(s, n, w, dp);
    std::cout << dp[n][s];
    free_memory(w, n, dp);
    return 0;
}