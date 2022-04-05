#include <iostream>
#include <vector>
#include <string>

class MyHeap{
private:
    std::pair<long long, int> p;
    std::vector<std::pair<long long, int> > heap;
    int *ptrs;
    int heap_size;
public:
    MyHeap(){
        ptrs = (int*)calloc(1000001, sizeof(int));
        p.first = 0; p.second = 0;
        heap.push_back(p);
        heap_size = 0;
    }
    
    ~MyHeap() {
        free(ptrs);   
    }
    
    void siftUp(int i){// Всё тоже - названия переменных
        while(i > 1 && heap[i] < heap[i / 2]){
            ptrs[heap[i].second] = i / 2;
            ptrs[heap[i / 2].second] = i;
            std::swap(heap[i], heap[i / 2]);
            i /= 2;
        }
    }
    
    void siftDown(int i){
        while(2 * i <= heap_size){
            int j = -1;
            if(heap[2 * i] < heap[i]){
                j = 2 * i;
            }
            if((2 * i + 1 <= heap_size && heap[2 * i + 1] < heap[i]) && (j == -1 || heap[2 * i] > heap[2 * i + 1])){
                j = 2 * i + 1;
            }
            if(j == -1){
                break;
            }
            else{
                ptrs[heap[i].second] = j;
                ptrs[heap[j].second] = i;
                std::swap(heap[i], heap[j]);
                i = j;
            }
        }
    }
    
    void insert(long long x, int ptr = 0){
        p.first = x; p.second = ptr;
        heap.push_back(p);
        heap_size++;
        ptrs[ptr] = heap_size;
        siftUp(heap_size);
    }
    
    long long getMin(){
        return heap[1].first;
    }
    
    void extractMin(){
        ptrs[heap[1].second] = 0;
        heap[1] = heap[heap_size];
        ptrs[heap[heap_size].second] = 1;
        heap.pop_back();
        heap_size--;
        siftDown(1);
    }
    
    void decreaseKey(int ptr, long long value){
        heap[ptrs[ptr]].first -= value;
        siftUp(ptrs[ptr]);
    }
};

int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    
    MyHeap heap;
    std::string query;
    int q, j;
    long long x;
    
    std::cin >> q;
    for(int i = 0; i < q; ++i){
        std::cin >> query;
        if(query == "insert"){
            std::cin >> x;
            heap.insert(x, i + 1);
        }
        if(query == "getMin"){
            std::cout << heap.getMin() << "\n";
        }
        if(query == "extractMin"){
            heap.extractMin();
        }
        if(query == "decreaseKey"){
            std::cin >> j >> x;
            heap.decreaseKey(j, x);
        }
    }
    return 0;
}
