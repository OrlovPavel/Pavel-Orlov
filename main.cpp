#include <iostream>
#include <vector>

class BinaryIndTree1{
private:
   std::vector<int> sum;
   std::vector<int> values;
   int f(int x){
       return x & (x + 1);
   }
   int g(int x){
       return x | (x + 1);
   }
public:
    BinaryIndTree1() = default;
    BinaryIndTree1(std::vector<int>& array);
    void increaseValue(size_t pos, int d);
    void setValue(size_t pos, int value);
    int getSum(size_t r);
    int getSum(size_t l, size_t r);
};

BinaryIndTree1::BinaryIndTree1(std::vector<int>& array) {
    values.push_back(array[0]);
    sum.push_back(array[0]);
    for(size_t i = 1; i < array.size(); ++i){
        values.push_back(values[i - 1] + array[i]);
        if(f(i) > 0)
            sum.push_back(values[i] - values[f(i) - 1]);
        else
            sum.push_back(values[i]);
    }
    values = array;
}

void BinaryIndTree1::increaseValue(size_t pos, int d) {
    for(size_t i = pos; i < sum.size(); i = g(i))
        sum[i] += d;
    values[pos] += d;
}

void BinaryIndTree1::setValue(size_t pos, int value) {
    int d = value - values[pos];
    increaseValue(pos, d);
}

int BinaryIndTree1::getSum(size_t r) {
    int res = 0;
    for(int i = r; i >= 0; i = f(i) - 1)
        res += sum[i];
    return res;
}

int BinaryIndTree1::getSum(size_t l, size_t r) {
    return getSum(r) - getSum(l - 1);
}

class BinaryIndTree2{
private:
    std::vector<BinaryIndTree1*> tree;
public:
    BinaryIndTree2() = default;
    BinaryIndTree2(std::vector<std::pair<int, int>>& array);
};

BinaryIndTree2::BinaryIndTree2(std::vector<std::pair<int, int>>& array){
    tree.emplace_back(new BinaryIndTree1())
}


int main() {
    std::vector<int> array;
    int n, a;
    std::cin >> n;
    for(int i = 0; i < n; ++i){
        std::cin >> a;
        a *= (i % 2 == 0 ? 1:-1);
        array.push_back(a);
    }
    int m, q, l, r;
    BinaryIndTree tree(array);
    std::cin >> m;
    for(int i = 0; i < m; ++i){
        std::cin >> q >> l >> r;
        if(q == 0){
            l--;
            r *= (l % 2 == 0 ? 1:-1);
            tree.setValue(l, r);
        }
        if(q == 1){
            l--; r--;
            std::cout << tree.getSum(l, r) * (l % 2 == 0? 1: -1) << "\n";
        }
    }
    return 0;
}
