#include <iostream>

class SegmentTree{
private:
    size_t size = 1;
    long long sum = 0;
    SegmentTree* left = nullptr;
    SegmentTree* right = nullptr;
    void correctValues(){
        sum = 0;
        if(left){
            sum += left->sum;
        }
        if(right){
            sum += right->sum;
        }
    }
public:
    SegmentTree() = default;
    SegmentTree(size_t size);
    void setValue(size_t pos, long long value);
    long long getSum(size_t l, size_t r);
    void print(bool, size_t);
};

SegmentTree::SegmentTree(size_t size) : size(size), sum(0){
}

void SegmentTree::setValue(size_t pos, long long value){
    if(size == 1){
        sum += value;
        return;
    }
    size_t m = size / 2 + size % 2;
    if(pos < m){
        if(left)
            left->setValue(pos, value);
        else{
            left = new SegmentTree(m);
            left->setValue(pos, value);
        }
    }
    else {
        if(right)
            right->setValue(pos - m, value);
        else{
            right = new SegmentTree(size - m);
            right->setValue(pos - m, value);
        }
    }
    correctValues();
}

long long SegmentTree::getSum(size_t l, size_t r) {
    if(r - l + 1 == size){
        return sum;
    }
    size_t m = size / 2 + size % 2;
    if(r < m) {
        if(left)
            return left->getSum(l, r);
        return 0;
    }
    if(l >= m) {
        if(right)
            return right->getSum(l - m, r - m);
        return 0;
    }
    long long res = 0;
    if(left)
        res += left->getSum(l, m - 1);
    if(right)
        res += right->getSum(0, r - m);
    return res;
}


void SegmentTree::print(bool f = true, size_t pos = 0){
    if(size == 1) {
        std::cout << "[" << pos << ", " << pos + size - 1 << "] " << sum << "\n";
        return;
    }
    size_t m = size / 2 + size % 2;
    std::cout << "[" << pos << ", " << pos + size - 1 << "] " << sum << "\n";
    if(left)
        left->print(false, pos);
    if(right)
        right->print(false, pos + m);
    if(f)
        std::cout << "\n";
}


int main() {
    std::ios::sync_with_stdio(0);
    std::cin.tie(0);
    std::cout.tie(0);
    SegmentTree a(1e9);
    int q; char c; long long x;
    std::cin >> q;
    for(int i = 0; i < q; ++i){
        std::cin >> c >> x;
        if(c == '+'){
            a.setValue(x, x);
        }
        else{
            std::cout << a.getSum(0, x) << "\n";
        }
        //a.print();
    }
    return 0;
}