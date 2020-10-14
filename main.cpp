#include <iostream>
#include <string>

class MyStack{
private:
    struct stackCell{
        int value;
        stackCell *previous;
    };
    stackCell *top;
    int size;
public:
    MyStack(){
        this->top = nullptr;
        this->size = 0;
    }
    bool isEmpty(){
        if(this->size == 0) {
            return true;
        }
        return false;
    }
    void push(int value){
        stackCell *newTop = new stackCell();
        newTop->value = value;
        newTop->previous = top;
        this->top = newTop;
        this->size++;
    }
    void pop(){
        if(this->isEmpty()) {
            return;
        }
        stackCell *deletableTop = this->top;
        this->top = deletableTop->previous;
        delete deletableTop;
        this->size--;
    }
    int getTopValue(){
        if(this->isEmpty()) {
            return -1;
        }
        return this->top->value;
    }
    int getSize(){
        return this->size;
    }
    void clear(){
        while(this->size != 0){
            this->pop();
        }
    }
};

int main() {
    MyStack stack;
    int value;
    std::string query;
    std::cin >> query;
    while(query != "exit"){
        if(query == "push"){
            std::cin >> value;
            stack.push(value);
            std::cout << "ok" << std::endl;
        }
        if(query == "back" || query == "pop"){
            value = stack.getTopValue();
            if(stack.getSize() == 0){
                std::cout << "error" << std::endl;
            }
            else{
                std::cout << value << std:: endl;
                if(query == "pop"){
                    stack.pop();
                }
            }
        }
        if(query == "size"){
            std::cout << stack.getSize() << std::endl;
        }
        if(query == "clear"){
            stack.clear();
            std::cout << "ok" << std::endl;
        }
        std::cin >> query;
    }
    std::cout << "bye";
    return 0;
}
