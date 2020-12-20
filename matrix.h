#ifndef MATRIX_RATIONAL_H
#define MATRIX_RATIONAL_H

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cassert>

class BigInteger{
private:
    static const int rank = 10000;
    std::vector<int> digits;
    bool sign = true;

    static int mod(int a){
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
    friend bool operator==(const BigInteger&, const BigInteger&);
    friend bool operator<(const BigInteger&, const BigInteger&);
    friend bool operator>=(const BigInteger&, const BigInteger&);
    friend std::istream& operator>>(std::istream&, BigInteger&);
    friend BigInteger operator*(const BigInteger&,const BigInteger&);
    friend BigInteger operator+(const BigInteger&,const BigInteger&);
    BigInteger() : sign(true){
        digits.push_back(0);
    }
    BigInteger(const BigInteger& num) : sign(num.sign){
        for(auto it = num.digits.begin(); it != num.digits.end(); ++it){
            digits.push_back(*it);
        }
    }
    BigInteger(int value){
        int cpy = value;
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


    BigInteger& operator+=(BigInteger num){
        int remainder = 0; int a;
        if((sign ? *this:-*this) < (num.sign ? num:-num)){
            swap(num);
        }
        if(num.sign == sign) {
            for (size_t i = 0; i < digits.size(); ++i) {
                if(i >= num.digits.size()) {
                    a = 0;
                }
                else
                    a = num.digits.at(i);
                digits.at(i) += a + remainder;
                remainder = digits.at(i) / rank;
                digits.at(i) %= rank;
            }
            if(remainder > 0){
                digits.push_back(remainder);
            }
        }
        else{
            for(size_t i = 0; i < digits.size(); ++i){
                if(i >= num.digits.size()) {
                    a = 0;
                }
                else
                    a = num.digits.at(i);
                digits.at(i) = mod(digits.at(i) - a - remainder);
                remainder = (digits.at(i) + a + remainder) / rank;
            }
            removeZeros();
        }
        return *this;
    }

    void setZeros(){
        for(auto it = digits.begin(); it != digits.end(); ++it)
            *it = 0;
    }

    BigInteger& operator*=(const BigInteger& num){
        BigInteger thisCopy = *this;
        int remainder = 0;
        digits.resize(thisCopy.digits.size() + num.digits.size() + 2);
        setZeros();
        for(size_t i = 0, j; i < num.digits.size(); ++i){
            remainder = 0;
            for(j = 0; j < thisCopy.digits.size(); ++j){
                digits.at(i + j) += num.digits.at(i) * thisCopy.digits.at(j) + remainder;
                remainder = digits.at(i + j) / rank;
                digits.at(i + j) %= rank;
            }
            if(remainder > 0){
                digits.at(i + j) += remainder;
            }
        }
        removeZeros();
        sign = thisCopy.sign == num.sign;
        return *this;
    }

    BigInteger& operator-=(const BigInteger& num){
        *this += -num;
        return *this;
    }

    BigInteger& operator++(){
        *this += 1;
        return *this;
    }

    BigInteger operator++(int){
        BigInteger copy = *this;
        ++*this;
        return copy;
    }

    BigInteger& operator--(){
        *this -= 1;
        return *this;
    }

    BigInteger operator--(int){
        BigInteger copy = *this;
        --*this;
        return copy;
    }



    static BigInteger powRank(int n){
        BigInteger res;
        for(int i = 1; i <= n; ++i){
            res.digits.push_back(0);
        }
        if(n >= 0)
            res.digits.at(n) = 1;
        return res;
    }

    std::string divide(const BigInteger& num, size_t presicion){
        std::string res = "";
        if(!*this || !num){
            for(size_t i = 0; i < presicion; ++i)
                res += "0000";
            return res;
        }
        if(*this < num){
            for(size_t i = 0; i < presicion; ++i) {
                *this *= rank;
                while(*this < num) {
                    *this *= rank;
                    res += "0000";
                    i++;
                    if(i >= presicion)
                        break;
                }
                if(i >= presicion)
                    break;
                int l = 1, r = rank, m;
                while (l + 1 < r) {
                    m = (l + r) / 2;
                    if (*this < (m * num)) {
                        r = m;
                    } else {
                        l = m;
                    }
                }
                *this -= l * num;
                //std::cout << res << "\n";
                res += BigInteger(l).toString(0);
                //std::cout << res << "\n";
            }
        }
        return res;
    }

    void addZeros(size_t n){
        std::vector<int> copy = digits;
        digits.resize(n + digits.size());
        for(size_t i = 0; i < n; ++i)
          digits[i] = 0;
        for(size_t i = n; i < digits.size(); ++i)
            digits[i] = copy[i - n];
    }

    void delZero(){
        for(size_t i = 0; i < digits.size() - 1; ++i)
            digits[i] = digits[i + 1];
        digits.pop_back();
    }

    BigInteger& operator/=(const BigInteger& x){
        bool resSign = x.sign == sign;
        BigInteger thisCopy = *this; thisCopy.sign = true;
        BigInteger num = x; num.sign = true;
        digits.resize(1);
        digits.at(0) = 0;
        if(thisCopy < num || !num) {
            sign = resSign;
            return *this;
        }
        size_t zeros; BigInteger k;
        while(thisCopy >= num){
            zeros = thisCopy.digits.size() - num.digits.size();
            k = num; k.addZeros(zeros); // k = num с zeros нулями на конце
            if(thisCopy < k) {
                zeros--;
                k.delZero();
            }
            int l = 1, r = rank, m;
            while(l + 1 < r){
                m = (l + r) / 2;
                if(thisCopy < (m * k)){
                    r = m;
                }
                else{
                    l = m;
                }
            }
            thisCopy -= l * k;
            BigInteger t(l);
            t.addZeros(zeros);
            *this += t;
        }
        sign = resSign;
        return *this;
    }

    BigInteger& operator%=(const BigInteger& x){
        bool resSign = sign;
        BigInteger thisCopy = *this; thisCopy.sign = true;
        BigInteger num = x; num.sign = true;
        if(thisCopy < num || !num){
            sign = resSign;
            return *this;
        }
        size_t zeros; BigInteger k;
        while(thisCopy >= num){
            zeros = thisCopy.digits.size() - num.digits.size();
            k = num; k.addZeros(zeros); // k = num с zeros нулями на конце
            if(thisCopy < k) {
                //zeros--;
                k.delZero();
            }
            int l = 1, r = rank, m;
            while(l + 1 < r){
                m = (l + r) / 2;
                if(thisCopy < (m * k)){
                    r = m;
                }
                else{
                    l = m;
                }
            }
            thisCopy -= l * k;
        }
        *this = thisCopy;
        sign = resSign;
        return *this;
    }

    std::string toString(int first = 1) const{
        std::stringstream ss;
        if(!sign && *this)
            ss << '-';
        int r, val; size_t i = digits.size() - first;
        if(first == 1)
            ss << digits.at(digits.size() - 1);
        for(; i >= 1; --i) {
            r = rank / 10; val = digits.at(i - 1);
            while(r > 0) {
                ss << val / r;
                val %= r;
                r /= 10;
            }
        }
        return ss.str();
    }

    explicit operator bool() const
    {
        return digits.at(digits.size() - 1) != 0;
    }

    BigInteger operator-() const {
        BigInteger copy=*this;
        copy.sign= !copy.sign;
        return copy;
    }

    bool getSign() const{
        return sign;
    }

    void setSign(bool s){
        sign = s;
    }

};


bool operator==(const BigInteger& num1, const BigInteger& num2){
    if(!num1 && !num2){
        return true;
    }
    if(num1.sign != num2.sign)
        return false;
    if(num1.digits.size() != num2.digits.size())
        return false;
    for(size_t i = 0; i < num1.digits.size(); ++i){
        if(num1.digits.at(i) != num2.digits.at(i))
            return false;
    }
    return true;
}

bool operator!=(const BigInteger& num1, const BigInteger& num2){
    return !(num1 == num2);
}

bool operator<(const BigInteger& num1, const BigInteger& num2){
    if(!num1 && !num2){
        return false;
    }
    if(num1.sign == num2.sign){
        if(num1.digits.size() < num2.digits.size())
            return num1.sign;
        if(num1.digits.size() > num2.digits.size())
            return !num1.sign;
        for(size_t i = num1.digits.size(); i >= 1; --i){
            if(num1.digits.at(i - 1) < num2.digits.at(i - 1))
                return num1.sign;
            if(num1.digits.at(i - 1) > num2.digits.at(i - 1))
                return !num1.sign;
        }
        return false;
    }
    return !num1.sign;
}

bool operator>(const BigInteger& num1, const BigInteger& num2){
    return num2 < num1;
}

bool operator<=(const BigInteger& num1, const BigInteger& num2){
    return !(num1 > num2);
}

bool operator>=(const BigInteger& num1, const BigInteger& num2){
    return !(num1 < num2);
}

BigInteger operator+(const BigInteger& num1, const BigInteger& num2){
    BigInteger res = num1;
    res += num2;
    return res;
}

BigInteger operator-(const BigInteger& num1, const BigInteger& num2){
    BigInteger res = num1;
    res -= num2;
    return res;
}

BigInteger operator*(const BigInteger& num1, const BigInteger& num2){
    BigInteger res = num1;
    res *= num2;
    return res;
}

BigInteger operator/(const BigInteger& num1, const BigInteger& num2){
    BigInteger res = num1;
    res /= num2;
    return res;
}

BigInteger operator%(const BigInteger& num1, const BigInteger& num2){
    BigInteger res = num1;
    res %= num2;
    return res;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& num){
    out << num.toString();
    return out;
}

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
        len = (i >= 4 + end)? 4 : i - end + 1;
        num.digits.push_back(stoi(s.substr(i - len, len)));
    }
    /*std::cout << num << "\n";
    for(int i = 0; i < num.digits.size() / 2; ++i){
        std::swap(num.digits.at(i), num.digits.at(num.digits.size() - i - 1));
    }
    std::cout << num << "\n";*/
    return in;
}

