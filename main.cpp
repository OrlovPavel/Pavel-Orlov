#include <iostream>
#include <vector>
#include <string>

const char FIRST = 96;
const char ALPHABET = 27;

int mod(int a, int b) {
    return (b + a % b) % b;
}

class SuffixArray {
    std::string word;
    std::vector<int> permut;

    void fillSuf(std::vector<int>& count, std::vector<int>& classes, size_t& cl_count) {
        for(char c : word) {
            ++count[c - FIRST];
        }
        for(char c = 1; c < ALPHABET; ++c) {
            count[c] += count[c - 1];
        }
        for(size_t i = 0; i < word.size(); ++i) {
            permut[--count[word[i] - FIRST]] = i;
        }
        classes[permut[0]] = 0;
        cl_count = 1;
        for(size_t i = 1; i < word.size(); ++i) {
            if(word[permut[i]] != word[permut[i - 1]]) {
                ++cl_count;
            }
            classes[permut[i]] = cl_count - 1;
        }
    }

public:
    SuffixArray(const std::string& s) : word(s + char(96)), permut(std::vector<int>(word.size())){
        std::vector<int> classes(500000, 0);
        std::vector<int> new_permut(500000, 0);
        std::vector<int> new_class(500000, 0);
        std::vector<int> count(500000, 0);
        size_t cl_count = 1;
        fillSuf(count, classes, cl_count);
        for(size_t k = 0; (1ull << k) < word.size(); ++k) {
            for(size_t i = 0; i < word.size(); ++i) {
                new_permut[i] = mod(permut[i] - int(1ull << k), word.size());
            }
            for(size_t i = 0; i < cl_count; ++i) {
                count[i] = 0;
            }
            for(size_t i = 0; i < word.size(); ++i) {
                ++count[classes[new_permut[i]]];
            }
            for(size_t i = 1; i < cl_count; ++i) {
                count[i] += count[i - 1];
            }
            for(int i = word.size() - 1; i >= 0; --i) {
                permut[--count[classes[new_permut[i]]]] = new_permut[i];
            }
            new_class[permut[0]] = 0;
            cl_count = 1;
            for(size_t i = 1; i < word.size(); ++i) {
                int first = permut[i];
                int second = mod(first + int(1ull << k), word.size());
                int prev_first = permut[i - 1];
                int prev_second = mod(prev_first + int(1ull << k), word.size());
                if(classes[first] != classes[prev_first] || classes[second] != classes[prev_second]) {
                    ++cl_count;
                }
                new_class[permut[i]] = cl_count - 1;
            }
            classes = new_class;
        }
    }

    int& operator[](size_t i) {
        return permut[i];
    }
};


int main() {
    std::string s;
    std::cin >> s;
    SuffixArray arr(s);
    for(size_t i = 1; i <= s.size(); ++i) {
        std::cout << arr[i] + 1 << " ";
    }
    return 0;
}
