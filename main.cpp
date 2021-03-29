#include <iostream>
#include <vector>

const long long MOD = 999999937;

long long mod(long long a){
    return (MOD + a % MOD) % MOD;
}

class Matrix{
private:
    std::vector<std::vector<long long>> matrix;
public:
    Matrix(size_t n, size_t m);
    Matrix(std::initializer_list<std::initializer_list<long long>> init);

    Matrix operator*(Matrix m1);

    long long getSum();
    void print();
};

long long Matrix::getSum() {
    long long ans = 0;
    for(size_t i = 0; i < matrix.size(); ++i){
        for(size_t j = 0; j < matrix[0].size(); ++j){
            ans = mod(ans + matrix[i][j]);
        }
    }
    return ans;
}

Matrix::Matrix(size_t n, size_t m) {
    matrix = std::vector<std::vector<long long>>(n);
    for(size_t i = 0; i < n; ++i)
        matrix[i] = std::vector<long long>(m, 0);
}

Matrix::Matrix(std::initializer_list<std::initializer_list<long long>> init) {
    matrix = std::vector<std::vector<long long>>(init.size());
    int i = 0, j;
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
    Matrix res(n, m);
    for(size_t i = 0; i < n; ++i){
        for(size_t j = 0; j < m; ++j)
            for(size_t k = 0; k < matrix[0].size(); ++k)
                res.matrix[i][j] = mod(res.matrix[i][j] + mod(matrix[i][k] * m1.matrix[k][j]));
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

Matrix pow(Matrix A, long long n){
    if(n == 1)
        return A;
    if(n % 2 == 0) {
        Matrix m = pow(A, n / 2);
        return m * m;
    }
    return pow(A, n - 1) * A;
}

int main() {
    Matrix A({{2, 2, 2, 2}, {1, 1, 1, 1}, {1, 0, 1, 0}, {1, 0, 1, 0}});
    Matrix base({{2}, {1}, {1}, {1}});
    long long n;
    std::cin >> n;
    while(n != 0){
        Matrix ans = (n == 1? base: pow(A, n - 1) * base);
        std::cout << ans.getSum() << "\n";
        std::cin >> n;
    }
    return 0;
}
