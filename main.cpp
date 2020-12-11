#include <iostream>

class SplayTree{
private:
    bool isEmpty = true;
    int key = 0;
    SplayTree* left = nullptr;
    SplayTree* right = nullptr;
    long long sum = 0;
    
    void setSum(){
        if(isEmpty)
            return;
        sum = key;
        sum += left->sum;
        sum += right->sum;
    }
    
    void splay(int x, bool isRoot = true){
        // Прям здесь поиск? Ну ладно
        if(left && x < key)
            left->splay(x, false);
        if(right && x > key)
            right->splay(x, false);
        if(!isEmpty && x != key){
            if(isRoot){
                if(right && x == right->key)
                    smallLeftRotate();
                if(left && x == left->key)
                    smallRightRotate();
            }
                if(left && left->left && left->left->key == x){ // zig-zig right
                    smallRightRotate();
                    smallRightRotate();
                }
                if(right && right->right && right->right->key == x){ // zig-zig left
                    smallLeftRotate();
                    smallLeftRotate();
                }
                if(left && left->right && left->right->key == x){ // zig-zag left-right
                    left->smallLeftRotate();
                    smallRightRotate();
                }
                if(right && right->left && right->left->key == x){ // zig-zag right-left
                    right->smallRightRotate();
                    smallLeftRotate();
                }
        }
    }
    void smallLeftRotate(){
        SplayTree root = *right;
        *right = *root.left;
        *root.left = *this;
        *this = root;
        left->setSum();
        setSum();
    }
    void smallRightRotate(){
        SplayTree root = *left;
        *left = *root.right;
        *root.right = *this;
        *this = root;
        right->setSum();
        setSum();
    }
    void merge(SplayTree tree){
        splay(findMax());
        *right = tree;
        this->setSum();
    }
    std::pair<SplayTree,SplayTree> split(int x){ // в певром дереве все < x, во втором >= x
        int border = next(x);
        std::pair<SplayTree,SplayTree> res;
        if(border == 1000000009) {
            splay(findMax());
            //print();std::cout <<"\n";
            res.first = *this;
            res.second = *(new SplayTree());
        }
        else {
            //std::cout << border;
            splay(border);
            //print();std::cout <<"\n";
            if (left)
                res.first = *left;
            else
                res.first = *(new SplayTree());
            res.second = *this;
            if(!res.second.isEmpty)
                res.second.left = new SplayTree();
            res.second.setSum();
        }
        return res;
    }
    void create(int x){
        isEmpty = false;//Зачем?? Всё равно при удалении... а хотя его тут нет
        key = x;
        sum = key;
        left = new SplayTree();
        right = new SplayTree();
    }
public:
    void print(){
        std::cout << "( " << key << ", " << sum << ") ";
        if(left){
            left->print();
        }
        if(right){
            right->print();
        }
    }
    bool exists(int find_key){
        if(isEmpty){
            return false;
        }
        if(find_key > key){
            return right->exists(find_key);
        }
        if(find_key < key){
            return left->exists(find_key);
        }
        return true;
    }
    void add(int x){
        if(x == 0)
            return;
        std::pair<SplayTree,SplayTree> splited = split(x);
        if(!splited.second.isEmpty && splited.second.key == x){
            *this = splited.first;
            merge(splited.second);
            splay(x);
        }
        else{
            create(x);
            *left = splited.first;
            *right = splited.second;
            setSum();
        }
    }
    long long summ(int l, int r){
        long long res = 0;
        //splay(next(r + 1));
        res += split(r + 1).first.sum;
        //splay(next(l));
        res -= split(l).first.sum;
        return res;
    }
    int findMax(){
        if(!right || right->isEmpty){
            return key;
        }
        return right->findMax();
    }
    int prev(int right_border){
        if(isEmpty){
            return -1000000009;
        }
        if(right_border < key){
            if(left->isEmpty)
                return -1000000009;
            else
                return left->prev(right_border);
        }
        else{
            if(right->isEmpty)
                return key;
            else
                return std::max(right->prev(right_border), key);
        }
    }
    int next(int left_border){
        if(isEmpty){
            return 1000000009;
        }
        if(left_border > key){
            if(right->isEmpty)
                return 1000000009;
            else
                return right->next(left_border);
        }
        else{
            if(left->isEmpty){
                return key;
            }
            else{
                return std::min(left->next(left_border), key);
            }
        }
    }
};

int main() {
    SplayTree tree; long long key, l, r, prevSum = 0;
    int n; char c;
    std::cin >> n;
    for(int i = 0; i < n; ++i){
        std::cin >> c;
        if(c == '+'){
            std::cin >> key;
            if(prevSum == 0)
                tree.add(key);
            else
                tree.add((prevSum + key)%1000000000);
            prevSum = 0;
        }
        if(c == '?'){
            std:: cin >> l >> r;
            prevSum = tree.summ(l, r);
            std::cout << prevSum << "\n";

        }
    }
    return 0;
}