//******************************************************************************************************************

class Rational{
private:
    BigInteger numerator;
    BigInteger denumerator;
    BigInteger gcd(const BigInteger& a, const BigInteger& b) const{
        if(b == 0)
            return a;
        return gcd(b, a % b);
    }
    void setSign(){
        if(numerator.getSign() == denumerator.getSign() || !numerator || !denumerator) {
            numerator.setSign(true);
            denumerator.setSign(true);
        }
        else{
            numerator.setSign(false);
            denumerator.setSign(true);
        }
    }
    void cutBack(){
        setSign();
        BigInteger nod;
        if(numerator.getSign())
            nod = gcd(numerator, denumerator);
        else
            nod = gcd(-numerator, denumerator);
        numerator /= nod;
        denumerator /= nod;
    }
public:
    friend bool operator==(const Rational&, const Rational&);
    friend bool operator<(const Rational&, const Rational&);
    friend std::istream& operator>>(std::istream& in, Rational& digit);
    Rational(){
        numerator = 0;
        denumerator = 1;
    }
    Rational(const BigInteger& num){
        numerator = num;
        denumerator = 1;
    }
    Rational(int val){
        numerator = val;
        denumerator = 1;
    }

    Rational& operator=(const Rational& x){
        numerator = x.numerator;
        denumerator = x.denumerator;
        return *this;
    }

