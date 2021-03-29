#include <iostream>
#include <vector>


class Matrix{
private:
    std::vector<std::vector<long long>> matrix;
public:
    Matrix(size_t n, size_t m, long long val);
    Matrix(std::initializer_list<std::initializer_list<long long>> init);

    Matrix operator*(Matrix m1);
    std::vector<long long>& operator[](const size_t& i);

    long long getSum();
    void print();
};

long long Matrix::getSum() {
    long long ans = 0;
    for(size_t i = 0; i < matrix.size(); ++i){
        for(size_t j = 0; j < matrix[0].size(); ++j){
            ans = ans + matrix[i][j];
        }
    }
    return ans;
}

Matrix::Matrix(size_t n, size_t m, long long val) {
    matrix = std::vector<std::vector<long long>>(n);
    for(size_t i = 0; i < n; ++i)
        matrix[i] = std::vector<long long>(m, val);
}

Matrix::Matrix(std::initializer_list<std::initializer_list<long long>> init) {
    matrix = std::vector<std::vector<long long>>(init.size());
    long long i = 0, j;
    for(auto it1 = init.begin(); it1 != init.end(); ++it1){
        matrix[i] = std::vector<long long>(it1->size());
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
    Matrix res(n, m, 0);
    for(size_t i = 0; i < n; ++i){
        for(size_t j = 0; j < m; ++j)
            for(size_t k = 0; k < matrix[0].size(); ++k)
                res.matrix[i][j] += matrix[i][k] * m1.matrix[k][j];
    }
    return res;
}

std::vector<long long> &Matrix::operator[](const size_t& i) {
    return matrix[i];
}

Matrix pow(Matrix A, long long n){
    if(n == 1)
        return A;
    if(n % 2 == 0) {
        Matrix m = pow(A, n / 2);
        return m * m;
    }
    return pow(A, n - 1) * A;
}

void Matrix::print() {
    for(size_t i = 0; i < matrix.size(); ++i){
        for(size_t j = 0; j < matrix[i].size(); ++j)
            std::cout << matrix[i][j] << " ";
        std::cout << "\n";
    }
}


bool isTransit(long long i, long long j, int n){
    long long count = 0;
    int col = -1;
    for(int k = 0; k < n; ++k){
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
    long long n, m;
    std::cin >> n >> m;
    if(n > m)
        std::swap(n, m);
    Matrix dp(1<<n, 1<<n, 0);
    for(long long i = 0; i < 1<<n; ++i){
        for(long long j = 0; j < 1<<n; ++j)
            dp[i][j] = isTransit(i, j, n);
    }
    //dp.print();
    Matrix base(1<<n, 1, 1);
    if(m > 1)
        std::cout << (pow(dp, m - 1) * base).getSum();
    else
        std::cout << base.getSum();
    return 0;
}
