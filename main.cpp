#include <iostream>

typedef unsigned long long lld;
const int byte = 256;

void stableSort(lld* a, int n, lld *copy, int *digits, int k){
    for(int i = 0; i < n; ++i){
        digits[(a[i] >> (8 * (k - 1))) % byte]++;
    }
    for(int i = 1; i < byte; ++i){
        digits[i] += digits[i - 1];
    }
    for(int i = n - 1; i >= 0; --i){
        copy[--digits[(a[i] >> (8 * (k - 1))) % byte]] = a[i];
    }
    for(int i = n - 1; i >= 0; --i){
        a[i] = copy[i];
        copy[i] = 0;
    }
    for(int i = 1; i < byte; ++i){
        digits[i] = 0;
    }
}

void LSDsort(lld *a, int n){
    int *digits = (int*)calloc(byte, sizeof(int));
    lld *copy = (lld*)calloc(n, sizeof(lld));
    int k = 1;
    while(k < 9){
        stableSort(a, n, copy, digits, k);
        /*for(int i = 0; i < n; ++i)
            std::cout << a[i] << " ";
        std::cout << "\n";*/
        k++;
    }
    free(digits);
    free(copy);
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
    free(a);
    return 0;
}
