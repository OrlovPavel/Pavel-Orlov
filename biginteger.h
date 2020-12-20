#ifndef BIGINTEGER_BIGINTEGER_H
#define BIGINTEGER_BIGINTEGER_H
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

class BigInteger{
private:
    static const int rank = 10000;
    std::vector<int> digits;
    bool sign = true;// is_positive

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
    // А их зачем friend, там всё равно скорее всего не используется ничего приватного
    friend BigInteger operator*(const BigInteger&,const BigInteger&);
    friend BigInteger operator+(const BigInteger&,const BigInteger&);
    
    BigInteger() : sign(true){
        digits.push_back(0);
    }
    
    BigInteger(const BigInteger& num) : sign(num.sign){// Для этого есть конструктор копирования у vector, уже всё давно реализовано
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

    // А почему он принимает обычный BigInt, а не const &? Это ведь Копирование!!! Короче переделай
    BigInteger& operator+=(BigInteger num){
        int remainder = 0; int a;
        if((sign ? *this:-*this) < (num.sign ? num:-num)){// И здесь тоже копирование при вызове -num
            swap(num);// И это тоже нельзя
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

    void setZeros(){// Даже боюсь спрашивать...
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
        *this += -num;// Тоже копирование, не катит
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
    // А ведь можно было просто (a*pow(10,precision)/num).toString и точку вставить, но ладно, так по идее быстрее, если явно считать
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
        size_t zeros;
        while(thisCopy >= num){
            zeros = thisCopy.digits.size() - num.digits.size();
            if(thisCopy < num * powRank(zeros))
                zeros--;
            int l = 1, r = rank, m;
            while(l + 1 < r){
                m = (l + r) / 2;
                if(thisCopy < (m * num * powRank(zeros))){
                    r = m;
                }
                else{
                    l = m;
                }
            }
            thisCopy -= l * num * powRank(zeros);
            *this += l * powRank(zeros);
        }
        sign = resSign;
        return *this;
    }

    BigInteger& operator%=(const BigInteger& x){
        // this = this - (this/x)*x
        bool resSign = sign;
        BigInteger thisCopy = *this; thisCopy.sign = true;
        BigInteger num = x; num.sign = true;
        if(thisCopy < num || !num){
            sign = resSign;
            return *this;
        }
        size_t zeros;
        while(thisCopy >= num){
            zeros = thisCopy.digits.size() - num.digits.size();
            if(thisCopy < num * powRank(zeros))
                zeros--;
            int l = 1, r = rank, m;
            while(l + 1 < r){
                m = (l + r) / 2;
                if(thisCopy < (m * num * powRank(zeros))){
                    r = m;
                }
                else{
                    l = m;
                }
            }
            thisCopy -= l * num * powRank(zeros);
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
        // Лучше заодно проверить что массив из 1 элемента... так на всякий
    }

    BigInteger operator-() const {
        BigInteger copy=*this;
        copy.sign= !copy.sign;
        return copy;
    }

    bool getSign() const{// Это метод должен быть private
        return sign;
    }

    void setSign(bool s){// А этот так тем более. Для этой цели можно перегрузить *=-1
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
            numerator.setSign(true);// *=-1
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
    // Как и в прошлый раз numerator*pow(10,precision)/denumerator. Причём у тебя уже реализована такая функция, зачем ещё раз и по другому?
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
#endif //BIGINTEGER_BIGINTEGER_H
