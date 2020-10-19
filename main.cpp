#include <iostream>
#include <fstream>


int* merge(int* b, int* c, int n, int m, int& ans){
    int i = 0;
    int j = 0;
    int *res = (int*)calloc(n + m, sizeof(int));
    while(i < n && j < m){
        if(b[i] <= c[j]){
            res[i + j] = b[i];
            ans += j;
            ++i;
        }
        else{
            res[i + j] = c[j];
            ++j;
        }
    }
    while(j < m){
        res[i + j] = c[j];
        ++j;
    }
    while(i < n){
        res[i + j] = b[i];
        ans += j;
        ++i;
    }
    return res;
}

void mergeSort(int *a, int l, int r, int& ans) {
    if (l == r) {
        return;
    }
    mergeSort(a, l, (l + r) / 2, ans);
    mergeSort(a, (l + r) / 2 + 1, r, ans);
    int *b = (int*) calloc((l + r) / 2 - l + 1, sizeof(int));
    int *c = (int*) calloc(r - (l + r) / 2, sizeof(int));
    for(int i = l; i <= r; ++i){
        if(i <= (l + r) / 2)
            b[i - l] = a[i];
        else
            c[i - (l + r) / 2 - 1] = a[i];
    }
    int *a1 = merge(b, c, (l + r)/2 - l + 1, r - (l + r)/2, ans);
    for(int i = l; i <= r; ++i){
        a[i] = a1[i - l];
    }
    free(b); free(c); free(a1);
    return;
}

int main() {
    freopen("inverse.in", "r", stdin);
    freopen("inverse.out", "w", stdout);
    int n, ans = 0;
    std::cin >> n;
    int *a = (int*) calloc(n, sizeof(int));
    for(int i = 0; i < n; ++i){
        std::cin >> a[i];
    }
    mergeSort(a, 0, n - 1, ans);
    /*for(int i = 0; i < n; ++i){
        std::cout << a[i] << " ";
    }*/
    std::cout << ans;
    return 0;
}
