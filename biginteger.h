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
    void setZeros(){
        for(auto it = digits.begin(); it != digits.end(); ++it)
            *it = 0;
    }
public:
    friend bool operator==(const BigInteger&, const BigInteger&);
    friend bool operator<(const BigInteger&, const BigInteger&);
    friend bool operator>=(const BigInteger&, const BigInteger&);
    friend std::istream& operator>>(std::istream&, BigInteger&);
    friend BigInteger operator*(const BigInteger&,const BigInteger&); //без этого почему-то не работают вызовы умножения внутри методов. Видимо надо было реализоввать методы вне класса, но я понял что так удобней только когда геометрию писал
    BigInteger() : sign(true){
        digits.push_back(0);
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


    BigInteger& operator+=(const BigInteger& num){
        int remainder = 0; int a;
        if(num.sign == sign) {
            for (size_t i = 0; i < std::max(digits.size(), num.digits.size()); ++i) {
                a = (i >= num.digits.size()) ? 0:num.digits.at(i);
                if(i >= digits.size())
                    digits.push_back(0);
                digits.at(i) += a + remainder;
                remainder = digits.at(i) / rank;
                digits.at(i) %= rank;
            }
            if(remainder > 0){
                digits.push_back(remainder);
            }
        }
        else{
            int s1 = 1; // s1 - знак цифр в this
            int s2 = -1; // знак цифр в num. С минусом будем брать цифры меньшего по модулю числа
            if((sign ? *this:-*this) < (num.sign ? num:-num)){
                s1 = -1;
                s2 = 1;
                sign = !sign;
            }
            for(size_t i = 0; i < std::max(digits.size(), num.digits.size()); ++i){
                a = (i >= num.digits.size()) ? 0:num.digits.at(i);
                if(i >= digits.size())
                    digits.push_back(0);
                digits.at(i) = digits.at(i) * s1 + a * s2 - remainder;
                remainder = digits.at(i) < 0? 1: 0;
                digits.at(i) = mod(digits.at(i));
            }

            removeZeros();
        }
        return *this;
    }

    BigInteger& operator*=(const BigInteger& num){
        if(num == 1){
            return *this;
        }
        if(num == -1){
            sign = !sign;
            return *this;
        }
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
        *this += -num;// Нагло, даже очень. Но ладно
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

    void addZeros(size_t n){ // использовал это в матрицах, чтобы уравнивать длину числа при делении, и прошло по времени. До этого я просто умножал на 10^n и валилось
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
        if((numerator < 0 && denumerator < 0) || (numerator > 0 && denumerator > 0) || !numerator || !denumerator) {
            numerator *= (numerator < 0) ? -1: 1;

        }
        else{
            numerator *= (numerator > 0) ? -1: 1;
        }
        denumerator *= (denumerator < 0) ? -1: 1;
    }
    void cutBack(){
        setSign();
        BigInteger nod;
        if(numerator >= 0)
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
    std::string asDecimal(size_t precision = 0) const{
        std::stringstream res;
        if(precision == 0){
            if((numerator / denumerator) || numerator >= 0 || !numerator)
                res << (numerator/denumerator).toString();
            else
                res << "-" << (numerator/denumerator).toString();
            return  res.str();
        }
        if((numerator / denumerator) || numerator>= 0 || !numerator)
            res << (numerator/denumerator).toString() << ".";
        else
            res << "-" << (numerator/denumerator).toString() << ".";
        std::string s;
        if(numerator >= 0)
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
        res.numerator *= -1;
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
