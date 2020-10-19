#include <iostream>
#include <vector>
#include <string>

class MyHeap{
private:
    std::vector<long long> heap;
    int max_size;
public:
    MyHeap(int size){
        heap.push_back(0);
        max_size = size;
    }
    void siftUp(int i){
        while(i > 1 && heap[i] > heap[i / 2]){
            std::swap(heap[i], heap[i / 2]);
            i /= 2;
        }
    }
    void siftDown(int i){
        while(2 * i <= heap.size() - 1){
            int j = -1;
            if(heap[2 * i] > heap[i]){
                j = 2 * i;
            }
            if((2 * i + 1 <= heap.size() - 1 && heap[2 * i + 1] > heap[i]) && (j == -1 || heap[2 * i] < heap[2 * i + 1])){
                j = 2 * i + 1;
            }
            if(j == -1){
                break;
            }
            else{
                std::swap(heap[i], heap[j]);
                i = j;
            }
        }
    }
    void insert(long long x){
        heap.push_back(x);
        siftUp(heap.size() - 1);
    }
    long long getMax(){
        return heap[1];
    }
    void extractMax(){
        heap[1] = heap[heap.size() - 1];
        heap.pop_back();
        siftDown(1);
    }
    void extractMax(long long value){
        heap[1] = value;
        siftDown(1);
    }
};




int main() {
    int n, k;
    long long x;
    scanf("%d%d", &n, &k);
    MyHeap heap(k);
    for(int i = 0; i < k; ++i){
        scanf("%d", &x);
        heap.insert(x);
    }
    for(int i = k; i < n; ++i){
        scanf("%d", &x);
        if(x < heap.getMax()){
            heap.extractMax(x);
        }
    }
    int *a = (int*)calloc(k, sizeof(int));
    for(int i = 0; i < k; ++i){
        a[i] = heap.getMax();
        heap.extractMax();
    }
    for(int i = k -1; i >=0; --i){
        printf("%d ", a[i]);
    }
    return 0;
}