    Rational& operator+=(const Rational& x){
        numerator *= x.denumerator;
        numerator += x.numerator * denumerator;
        denumerator *= x.denumerator;
        cutBack();
        return *this;

    }
    Rational& operator-=(const Rational& x){
        *this += -x;
        return *this;
    }
    Rational& operator/=(const Rational& x){
        numerator *= x.denumerator;
        denumerator *= x.numerator;
        cutBack();
        return *this;
    }
    Rational& operator*=(const Rational& x){
        numerator *= x.numerator;
        denumerator *= x.denumerator;
        cutBack();
        return *this;
    }
    std::string toString(){
        std::stringstream res;
        cutBack();
        res << numerator.toString();
        if(denumerator != 1 && denumerator)
            res << "/" << denumerator.toString();
        return res.str();
    }
    std::string asDecimal(size_t precision = 0) const{
        std::stringstream res;
        if(precision == 0){
            if((numerator / denumerator) || numerator.getSign() || !numerator)
                res << (numerator/denumerator).toString();
            else
                res << "-" << (numerator/denumerator).toString();
            return  res.str();
        }
        if((numerator / denumerator) || numerator.getSign() || !numerator)
            res << (numerator/denumerator).toString() << ".";
        else
            res << "-" << (numerator/denumerator).toString() << ".";
        std::string s;
        if(numerator.getSign())
            s =(numerator%denumerator).divide(denumerator, precision / 4 + !(precision % 4 == 0));
        else
            s =((-numerator)%denumerator).divide(denumerator, precision / 4 + !(precision % 4 == 0));
        for(size_t i = 0; i < 4 - (precision % 4 == 0 ? 4:precision % 4); ++i)
            s.pop_back();
        res << s;
        return res.str();
    }
    explicit operator double() const{
        return strtod(asDecimal(50).c_str(), nullptr);
    }
    Rational operator-() const{
        Rational res = *this;
        res.numerator.setSign(!res.numerator.getSign());
        return res;
    }
};

