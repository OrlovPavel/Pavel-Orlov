#include <iostream>

int main() { // вся функциональность в main
    int s, n;
    std::cin >> s >> n;
    int* w = new int[n + 1]; // память не освобождается
    for(int i = 1; i < n + 1; ++i){
        std::cin >> w[i];
    }
    int** dp = new int*[n + 1];
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
    std::cout << dp[n][s];
    return 0;
}
