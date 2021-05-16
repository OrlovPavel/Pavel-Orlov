#include <iostream>
#include <vector>
#include <sstream>

class BigInteger{
public:
    static const long long rank = 1000000000000000000;
    static const long long rank_length = 18;
    std::vector<long long> digits;
    bool sign = true;

    static long long mod(long long a){
        return (rank + a % rank) % rank;
    }
    void removeZeros(){
        for(auto it = digits.rbegin(); it != digits.rend(); ++it){
            if(*it == 0 && digits.size() > 1)
                digits.pop_back();
            else
                break;
        }
    };
public:

    void divide_by_2() {
        BigInteger res;
        res.digits = std::vector<long long>(digits.size(), 0);
        long long d = 0;
        for (int i = digits.size() - 1; i >= 0; --i) {
            d += digits[i];
            res.digits[i] = d / 2;
            d = digits[i] % 2;
            d *= rank;
        }
        res.removeZeros();
        *this = res;
    }

    friend std::istream& operator>>(std::istream&, BigInteger&);

    BigInteger() : sign(true){
        digits.push_back(0);
    }

    BigInteger(long long value){
        long long cpy = value;
        sign = value >= 0;
        cpy *= (sign? 1: -1);
        if(cpy == 0)
            digits.push_back(0);
        while(cpy > 0){
            digits.push_back(mod(cpy));
            cpy /= rank;
        }
    }

    BigInteger& operator=(BigInteger num){
        swap(num);
        return *this;
    }

    void swap(BigInteger& num){
        std::swap(digits, num.digits);
        std::swap(sign, num.sign);
    }

    BigInteger& operator--(){
        for(size_t i = 0; i < digits.size(); ++i){
            if(digits[i] == 0){
                digits[i] = rank - 1;
            }
            else{
                digits[i] -= 1;
                break;
            }
        }
        removeZeros();
        return *this;
    }

    void print(){
        for(int i = digits.size() - 1; i >= 0; --i) {
            std::cout << digits[i] << "|";
        }
        std::cout << "\n";
    }
};

std::istream& operator>>(std::istream& in, BigInteger& num){
    std::string s;
    num.digits.clear();
    num.sign = true;
    in >> s;
    if(s.size() == 0){
        num.digits.push_back(0);
        return in;
    }
    size_t end = 1, len;
    if(s.at(0) == '-') {
        num.sign = false;
        end++;
    }
    for(size_t i = s.size(); i >= end; i -= len){
        len = (i >= BigInteger::rank_length + end)? BigInteger::rank_length : i - end + 1;
        num.digits.push_back(stoll(s.substr(i - len, len)));
    }
    /*std::cout << num << "\n";
    for(long long i = 0; i < num.digits.size() / 2; ++i){
        std::swap(num.digits.at(i), num.digits.at(num.digits.size() - i - 1));
    }
    std::cout << num << "\n";*/
    return in;
}



size_t MOD(size_t a, size_t mod){
    return (mod + a % mod) % mod;
}

class Matrix{
private:
    std::vector<std::vector<size_t>> matrix;
    size_t mod;
public:
    Matrix(size_t n, size_t m, size_t val, size_t mod);
    Matrix(std::initializer_list<std::initializer_list<size_t>> init);

    Matrix operator*(Matrix m1);
    std::vector<size_t>& operator[](const size_t& i);

    size_t getSum();
    void print();
};

size_t Matrix::getSum() {
    size_t ans = 0;
    for(size_t i = 0; i < matrix.size(); ++i){
        for(size_t j = 0; j < matrix[0].size(); ++j){
            ans = MOD(ans + matrix[i][j], mod);
        }
    }
    return ans;
}

Matrix::Matrix(size_t n, size_t m, size_t val, size_t mod) : mod(mod){
    matrix = std::vector<std::vector<size_t>>(n);
    for(size_t i = 0; i < n; ++i)
        matrix[i] = std::vector<size_t>(m, val);
}

Matrix::Matrix(std::initializer_list<std::initializer_list<size_t>> init) {
    matrix = std::vector<std::vector<size_t>>(init.size());
    size_t i = 0, j;
    for(auto it1 = init.begin(); it1 != init.end(); ++it1){
        matrix[i] = std::vector<size_t>(it1->size());
        j = 0;
        for(auto it2 = it1->begin(); it2 != it1->end(); ++it2) {
            matrix[i][j] = *it2;
            ++j;
        }
        ++i;
    }
}

Matrix Matrix::operator*(Matrix m1){
    size_t n = matrix.size();
    size_t m = m1.matrix[0].size();
    Matrix res(n, m, 0, mod);
    for(size_t i = 0; i < n; ++i){
        for(size_t j = 0; j < m; ++j)
            for(size_t k = 0; k < matrix[0].size(); ++k)
                res.matrix[i][j] = MOD(res.matrix[i][j] + MOD(matrix[i][k] * m1.matrix[k][j], mod), mod);
    }
    return res;
}

std::vector<size_t> &Matrix::operator[](const size_t& i) {
    return matrix[i];
}

Matrix pow(Matrix& A, BigInteger k, size_t mod){
    Matrix res(A[0].size(), A[0].size(), 0, mod);
    for(size_t i = 0; i < A[0].size(); ++i)
        res[i][i] = 1;
    while(k.digits[k.digits.size() - 1] != 0){
        if(k.digits[0] % 2 == 1)
            res = res * A;
        A = A * A;
        k.divide_by_2();
    }
    return res;
}

void Matrix::print() {
    for(size_t i = 0; i < matrix.size(); ++i){
        for(size_t j = 0; j < matrix[i].size(); ++j)
            std::cout << matrix[i][j] << " ";
        std::cout << "\n";
    }
}


bool isTransit(size_t i, size_t j, size_t n){
    size_t count = 0;
    size_t col = -1;
    for(size_t k = 0; k < n; ++k){
        if(i % 2 == j % 2){
            count++;
            if(count > 1 && i % 2 == col)
                return false;
            col = i % 2;
        }
        else {
            count = 0;
            col = -1;
        }
        i /= 2;
        j /= 2;
    }
    return true;
}

int main() {
    BigInteger m;
    size_t n, mod;
    std::cin >> m >> n >> mod;
    Matrix dp(1<<n, 1<<n, 0, mod);
    for(size_t i = 0; i < 1<<n; ++i){
        for(size_t j = 0; j < 1<<n; ++j)
            dp[i][j] = isTransit(i, j, n);
    }
    //dp.prsize_t();
    Matrix base(1<<n, 1, 1, mod);
    std::cout << (pow(dp, --m, mod) * base).getSum();
    return 0;
} 