Rational operator+(const Rational& x, const Rational& y){
    Rational res = x;
    res += y;
    return res;
}
Rational operator-(const Rational& x, const Rational& y){
    Rational res = x;
    res -= y;
    return res;
}
Rational operator*(const Rational& x, const Rational& y){
    Rational res = x;
    res *= y;
    return res;
}
Rational operator/(const Rational& x, const Rational& y){
    Rational res = x;
    res /= y;
    return res;
}
bool operator==(const Rational& x, const Rational& y){
    if(x.numerator == y.numerator && x.denumerator == y.denumerator)
        return true;
    return false;
}
bool operator!=(const Rational& x, const Rational& y){
    return !(x == y);
}
bool operator<(const Rational& x, const Rational& y){
    if(x.numerator * y.denumerator < y.numerator * x.denumerator)
        return true;
    return false;
}
bool operator>(const Rational& x, const Rational& y){
    return y < x;
}
bool operator<=(const Rational& x, const Rational& y){
    return !(x > y);
}
bool operator>=(const Rational& x, const Rational& y){
    return !(x < y);
}

std::istream& operator>>(std::istream& in, Rational& digit){
    std::string s;
    std::stringstream num1;
    std::stringstream num2;
    in >> s;
    size_t i = 0;
    for(; s[i] != '/' && i < s.size(); ++i){
        num1.put(s[i]);
    }
    num1 >> digit.numerator;
    if(i++ < s.size()){
        for(; i < s.size(); ++i)
            num2.put(s[i]);
        num2 >> digit.denumerator;
    }else{
        digit.denumerator = 1;
    }
    digit.cutBack();
    return in;
}


//********************************************************************************************************************

template<long long N>
class Finite{
private:
    long long value = 0;
    static long long mod(const long long&);
    long long getPow(long long) const;
public:
    template<long long M>
    friend std::istream& operator>>(std::istream& in, Finite<M>& digit);
    Finite() = default;
    Finite(const long long& x): value(mod(x)) {}


    Finite& operator++();
    Finite operator++(int);
    Finite& operator--();
    Finite operator--(int);
    Finite& operator+=(const Finite&);
    Finite operator-() const;
    Finite& operator-=(const Finite&);
    Finite& operator*=(const Finite&);
    Finite& operator/=(const Finite&);
    Finite inverted() const;
    long long getValue() const;
    long long toString() const; //костылечек для вывода матрицы для дебага, просто в Rational тоже есть такой метод

    friend Finite<N> operator+(const Finite<N>& x1, const Finite<N>& x2) {
        Finite<N> copy = x1;
        copy += x2;
        return copy;
    }
    friend Finite<N> operator-(const Finite<N>& x1, const Finite<N>& x2) {
        Finite<N> copy = x1;
        copy -= x2;
        return copy;
    }
    friend Finite<N> operator*(const Finite<N>& x1, const Finite<N>& x2) {
        Finite<N> copy = x1;
        copy *= x2;
        return copy;
    }
    friend Finite<N> operator/(const Finite<N>& x1, const Finite<N>& x2) {
        Finite<N> copy = x1;
        copy /= x2;
        return copy;
    }
    friend bool operator==(const Finite<N>& x1, const Finite<N>& x2) {
        return x1.getValue() == x2.getValue();
    }

    friend bool operator!=(const Finite<N>& x1, const Finite<N>& x2) {
        return x1.getValue() != x2.getValue();
    }
};

template<long long N>
long long Finite<N>::mod(const long long& x) {
    return (N + x % N) % N;
}

template<long long N>
long long Finite<N>::getPow(long long n) const {
    if(n == 0)
        return 1;
    if(n % 2 == 0)
        return mod(getPow(n / 2) * getPow(n / 2));
    return mod(value * getPow(n - 1));
}

template<long long N>
Finite<N>& Finite<N>::operator+=(const Finite& x) {
    value = mod(value + mod(x.value));
    return *this;
}

template<long long N>
Finite<N> Finite<N>::operator-() const{
    Finite<N> copy = *this;
    copy.value = mod(N - copy.value);
    return copy;
}

