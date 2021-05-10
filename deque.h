#include <iostream>
#include <vector>
#include <type_traits>

template<typename T>
class Deque{
private:
    using Alloc = std::allocator<T>;
    using AllocTraits = std::allocator_traits<Alloc>;

    static const size_t bucket_size = 4;
    std::vector<T*> buckets;
    std::pair<size_t, size_t> front_coord; // first - внешний индекс, second - внутренний
    std::pair<size_t, size_t> back_coord;
    size_t sz;
    Alloc alloc;

    void expand();
    size_t getFirstCoord(size_t) const;
    size_t getSecondCoord(size_t) const;
    size_t activeBuckets() const;
public:
    Deque();
    Deque(size_t, const T& value = T());
    Deque(const Deque<T>&);
    Deque& operator=(const Deque<T>&);

    size_t size() const;
    T& operator[](size_t);
    const T& operator[](size_t) const;
    T& at(size_t);
    const T& at(size_t) const;

    void push_back(const T&);
    //void push_back(T&& value);
    void push_front(const T&);
    //void push_front(T&& value);
    void pop_back();
    void pop_front();

    template<bool isConst>
    struct template_iterator{
        using conditionalPtr = typename std::conditional<isConst, const T*, T*>::type;
        using conditionalReference = typename std::conditional<isConst, const T&, T&>::type;
        using iteratorType = typename std::vector<T*>::const_iterator;
        using iterator_category = std::random_access_iterator_tag;
        using value_type = typename std::conditional<isConst, const T, T>::type;
        using difference_type = int;
        using pointer = typename std::conditional<isConst, const T*, T*>::type;
        using reference = typename std::conditional<isConst, const T&, T&>::type;
    private:
        iteratorType bucketIndex;
        int elemIndex = 0;
    public:
        template_iterator(iteratorType it, int n):bucketIndex(it), elemIndex(n){}
        template_iterator& operator++();
        template_iterator operator++(int);
        template_iterator& operator--();
        template_iterator operator--(int);
        template_iterator& operator+=(int);
        template_iterator& operator-=(int);
        template_iterator operator+(int) const;
        template_iterator operator-(int) const;
        bool operator<(const template_iterator<isConst>&) const;
        bool operator>(const template_iterator<isConst>&) const;
        bool operator<=(const template_iterator<isConst>&) const;
        bool operator>=(const template_iterator<isConst>&) const;
        bool operator==(const template_iterator<isConst>&) const;
        bool operator!=(const template_iterator<isConst>&) const;
        int operator-(const template_iterator<isConst>&) const;
        conditionalReference operator*(){
            return *((*bucketIndex) + elemIndex);
        }
        conditionalPtr operator->(){
            return *bucketIndex + elemIndex;
        }
    };
    using iterator = template_iterator<false>;
    using const_iterator = template_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cbegin() const;
    const_iterator cend() const;

    reverse_iterator rbegin();
    const_reverse_iterator rbegin() const;
    reverse_iterator rend();
    const_reverse_iterator rend() const;
    const_reverse_iterator crbegin() const;
    const_reverse_iterator crend() const;

    void insert(iterator it, const T& value);
    void erase(iterator);
};


//Iterator**************************************************************************************************************************


template<typename T>
template<bool isConst>
typename Deque<T>::template template_iterator<isConst>& Deque<T>::template_iterator<isConst>::operator++() {
    bucketIndex += (++elemIndex) / bucket_size;
    elemIndex %= bucket_size;
    return *this;
}

template<typename T>
template<bool isConst>
typename Deque<T>::template template_iterator<isConst> Deque<T>::template_iterator<isConst>::operator++(int) {
    template_iterator<isConst> copy = *this;
    ++(*this);
    return copy;
}

template<typename T>
template<bool isConst>
typename Deque<T>::template template_iterator<isConst>& Deque<T>::template_iterator<isConst>::operator--() {
    bucketIndex -= (--elemIndex < 0 ? 1:0);
    elemIndex = (bucket_size + elemIndex) % bucket_size;
    return *this;
}

