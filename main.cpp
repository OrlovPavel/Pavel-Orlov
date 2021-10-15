#include <iostream>
#include <string>
#include <vector>

std::vector<int> z_function(std::string &s){
    std::vector<int> z(s.size(), 0);
    for(int i = 1, l = 0, r = 0; i < s.size(); ++i){
        if(i <= r)
            z[i] = std::min(z[i - l], r - i + 1);
        while(i + z[i] < s.size() && s[i + z[i]] == s[z[i]])
            ++z[i];
        if(i + z[i] > r + 1){
            l = i;
            r = i + z[i] - 1;
        }
    }
    return z;
}

int main() {
    std::string s;
    std::cin >> s;
    std::vector<int> z = z_function(s);
    for(int i = 0; i < s.size(); ++i)
        std::cout << z[i] << " ";
    return 0;
}