template<long long N>
Finite<N>& Finite<N>::operator-=(const Finite& x) {
    *this += -x;
    return *this;
}

template<long long N>
Finite<N>& Finite<N>::operator*=(const Finite& x) {
    value = mod(value * mod(x.value));
    return *this;
}

template<long long N>
Finite<N>& Finite<N>::operator/=(const Finite& x) {
    *this *= x.inverted();
    return *this;
}

template<long long N>
long long Finite<N>::getValue() const{
    return value;
}

template<long long N>
Finite<N> Finite<N>::inverted() const {
    return getPow(N-2);
}

template<long long N>
long long Finite<N>::toString() const {
    return value;
}

template<long long int N>
Finite<N> &Finite<N>::operator++() {
    *this += 1;
    return *this;
}

template<long long int N>
Finite<N> Finite<N>::operator++(int) {
    Finite<N> copy = *this;
    *this += 1;
    return copy;
}

template<long long int N>
Finite<N> &Finite<N>::operator--() {
    *this -= 1;
    return *this;
}

template<long long int N>
Finite<N> Finite<N>::operator--(int) {
    Finite<N> copy = *this;
    *this -= 1;
    return copy;
}

template<long long int M>
std::istream &operator>>(std::istream &in, Finite<M>& digit) {
    in >> digit.value;
    digit.value = Finite<M>::mod(digit.value);
    return in;
}

//*****************************************************************************************************************


template<unsigned N, unsigned M, typename Field = Rational>
class Matrix{
protected:
    std::vector<std::vector<Field> >matrix;
public:
    Matrix();
    Matrix(const Field&);
    Matrix(const std::vector<std::vector<Field> >&);
    Matrix(const std::vector<std::vector<int> >&);

    bool operator==(const Matrix<N, M, Field>&) const;
    bool operator!=(const Matrix<N, M, Field>&) const;
    Matrix& operator+=(const Matrix<N, M, Field>&);
    Matrix& operator-=(const Matrix<N, M, Field>&);
    Matrix& operator*=(const Matrix<N, M, Field>&);
    Matrix& operator*=(const Field&);
    Matrix operator+(const Matrix<N, M, Field>&) const;
    Matrix operator-(const Matrix<N, M, Field>&) const;
    std::vector<Field> getRow(unsigned) const;
    std::vector<Field> getColumn(unsigned) const;
    std::vector<Field>& operator[](unsigned);
    const std::vector<Field>& operator[](unsigned) const;
    Matrix<M, N, Field> transposed() const;
    unsigned rank() const;

    Field det()const ;
    Field trace()const;
    Matrix inverted() const;
    void invert();

    void print();
};

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field> operator*(const Matrix<N, M, Field>& matrix, const Field& value){
    Matrix<N, M, Field> res = matrix;
    res *= value;
    return res;
}

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field> operator*(const Field& value, const Matrix<N, M, Field>& matrix){
    Matrix<N, M, Field> res = matrix;
    res *= value;
    return res;
}

template<unsigned int N, unsigned int M, typename Field>
void Matrix<N, M, Field>::print(){
    for(unsigned i = 0; i < N; ++i){
        for(unsigned j = 0; j < M; ++j){
            std::cout << matrix[i][j].toString() << " ";
        }
        std::cout << "\n";
    }
}

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>::Matrix(){
    matrix = std::vector<std::vector<Field>>(N);
    for(unsigned i = 0; i < N; ++i){
        matrix[i] = std::vector<Field>(M);
        if(N == M)
            matrix[i][i] = 1;
    }
}

template<unsigned N, unsigned M, typename Field>
Matrix<N, M, Field>::Matrix(const Field& x){
    matrix = std::vector<std::vector<Field>>(N);
    for(unsigned i = 0; i < N; ++i){
        matrix[i] = std::vector<Field>(M);
        for(unsigned j = 0; j < M; ++j)
            matrix[i][j] = x;
    }
}