template<typename T>
template<bool isConst>
typename Deque<T>::template template_iterator<isConst> Deque<T>::template_iterator<isConst>::operator--(int) {
    template_iterator<isConst> copy = *this;
    --(*this);
    return copy;
}

template<typename T>
template<bool isConst>
typename Deque<T>::template template_iterator<isConst>& Deque<T>::template_iterator<isConst>::operator+=(int n) {
    if(n >= 0) {
        bucketIndex += (elemIndex + n) / bucket_size;
        elemIndex = (elemIndex + n) % bucket_size;
    }
    else{
        n = -n;
        bucketIndex -= ((bucket_size - 1 - elemIndex) + n) / bucket_size;
        elemIndex = (bucket_size + (elemIndex - n) % bucket_size) % bucket_size;
    }
    return *this;
}

template<typename T>
template<bool isConst>
typename Deque<T>::template template_iterator<isConst>& Deque<T>::template_iterator<isConst>::operator-=(int n) {
    *this += (-n);
    return *this;
}
template<typename T>
template<bool isConst>
typename Deque<T>::template template_iterator<isConst> Deque<T>::template_iterator<isConst>::operator+(int n) const {
    template_iterator<isConst> copy = *this;
    copy += n;
    return copy;
}

template<typename T>
template<bool isConst>
typename Deque<T>::template template_iterator<isConst> Deque<T>::template_iterator<isConst>::operator-(int n) const {
    template_iterator<isConst> copy = *this;
    copy -= n;
    return copy;
}

template<typename T>
template<bool isConst>
bool Deque<T>::template_iterator<isConst>::operator<(const Deque<T>::template_iterator<isConst>& other) const {
    return (bucketIndex < other.bucketIndex) || (bucketIndex == other.bucketIndex && elemIndex < other.elemIndex);
}

template<typename T>
template<bool isConst>
bool Deque<T>::template_iterator<isConst>::operator==(const Deque<T>::template_iterator<isConst>& other) const {
    return bucketIndex == other.bucketIndex && elemIndex == other.elemIndex;
}

template<typename T>
template<bool isConst>
bool Deque<T>::template_iterator<isConst>::operator<=(const Deque<T>::template_iterator<isConst>& other) const {
    return *this == other || *this < other;
}

template<typename T>
template<bool isConst>
bool Deque<T>::template_iterator<isConst>::operator>=(const Deque<T>::template_iterator<isConst>& other) const {
    return !(*this < other);
}

template<typename T>
template<bool isConst>
bool Deque<T>::template_iterator<isConst>::operator>(const Deque<T>::template_iterator<isConst>& other) const {
    return !(*this <= other);
}

template<typename T>
template<bool isConst>
bool Deque<T>::template_iterator<isConst>::operator!=(const Deque<T>::template_iterator<isConst>& other) const {
    return !(*this == other);
}

template<typename T>
template<bool isConst>
int Deque<T>::template_iterator<isConst>::operator-(const Deque<T>::template_iterator<isConst>& other) const {
    int ans = 0;
    if(*this > other){
        ans += bucket_size - other.elemIndex;
        ans += (bucketIndex - other.bucketIndex - 1) * bucket_size;
        ans += elemIndex;
    }
    else{
        ans += bucket_size - elemIndex;
        ans += (other.bucketIndex - bucketIndex - 1) * bucket_size;
        ans += other.elemIndex;
        ans = -ans;
    }
    return ans;
}



//Begin-End***********************************************************************************************************

template<typename T>
typename Deque<T>::iterator Deque<T>::begin() {
    iterator res(buckets.begin() + front_coord.first, front_coord.second);
    return res;
}

template<typename T>
typename Deque<T>::const_iterator Deque<T>::cbegin() const{
    const_iterator res(buckets.cbegin() + front_coord.first, front_coord.second);
    return res;
}

template<typename T>
typename Deque<T>::const_iterator Deque<T>::begin() const{
    return cbegin();
}

template<typename T>
typename Deque<T>::iterator Deque<T>::end() {
    iterator res(buckets.begin() + back_coord.first + (back_coord.second == bucket_size - 1), (back_coord.second + 1) % bucket_size);
    return res;
}

