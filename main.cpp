#include <iostream>

typedef unsigned long long lld;

lld pow(lld a, lld n){
    if(n == 0) {
        return 1;
    }
    if(n % 2 == 1) {
        return a * pow(a, (n - 1));
    }
    return pow(a, n / 2) * pow(a, n / 2);
}

void stableSort(lld* a, int n, lld *copy, int *digits, int k){
    lld p = pow(10, k), q = pow(10, k - 1);
    for(int i = 0; i < n; ++i){
        digits[(a[i] % p) / q]++;
    }
    for(int i = 1; i < 10; ++i){
        digits[i] += digits[i - 1];
    }
    for(int i = n - 1; i >= 0; --i){
        copy[--digits[(a[i] % p) / q]] = a[i];
    }
    for(int i = n - 1; i >= 0; --i){
        a[i] = copy[i];
        copy[i] = 0;
    }
    for(int i = 1; i < 10; ++i){
        digits[i] = 0;
    }
}

void LSDsort(lld *a, int n){
    int *digits = (int*)calloc(10, sizeof(int));
    lld *copy = (lld*)calloc(n, sizeof(lld));
    int k = 1;
    while(k < 20){
        stableSort(a, n, copy, digits, k);
        /*for(int i = 0; i < n; ++i)
            std::cout << a[i] << " ";
        std::cout << "\n";*/
        k++;
    }
}

int main() {
    int n;
    scanf("%d", &n);
    lld *a = (lld*)calloc(n, sizeof(lld));
    for(int i = 0; i < n; ++i){
        scanf("%llu", &a[i]);
    }
    LSDsort(a, n);
    for(int i = 0; i < n; ++i){
        printf("%llu ", a[i]);
    }
    return 0;
}
