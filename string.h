#ifndef MYSTRING_STRING_H
#define MYSTRING_STRING_H
#include <iostream>
#include <cstring>

class String{
private:
    size_t size = 0;
    size_t maxSize = 0;
    char* str = nullptr;
    void increaseMaxSize(){
        maxSize *= 2;
        char* copy = new char[maxSize];
        memcpy(copy, str, size);
        delete[] str;
        str = copy;
    }
    void decreaseMaxSize(){
        maxSize = maxSize/2 + 1;
        char* copy = new char[maxSize];
        memcpy(copy, str, maxSize);
        delete[] str;
        str = copy;
    }
public:
    String(){
        size = 0;
        str = new char[1];
        str[0] = '\0';
        maxSize = 1;
    };

    String(size_t size, char c = '\0') : size(size), str(new char[size + 1]){
        maxSize = size * 2;
        memset(str, c, size);
    }

    String(const String& s): size(s.size), str(new char[size + 1]){
        maxSize = size * 2;
        memcpy(str, s.str, size);
    }

    String(const char* s){
        size = strlen(s);
        str = new char[size * 2];
        memcpy(str, s, size);
    }

    String& operator=(String s){
        swap(s);
        return *this;
    }

    void swap(String& s){
        std::swap(maxSize, s.maxSize);
        std::swap(size, s.size);
        std::swap(str, s.str);
    }

    size_t length() const{
        return size;
    }

    void push_back(const char& c){
        if(size >= maxSize){
            increaseMaxSize();
        }
        str[size] = c;
        ++size;
    }

    void pop_back(){
        if(size == 0){
            return;
        }
        size--;
        if(4 * size <= maxSize){
            decreaseMaxSize();
        }
    }

    char& back(){
        if(size > 0)
            return str[size - 1];
        return str[0];
    }

    const char& back() const{
        if(size > 0)
            return str[size - 1];
        return str[0];
    }

    char& front(){
        return str[0];
    }

    const char& front() const{
        return str[0];
    }

    size_t find(const String& substr) const{
        if(substr.size > size)
            return size;
        bool flag; size_t res = size;
        for(size_t i = 0; i <= size - substr.size; ++i){
            flag = true;
            for(size_t j = i; j < i + substr.size; ++j){
                if(str[j] != substr[j - i]){
                    flag = false;
                }
            }
            if(flag){
                return i;
            }
        }
        return size;
    }

    size_t rfind(const String& substr) const{
        if(substr.size > size)
            return size;
        bool flag; size_t res;
        for(size_t i = size - 1; i >= substr.size - 1; --i){
            flag = true;
            for(size_t j = i; j >= i - substr.size + 1; --j){
                if(str[j] != substr[j - i + substr.size - 1]){
                    flag = false;
                }
            }
            if(flag){
                return i - substr.size + 1;
            }
        }
        return size;
    }

    String substr(size_t start, size_t count) const{
        String res(count);
        memcpy(res.str, str + start, count);
        return res;
    }

    char& operator[](size_t index){
        if(index < size)
            return str[index];
        return str[0];
    }

    const char& operator[](size_t index) const{
        if(index < size)
            return str[index];
        return str[0];
    }

    String& operator+=(const char& c){
        push_back(c);
        return *this;
    }

    String& operator+=(const String& s){
        if(size + s.size > maxSize){
            maxSize = (size + s.size) * 2;
            char* copy = new char[maxSize];
            memcpy(copy, str, size);
            delete[] str;
            str = copy;
        }
        memcpy(str + size, s.str, s.size);
        size += s.size;
        return *this;
    }

    bool operator==(const String& s) const{
        if(size != s.size)
            return false;
        for(size_t i = 0; i < size; ++i){
            if(str[i] != s[i])
                return false;
        }
        return true;
    }

    bool empty() const{
        return size == 0;
    }

    void clear(){
        size = 0;
        maxSize = 1;
        delete[] str;
        str = new char[1];
    }

    ~String(){
        delete[] str;
    }
};

String operator+(const String& s1, const String& s2){
    String copy = s1;
    copy += s2;
    return copy;
}

std::ostream& operator<<(std::ostream& out, const String& s){
    for(size_t i = 0; i < s.length(); ++i){
        out << s[i];
    }
    return out;
}

std::istream& operator>>(std::istream& in, String& s){
    char c;
    s.clear();
    in.get(c);
    while(c != '\n' && c != ' ' && c != '\0'){
        s.push_back(c);
        if(!in.get(c))
            return in;
    }
    return in;
}


#endif //MYSTRING_STRING_H