template<typename T>
typename Deque<T>::const_iterator Deque<T>::cend() const{
    const_iterator res(buckets.cbegin() + back_coord.first + (back_coord.second == bucket_size - 1), (back_coord.second + 1) % bucket_size);
    return res;
}

template<typename T>
typename Deque<T>::const_iterator Deque<T>::end() const{
    return cend();
}

template<typename T>
typename Deque<T>::reverse_iterator Deque<T>::rbegin() {
    return std::make_reverse_iterator(end());
}

template<typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::crbegin() const{
    return std::make_reverse_iterator(cend());
}

template<typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rbegin() const{
    return crbegin();
}

template<typename T>
typename Deque<T>::reverse_iterator Deque<T>::rend() {
    return std::make_reverse_iterator(begin());
}

template<typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::crend() const{
    return std::make_reverse_iterator(cbegin());
}

template<typename T>
typename Deque<T>::const_reverse_iterator Deque<T>::rend() const{
    return crend();
}


//Deque**********************************************************************************************************************
template<typename T>
void Deque<T>::expand() {
    size_t n = buckets.size();
    buckets.resize(n * 3, nullptr);
    size_t l = n + n / 2 - activeBuckets() / 2;
    size_t r = l;
    for(size_t i = front_coord.first; i <= back_coord.first; ++i, ++r){
        buckets[r] = buckets[i];
        buckets[i] = nullptr;
    }
    front_coord.first = l;
    back_coord.first = r - 1;
}

template<typename T>
size_t Deque<T>::getFirstCoord(size_t i) const{
    return ((i + front_coord.second) / bucket_size) + front_coord.first;
}

template<typename T>
size_t Deque<T>::getSecondCoord(size_t i) const{
    return (i + front_coord.second) % bucket_size;
}

template<typename T>
size_t Deque<T>::activeBuckets() const{
    return back_coord.first - front_coord.first + 1;
}

template<typename T>
Deque<T>::Deque() : buckets(std::vector<T*>(3, nullptr)), sz(0){
    front_coord.first = back_coord.first = 1;
    buckets[1] = AllocTraits::allocate(alloc, bucket_size);
    front_coord.second = bucket_size / 2;
    back_coord.second = bucket_size / 2 - 1;
}

template<typename T>
Deque<T>::Deque(size_t n, const T& value) : buckets(std::vector<T*>((n / bucket_size + 1) * 3, nullptr)), sz(n) {
    front_coord.first = back_coord.first = buckets.size() / 3;
    front_coord.second = back_coord.second = 0;
    size_t i = 0, j = 0;
    try {
        for (i = 0; i < n;) {
            buckets[back_coord.first] = AllocTraits::allocate(alloc, bucket_size);
            for (j = 0; j < bucket_size && i < n; ++j, ++i) {
                AllocTraits::construct(alloc, buckets[back_coord.first] + j, value);
            }
            if (i < n)
                ++back_coord.first;
            else
                back_coord.second = j - 1;
        }
    }
    catch(...){
        for(size_t g = 0; g <= i;){
            for(size_t k = 0; k < bucket_size && g < i; ++g, ++k){
                AllocTraits::destroy(alloc, buckets[front_coord.first] + g);
            }
            AllocTraits::deallocate(alloc, buckets[front_coord.first], bucket_size);
            ++front_coord.first;
        }
    }
}

template<typename T>
Deque<T>::Deque(const Deque<T>& deq): front_coord(deq.front_coord), back_coord(deq.back_coord), sz(deq.size()){
    buckets = std::vector<T*>(deq.buckets.size(), nullptr);
    size_t i = front_coord.first;
    try {
        for (; i <= back_coord.first; ++i) {
            buckets[i] = AllocTraits::allocate(alloc, bucket_size);
            std::uninitialized_copy(deq.buckets[i], deq.buckets[i]+bucket_size, buckets[i]);
        }
    }
    catch(...){
        for(size_t g = front_coord.first; g <= i; ++g){
            AllocTraits::deallocate(alloc, buckets[g], bucket_size);
        }
        throw;
    }
}

