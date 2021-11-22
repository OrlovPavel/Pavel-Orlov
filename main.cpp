#include <iostream>
#include <vector>
#include <cmath>
#include <complex>

struct complex {
    double re = 0;
    double im = 0;
    complex() : re(0), im(0) {}
    complex(const complex&) = default;
    complex(double re) : re(re), im(0) {}
    complex(double re, double im) : re(re), im(im) {}

    complex& operator*=(const complex& other) {
        double copy1 = re, copy2 = other.re;
        re = re * other.re - im * other.im;
        im = copy1 * other.im + im * copy2;
        return *this;
    }

    complex& operator/=(double div) {
        re /= div;
        im /= div;
        return *this;
    }

    complex& operator+=(const complex& other) {
        re += other.re;
        im += other.im;
        return *this;
    }

    complex& operator-=(const complex& other) {
        re -= other.re;
        im -= other.im;
        return *this;
    }
};

complex operator*(const complex& x, const complex& y) {
    complex res = x;
    res *= y;
    return res;
}

complex operator+(const complex& x, const complex& y) {
    complex res = x;
    res += y;
    return res;
}

complex operator-(const complex& x, const complex& y) {
    complex res = x;
    res -= y;
    return res;
}


const double pi = 3.14159265358979323846264;
const uint32_t k = 17;
const complex W(std::cos(2 * pi/(1u << k)), std::sin(2 * pi/(1u << k)));
std::vector<uint32_t> rev(1u << k);

complex pow(complex x, int n) {
    complex ans(1, 0);
    while(n > 0) {
        if(n % 2 == 1) {
            ans *= x;
        }
        x *= x;
        n /= 2;
    }
    return ans;
}

void fill_rev() {
    rev[0] = 0;
    uint32_t oldest = -1;
    for(uint32_t mask = 1; mask < (1u << k); ++mask) {
        if(!(mask & (mask - 1))) ++oldest;
        rev[mask] = rev[mask ^ (1u << oldest)] | (1u << (k - oldest - 1));
    }
}

void find_values(std::vector<complex>& a,  complex w = W) {
    std::vector<complex> copy = a;
    for(int i = 0; i < a.size(); ++i){
        a[i] = copy[rev[i]];
    }
    complex x;
    complex y;
    complex v;
    for(uint32_t j = 0; j < k; ++j) {
        v = pow(w, 1u << (k - j - 1));
        for(uint32_t i = 0; i < (1u << k); i += (1u << (j + 1))) {
            for(uint32_t s = 0; s < (1u << j); ++s) {
                x = a[i + s];
                y = a[i + (1u << j) + s];
                complex powed = pow(v, s);
                a[i + s] = x + powed * y;
                a[i + (1u << j) + s] = x - powed * y;
            }
        }
    }
}

void fft(std::vector<complex>& a, std::vector<complex>& b) {
    find_values(a);
    find_values(b);
    for(int i = 0; i < a.size(); ++i) {
        a[i] *= b[i];
    }
    find_values(a, complex(std::cos(2 * pi/(1u << k)), -std::sin(2 * pi/(1u << k))));
    for(int i = 0; i < a.size(); ++i) {
        a[i] /= 1u << k;
    }
}

int insert(std::vector<complex>& a) {
    int n;
    double val;
    std::cin >> n;
    for(int i = n; i >= 0; --i) {
        std::cin >> val;
        a[i] = val;
    }
    return n;
}

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    std::vector<complex> a(1u << k);
    std::vector<complex> b(1u << k);
    int nm = 0;
    nm += insert(a);
    nm += insert(b);
    fill_rev();
    fft(a, b);
    std::cout << nm << " ";
    for(int i = nm; i >= 0; --i) {
        double val = fabs(a[i].re);
        int ans = int(val);
        if(val - ans < 0.5) {
            std::cout << int(val) * (a[i].re >= 0? 1 : -1) << " ";
        } else{
            std::cout << (int(val) + 1) * (a[i].re >= 0? 1 : -1) << " ";
        }
    }
    return 0;
}