template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field>::Matrix(const std::vector<std::vector<Field>>& init) {
    matrix = std::vector<std::vector<Field>>(N);
    for(unsigned i = 0; i < N; ++i){
        matrix[i] = std::vector<Field>(M);
        for(unsigned j = 0; j < M; ++j){
            matrix[i][j] = init.at(i).at(j);
        }
    }
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field>::Matrix(const std::vector<std::vector<int>>& init) {
    matrix = std::vector<std::vector<Field>>(N);
    for(unsigned i = 0; i < N; ++i){
        matrix[i] = std::vector<Field>(M);
        for(unsigned j = 0; j < M; ++j){
            matrix[i][j] = init.at(i).at(j);
        }
    }
}

template<unsigned int N, unsigned int M, typename Field>
bool Matrix<N, M, Field>::operator==(const Matrix& x) const {
    for(unsigned i = 0; i < N; ++i) {
        for (unsigned j = 0; j < M; ++j) {
            if (matrix[i][j] != x.matrix[i][j])
                return false;
        }
    }
    return true;
}

template<unsigned int N, unsigned int M, typename Field>
bool Matrix<N, M, Field>::operator!=(const Matrix& x) const {
    return !(*this == x);
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field> &Matrix<N, M, Field>::operator+=(const Matrix& x) {
    for(unsigned i = 0; i < N; ++i) {
        for (unsigned j = 0; j < M; ++j) {
            matrix[i][j] += x.matrix[i][j];
        }
    }
    return *this;
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field> &Matrix<N, M, Field>::operator-=(const Matrix& x) {
    for(unsigned i = 0; i < N; ++i) {
        for (unsigned j = 0; j < M; ++j) {
            matrix[i][j] -= x.matrix[i][j];
        }
    }
    return *this;
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field>& Matrix<N, M, Field>::operator*=(const Field& value) {
    for(unsigned i = 0; i < N; ++i) {
        for(unsigned j = 0; j < M; ++j)
            matrix.at(i).at(j) *= value;
    }
    return *this;
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field> Matrix<N, M, Field>::operator+(const Matrix& x) const {
    Matrix<N, M, Field> res = *this;
    res += x;
    return res;
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field> Matrix<N, M, Field>::operator-(const Matrix& x) const {
    Matrix<N, M, Field> res = *this;
    res -= x;
    return res;
}

template<unsigned int N, unsigned int M, unsigned int K, typename Field>
Matrix<N, K, Field> operator*(const Matrix<N, M, Field>& x1, const Matrix<M, K, Field>& x2) {
    Matrix<N, K, Field> res(0);
    for(unsigned i = 0; i < N; ++i) {
        for (unsigned j = 0; j < K; ++j) {
            for(unsigned g = 0; g < M; ++g){
                res[i][j] += x1[i][g] * x2[g][j];
            }
        }
    }
    return res;
}

template<unsigned int N, unsigned int M, typename Field>
std::vector<Field> Matrix<N, M, Field>::getRow(unsigned i) const {
    return matrix[i];
}

template<unsigned int N, unsigned int M, typename Field>
std::vector<Field> Matrix<N, M, Field>::getColumn(unsigned j) const {
    std::vector<Field> res(N);
    for(unsigned i = 0; i < N; ++i)
        res[i] = matrix[i][j];
    return res;
}

template<unsigned int N, unsigned int M, typename Field>
std::vector<Field> &Matrix<N, M, Field>::operator[](unsigned i) {
    return matrix[i];
}

template<unsigned int N, unsigned int M, typename Field>
const std::vector<Field> &Matrix<N, M, Field>::operator[](unsigned i) const {
    return matrix[i];
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<M, N, Field> Matrix<N, M, Field>::transposed() const {
    Matrix<M, N, Field> res;
    for(unsigned i = 0; i < N; ++i) {
        for(unsigned j = 0; j < M; ++j)
            res[j][i] = matrix[i][j];
    }
    return res;
}

template<typename Field>
std::vector<Field> operator*(const std::vector<Field>& row, const Field& x){
    std::vector<Field> copy = row;
    for(size_t i = 0; i < copy.size(); ++i){
        copy[i] *= x;
    }
    return copy;
}

template<typename Field>
std::vector<Field> operator-(const std::vector<Field>& row1, const std::vector<Field>& row2){
    std::vector<Field> copy = row1;
    for(size_t i = 0; i < copy.size(); ++i){
        copy[i] -= row2[i];
    }
    return copy;
}


template<unsigned int N, unsigned int M, typename Field>
unsigned Matrix<N, M, Field>::rank() const {
    Matrix<N, M, Field> copy = *this;
    unsigned rank = 0;
    for(unsigned j = 0, i = 0; i < N && j < M; ++j){
        bool f = false; //false если все что не выше i в текущем столбце - нули

        if(copy[i][j] == 0){
            for(unsigned g = i + 1; g < N; ++g){
                if(copy[g][j] != 0){
                    f = true;
                    std::swap(copy[i], copy[g]);
                    break;
                }
            }
        }
        else{
            f = true;
        }

        if(f){
            Field a = 1 / copy[i][j];
            bool finish = true;
            for(unsigned g = i + 1; g < N; ++g){
                if(copy[g][j] != 0) {
                    //copy[g] = copy[g] - (copy[i] * a * copy[g][j]);
                    for(unsigned t = j + 1; t < M; ++t){
                        copy[g][t] -= copy[i][t] * a * copy[g][j];
                        if(copy[g][t] != 0)
                            finish = false;
                    }
                    copy[g][j] = 0;
                }
            }
            ++i;
            ++rank;
            if(finish)
                break;
        }
    }
    return rank;
}

template<unsigned int N, unsigned int M, typename Field>
Field Matrix<N, M, Field>::det() const{
    static_assert(N == M, "det is not defined for not square matrix");
    Matrix<N, M, Field> copy = *this;
    Field res = 1;
    for(unsigned i = 0; i < N; ++i){
        bool f = false; //false если все что не выше i в текущем столбце - нули

        if(copy[i][i] == 0){
            for(unsigned g = i + 1; g < N; ++g){
                if(copy[g][i] != 0){
                    f = true;
                    res *= -1; // определитель сменил знак
                    std::swap(copy[i], copy[g]);
                    break;
                }
            }
        }
        else{
            f = true;
        }

        res *= copy[i][i];
        if(res == 0){
            break;
        }
        if(f) {
            Field a = 1 / copy[i][i];
            for (unsigned g = i + 1; g < N; ++g) {
                if(copy[g][i] != 0) {
                    //copy[g] = copy[g] - (copy[i] * a * copy[g][i]);
                    for(unsigned j = i + 1; j < M; ++j){
                        copy[g][j] -= copy[i][j] * a * copy[g][i];
                    }
                    copy[g][i] = 0;
                }
            }
        }
    }
    //copy.print();
    return res;
}

template<unsigned int N, unsigned int M, typename Field>
Field Matrix<N, M, Field>::trace() const {
    static_assert(N == M, "trace is not defined for not square matrix");
    Field res = 0;
    for(unsigned i = 0; i < N; ++i)
        res += matrix[i][i];
    return res;
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field> Matrix<N, M, Field>::inverted() const{
    static_assert(N == M, "inverted is not defined for not square matrix");
    //assert(det() != 0);
    Matrix<N, 2 * M, Field> paired;
    for(unsigned i = 0; i < N; ++i){
        for(unsigned j = 0; j < M; ++j)
            paired[i][j] = matrix[i][j];
        paired[i][i + M] = 1;
    }
    for(unsigned i = 0; i < N; ++i){
        if(paired[i][i] == 0){
            for(unsigned g = i + 1; g < N; ++g){
                if(paired[g][i] != 0){
                    std::swap(paired[i], paired[g]);
                    break;
                }
            }
        }
        paired[i] = paired[i] * (1 / paired[i][i]);
        for(unsigned g = 0; g < N; ++g){
            if(g != i && paired[g][i] != 0){
                //paired[g] = paired[g] - (paired[i] * paired[g][i]);
                for(unsigned j = i + 1; j < 2 * M; ++j){
                    paired[g][j] -= paired[i][j] * paired[g][i];
                }
                paired[g][i] = 0;
            }
        }
    }
    Matrix<N, M, Field> res;
    for(unsigned i = 0; i < N; ++i) {
        for (unsigned j = 0; j < M; ++j) {
            res[i][j] = paired[i][j + M];
        }
    }
    return res;
}

template<unsigned int N, unsigned int M, typename Field>
void Matrix<N, M, Field>::invert(){
    matrix = inverted().matrix;
}

template<unsigned int N, unsigned int M, typename Field>
Matrix<N, M, Field>& Matrix<N, M, Field>::operator*=(const Matrix<N, M, Field>& x) {
    static_assert(N == M, "operator *= is not defined for not square matrix");
    *this = (*this * x);
    return *this;
}




//*************************************************************************************************************
template <size_t N, typename Field = Rational>
using SquareMatrix = Matrix<N, N, Field>;

#endif //MATRIX_RATIONAL_H