template<typename T>
Deque<T>& Deque<T>::operator=(const Deque<T>& deq) {
    for(size_t i = 0; i < sz; ++i){
        AllocTraits::destroy(alloc, buckets[getFirstCoord(i)]+getSecondCoord(i));
    }
    for(size_t i = front_coord.first; i <= back_coord.first; ++i){
        AllocTraits::deallocate(alloc, buckets[i], bucket_size);
    }
    front_coord = deq.front_coord;
    back_coord = deq.back_coord;
    sz = deq.size();
    size_t i = front_coord.first;
    try{
        buckets.reserve(deq.buckets.size());
        for(; i <= back_coord.first; ++i){
            buckets[i] = AllocTraits::allocate(alloc, bucket_size);
            std::uninitialized_copy(deq.buckets[i], deq.buckets[i]+bucket_size, buckets[i]);
        }
    }
    catch(...){
        for(size_t g = front_coord.first; g <= i; ++g){
            AllocTraits::deallocate(alloc, buckets[g], bucket_size);
        }
        throw;
    }
    return *this;
}

template<typename T>
size_t Deque<T>::size() const {
    return sz;
}

template<typename T>
T &Deque<T>::operator[](size_t i) {
    return buckets[getFirstCoord(i)][getSecondCoord(i)];
}

template<typename T>
const T &Deque<T>::operator[](size_t i) const {
    return buckets[getFirstCoord(i)][getSecondCoord(i)];
}

template<typename T>
T &Deque<T>::at(size_t i) {
    if (i >= sz)
        throw std::out_of_range("Out of range");
    return (*this)[i];
}

template<typename T>
const T &Deque<T>::at(size_t i) const {
    if (i >= sz)
        throw std::out_of_range("Out of range");
    return (*this)[i];
}

template<typename T>
void Deque<T>::push_back(const T& value) {
    if(back_coord.first + 1 == buckets.size() && back_coord.second + 1 == bucket_size){
        expand();
    }
    if(back_coord.second < bucket_size - 1){
        ++back_coord.second;
    }
    else{
        buckets[++back_coord.first] = AllocTraits::allocate(alloc, bucket_size);
        back_coord.second = 0;
    }
    ++sz;
    try {
        AllocTraits::construct(alloc, buckets[back_coord.first] + back_coord.second, value);
    }
    catch(...){
        --sz;
    }
}

template<typename T>
void Deque<T>::push_front(const T& value) {
    if(front_coord.first == 0 && front_coord.second == 0){
        expand();
    }
    if(front_coord.second > 0){
        --front_coord.second;
    }
    else{
        buckets[--front_coord.first] = AllocTraits::allocate(alloc, bucket_size);
        front_coord.second = bucket_size - 1;
    }
    ++sz;
    try {
        AllocTraits::construct(alloc, buckets[front_coord.first] + front_coord.second, value);
    }
    catch(...){
        --sz;
    }
}

template<typename T>
void Deque<T>::pop_back() {
    AllocTraits::destroy(alloc,  buckets[back_coord.first]+back_coord.second);
    if(back_coord.second == 0){
        back_coord.second = bucket_size - 1;
        AllocTraits::deallocate(alloc, buckets[back_coord.first], bucket_size);
        buckets[back_coord.first] = nullptr;
        --back_coord.first;
    }
    else{
        --back_coord.second;
    }
    --sz;

}

template<typename T>
void Deque<T>::pop_front() {
    AllocTraits::destroy(alloc,  buckets[front_coord.first]+front_coord.second);
    if(front_coord.second == bucket_size - 1){
        front_coord.second = 0;
        AllocTraits::deallocate(alloc, buckets[front_coord.first], bucket_size);
        buckets[front_coord.first] = nullptr;
        ++front_coord.first;
    }
    else{
        ++front_coord.second;
    }
    --sz;
}

template<typename T>
void Deque<T>::insert(Deque<T>::iterator it, const T &value) {
    push_back(value);
    for(auto j = --end(); j != it; --j){
        *j = *(j - 1);
    }
    *it = value;
}

template<typename T>
void Deque<T>::erase(Deque<T>::iterator it) {
    for(; it + 1 != end(); ++it)
        *it = *(it + 1);
    pop_back();
}

