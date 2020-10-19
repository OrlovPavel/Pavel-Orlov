#include <iostream>
#include <deque>

class MyQueue {
private:
    std::deque<int> firstPart;
    std::deque<int> secondPart;
public:
    void pushBack(int i){
        this->firstPart.push_back(i);
        this->balance();
    }
    void pushCenter(int i){
        this->firstPart.push_front(i);
        this->balance();
    }
    void balance(){
        int firstSize = this->firstPart.size();
        int secondSize =this->secondPart.size();
        if(firstSize != secondSize){
            if(secondSize - firstSize >= 2){
                this->firstPart.push_front(this->secondPart.back());
                this->secondPart.pop_back();
            }
            if(firstSize > secondSize){
                this->secondPart.push_back(this->firstPart.front());
                this->firstPart.pop_front();
            }
        }
    }
    int pop(){
        int res = 0;
        res = this->secondPart.front();
        this->secondPart.pop_front();
        this->balance();
        return res;
    }
};

int main() {
    char query;
    MyQueue queue;
    int n, i;
    std::cin >> n;
    for(int j = 0; j < n; ++j){
        std:: cin >> query;
        if(query == '+'){
            std::cin >> i;
            queue.pushBack(i);
        }
        if(query == '*'){
            std::cin >> i;
            queue.pushCenter(i);
        }
        if(query == '-'){
            std::cout << queue.pop() << std::endl;
        }
    }
    return 0;
}
