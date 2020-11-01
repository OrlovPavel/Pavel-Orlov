#include <iostream>
#include <algorithm>
#include <ctime>

int particion(int *array, int l, int r){
    int i = l, j = r, p = array[(r + l) / 2];
    while(i <= j){
        while(array[i] < p) ++i;
        while(array[j] > p) --j;
        if(i <= j){
            std::swap(array[i], array[j]);
            ++i;
            --j;
        }
    }
    return i - l;
}

int findKstat(int *array, int n, int find){
    int k = find;
    int left = 0, right = n - 1, index;
    while(left < right){
        if(left + 1 == right){
            if(array[left] > array[right]) {
                std::swap(array[left], array[right]);
            }
            return array[left + k];
        }
        index = particion(array, left, right);
        if(k < index){
            right = left + index - 1;
        }
        if(k >= index){
            left += index;
            k -= index;
        }
    }
    return array[left + k];
}

int main() {
    int n, k;
    scanf("%d%d", &n, &k);
    int *array = (int*)calloc(n, sizeof(int));// А где освобождение массива? Да, даже в конце программы
    for(int i = 0; i < n; ++i)
        scanf("%d", &array[i]);
    printf("%d", findKstat(array, n, k));
    return 0;
}
