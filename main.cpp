#include <iostream>

class AVLTree{
private:
    bool isEmpty = true;
    int key = 0;
    int height = 0;
    int sonsCount = 0;
    AVLTree *left = nullptr;
    AVLTree *right = nullptr;
    int diff(){
        int l = 0, r = 0;
        if(left)
            l = left->height;
        if(right)
            r = right->height;
        return l - r;
    }
    void setHeight(){
        height = 0;
        if(left)
            height += left->height + 1;
        if(right && right->height + 1 > height)
            height = right->height + 1;
    }
    void setSons(){
        if(isEmpty)
            return;
        sonsCount = 1;
        if(left)
            sonsCount += left->sonsCount;
        if(right)
            sonsCount += right->sonsCount;
    }
    void smallLeftRotate(){
        AVLTree root = *right;
        *right = *root.left;
        *root.left = *this;
        *this = root;
        this->left->setHeight();
        this->left->setSons();
        this->setHeight();
        this->setSons();
    }
    void smallRightRotate(){
        AVLTree root = *left;
        *left = *root.right;
        *root.right = *this;
        *this = root;
        this->right->setHeight();
        this->right->setSons();
        this->setHeight();
        this->setSons();
    }
    void bigLeftRotate(){
        right->smallRightRotate();
        smallLeftRotate();
    }
    void bigRightRotate(){
        left->smallLeftRotate();
        smallRightRotate();
    }
    void balance(){
        /*std::cout << key << ":";
        std::cout << left->height << "," << right->height << " left - " << left->diff();
        std::cout << std::endl;*/
        setHeight();
        setSons();
        if(diff() == -2){
            if(right->diff() == -1 || right->diff() == 0)
                smallLeftRotate();
            else
                bigLeftRotate();
        }
        if(diff() == 2){
            if(left->diff() == 1 || left->diff() == 0)
                smallRightRotate();
            else
                bigRightRotate();
        }
    }
    void create(int new_key){
        isEmpty = false;
        key = new_key;
        AVLTree *l = new AVLTree();
        AVLTree *r = new AVLTree();
        sonsCount = 1;
        left = l; right = r;
        setHeight();
    }
    int findMin(){
        if(left->isEmpty) return key;
        return left->findMin();
    }
    void delMin(){
        if(left->isEmpty){
            *this = *right;
        }
        else{
            left->delMin();
        }
        balance();
    }
public:
    void print(){
        std::cout << "(" << key << "," << sonsCount << ") ";
        if(left){
            left->print();
        }
        if(right){
            right->print();
        }
    }
    void insert(int new_key){
        if(isEmpty){
            create(new_key);
        }
        if(new_key < key){
            left->insert(new_key);
        }
        if(new_key > key){
            right->insert(new_key);
        }
        balance();
    }
    void erase(int del_key){
        if(isEmpty){
            return;
        }
        if(del_key < key){
            left->erase(del_key);
        }
        if(del_key > key){
            right->erase(del_key);
        }
        if(del_key == key){
            if(right->isEmpty){
                *this = *left;
            }
            else{
                this->key = right->findMin();
                right->delMin();
            }
        }
        balance();
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

    int findKthMin(int k){
        if(left->sonsCount + 1 == k){
            return key;
        }
        if(left->sonsCount + 1 > k){
            return left->findKthMin(k);
        }
        if(left->sonsCount + 1 < k){
            return right->findKthMin(k - left->sonsCount - 1);
        }
    }
    int findKthMax(int k){
        return findKthMin(sonsCount - k + 1);
    }
};

int main() {
    //std::ios_base::sync_with_stdio(false);
    //std::cin.tie(0);
    //std::cout.tie(0);

    std::string s; int key, res;
    AVLTree tree;
    while(std::cin >> s){
        std::cin >> key;
        if(s[0] == 'i'){
            tree.insert(key);
            tree.print(); std::cout << "\n";
        }
        if(s[0] == 'd'){
            tree.erase(key);
            tree.print(); std::cout << "\n";
        }
        if(s[0] == 'e'){
            std::cout << (tree.exists(key)? "true" : "false") << "\n";
        }
        if(s[0] == 'n') {
            res = tree.next(key);
            if(res == 1000000009)
                std::cout << "none" << "\n";
            else
                std::cout << res << "\n";
        }
        if(s[0] == 'p'){
            res = tree.prev(key);
            if(res == -1000000009)
                std::cout << "none" << "\n";
            else
                std::cout << res << "\n";
        }
    }
    return 0;
}
