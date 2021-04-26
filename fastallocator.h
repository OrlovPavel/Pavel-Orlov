#include <iostream>
#include <memory>
#include <list>
#include <ctime>

int chtobi_zashlo1 = 0;
template<size_t chunkSize>
class FixedAllocator{
private:
    size_t capacity;
    size_t size;
    void** chunks;
    void updateChunks(){
        delete[] chunks;
        chunks = new void*[2 * capacity];
        size = capacity;
        for(size_t i = 0; i < size; ++i)
            chunks[i] = ::operator new(chunkSize);
        capacity *= 2;
    }
public:
    FixedAllocator(){
        capacity = 10000;
        size = capacity;
        chunks = new void*[capacity];
        for(size_t i = 0; i < capacity; ++i)
            chunks[i] = ::operator new(chunkSize);
    }

    FixedAllocator(const FixedAllocator<chunkSize>& other){
        capacity = other.capacity;
        size = capacity;
        chunks = new void*[capacity];
        for(size_t i = 0; i < capacity; ++i)
            chunks[i] = ::operator new(chunkSize);
    }

    FixedAllocator& operator=(const FixedAllocator<chunkSize>& other){
        if(this == &other) return *this;
        capacity = other.capacity;
        size = capacity;
        chunks = new void*[capacity];
        for(size_t i = 0; i < capacity; ++i)
            chunks[i] = ::operator new(chunkSize);
        return *this;
    }

    void* allocate(){
        if(size == 0){
            updateChunks();
        }
        return chunks[--size];
    }

    void deallocate(void* p){
        chunks[size++] = p;
    }

    ~FixedAllocator(){
        for(size_t i = 0; i < size; ++i){
            ::operator delete(chunks[i], chunkSize);
        }
        delete[] chunks;
    }
};

template<typename T>
class FastAllocator{
public:
    //FixedAllocator<24> byte24;
    FixedAllocator<16> byte16;
    FixedAllocator<8> byte8;
    //FixedAllocator<4> byte4;
    friend FastAllocator<typename std::remove_pointer<T>::type>;
public:
    using value_type = T;

    FastAllocator() = default;
    FastAllocator(const FastAllocator<typename std::remove_pointer<T>::type>& other):/*byte24(other.byte24)*/ byte16(other.byte16), byte8(other.byte8) /*byte4(other.byte4)*/{}

    T* allocate(size_t n){
        //if(n * sizeof(T) == 24)
        //return static_cast<T*>(byte24.allocate());
        if(n * sizeof(T) == 16)
            return static_cast<T*>(byte16.allocate());
        if(n * sizeof(T) == 8)
            return static_cast<T*>(byte8.allocate());
        //if(n * sizeof(T) == 4)
        //return static_cast<T*>(byte4.allocate());
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    void deallocate(T* p, size_t n){
        /*if(n * sizeof(T) == 24) {
            byte24.deallocate(p);
            return;
        }*/
        if(n * sizeof(T) == 16) {
            byte16.deallocate(p);
            return;
        }
        if(n * sizeof(T) == 8) {
            byte8.deallocate(p);
            return;
        }
        /*if(n * sizeof(T) == 4) {
            byte4.deallocate(p);
            return;
        }*/
        ::operator delete(p);
    }

    FastAllocator<T> select_on_container_copy_construction(){
        FastAllocator<T> copy = *this;
        return copy;
    }

    template<typename U>
    struct rebind { using other =  FastAllocator<U>; };
};


//*******************************************************************************************************************

template<typename T, typename Allocator = std::allocator<T>>
class List{
public:
    struct Node{
        T value;
        Node* next = nullptr;
        Node* prev = nullptr;
        Node() = default;
        Node(const T& val):value(val){}
        Node(const Node& other) : value(other.value), next(nullptr), prev(nullptr){}
    };
    using Alloc = typename std::allocator_traits<Allocator>::template rebind_alloc<Node>;
    using AllocTraits = std::allocator_traits<Alloc>;
    Node* first = nullptr;
    Node* last = nullptr;
    size_t sz = 0;
    Alloc alloc;
public:
    explicit List(const Allocator& alloc = Allocator());
    explicit List(size_t count, const T& value, const Allocator& alloc = Allocator());
    explicit List(size_t count);
    List(const List& other);
    List& operator=(const List& other);

    size_t size() const;
    Allocator get_allocator()const ;

    void push_back(const T& value);
    void push_front(const T& value);
    void pop_back();
    void pop_front();

    ~List();

    //******************************************************************************************************************8
    template<bool isConst>
    struct template_iterator{
        using conditionalPtr = typename std::conditional<isConst, const T*, T*>::type;
        using conditionalReference = typename std::conditional<isConst, const T&, T&>::type;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::conditional<isConst, const T, T>;
        using difference_type = int;
        using pointer = conditionalPtr;
        using reference = conditionalReference;
        friend List;
        friend template_iterator<true>;
        friend template_iterator<false>;
    private:
        Node* current = nullptr;
    public:
        template_iterator(const template_iterator<false>& other):current(other.current){}
        explicit template_iterator(Node* node) : current(node){}
        template_iterator& operator++(){
            current = current->next;
            return *this;
        }
        template_iterator<isConst> operator++(int){
            template_iterator<isConst> copy = *this;
            ++(*this);
            return copy;
        }
        template_iterator& operator--() {
            current = current->prev;
            return *this;
        }
        template_iterator operator--(int){
            template_iterator<isConst> copy = *this;
            --(*this);
            return copy;
        }
        bool operator==(const template_iterator<isConst>& other) const{
            return current == other.current;
        }
        bool operator!=(const template_iterator<isConst>& other) const{
            return current != other.current;
        }
        conditionalReference operator*(){
            return current->value;
        }
        conditionalPtr operator->(){
            return &(current->value);
        }
    };
    using iterator = template_iterator<false>;
    using const_iterator = template_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin(){
        iterator res(first->next);
        return res;
    }
    iterator end() {
        return iterator(last);
    }
    const_iterator cbegin() const{
        return const_iterator(first->next);
    }
    const_iterator cend() const{
        return const_iterator(last);
    }
    const_iterator begin() const{
        return cbegin();
    }
    const_iterator end() const{
        return cend();
    }
    reverse_iterator rbegin(){
        return std::make_reverse_iterator(end());
    }
    reverse_iterator rend(){
        return std::make_reverse_iterator(begin());
    }
    const_reverse_iterator crbegin() const{
        return std::make_reverse_iterator(cend());
    }
    const_reverse_iterator crend() const{
        return std::make_reverse_iterator(cbegin());
    }
    const_reverse_iterator rbegin() const{
        return crbegin();
    }
    const_reverse_iterator rend() const{
        return crend();
    }

    void insert(iterator it, const T& value){
        Node* node = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, node, value);
        node->next = it.current;
        node->prev = it.current->prev;
        if(it.current->prev)
            it.current->prev->next = node;
        it.current->prev = node;
        ++sz;
    }

    void insert(const_iterator it, const T& value){
        Node* node = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, node, value);
        node->next = it.current;
        node->prev = it.current->prev;
        if(it.current->prev)
            it.current->prev->next = node;
        it.current->prev = node;
        ++sz;
    }

    void erase(iterator it){
        Node* node = it.current;
        if(node->prev)
            node->prev->next = node->next;
        if(node->next)
            node->next->prev = node->prev;
        AllocTraits::destroy(alloc, node);
        AllocTraits ::deallocate(alloc, node, 1);
        --sz;
    }

    void erase(const_iterator it){
        Node* node = it.current;
        if(node->prev)
            node->prev->next = node->next;
        if(node->next)
            node->next->prev = node->prev;
        AllocTraits::destroy(alloc, node);
        AllocTraits ::deallocate(alloc, node, 1);
        --sz;
    }
};


//********************************************************************************************************
template<typename T, typename Allocator>
List<T, Allocator>::List(const Allocator &) : alloc(Alloc()){
    first = AllocTraits::allocate(alloc, 1);
    last = AllocTraits::allocate(alloc, 1);
    first->next = last;
    first->prev = nullptr;
    last->prev = first;
    last->next = nullptr;
}

template<typename T, typename Allocator>
List<T, Allocator>::List(size_t count, const T &value, const Allocator &) : sz(count), alloc(Alloc()){
    first = AllocTraits::allocate(alloc, 1);
    first->prev = nullptr;
    Node* current = nullptr;
    Node* prev = first;
    for(size_t i = 0; i < sz; ++i){
        current = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, current, value);
        prev->next = current;
        current->prev = prev;
        prev = current;
    }
    last = AllocTraits::allocate(alloc, 1);
    prev->next = last;
    last->prev = current;
    last->next = nullptr;
}

template<typename T, typename Allocator>
List<T, Allocator>::List(size_t count) : sz(count), alloc(Alloc()){
    first = AllocTraits::allocate(alloc, 1);
    first->prev = nullptr;
    Node* current = nullptr;
    Node* prev = first;
    for(size_t i = 0; i < sz; ++i){
        current = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, current);
        prev->next = current;
        current->prev = prev;
        prev = current;
    }
    last = AllocTraits::allocate(alloc, 1);
    prev->next = last;
    last->prev = current;
    last->next = nullptr;
}

template<typename T, typename Allocator>
List<T, Allocator>::List(const List& other) : sz(other.sz), alloc(AllocTraits::select_on_container_copy_construction(other.alloc)){
    Node* other_node = other.first->next;
    first = AllocTraits::allocate(alloc, 1);
    first->prev = nullptr;
    Node* current = nullptr;
    Node* prev = first;
    for(size_t i = 0; i < sz; ++i){
        current = AllocTraits::allocate(alloc, 1);
        std::uninitialized_copy(other_node, other_node + 1, current);
        prev->next = current;
        current->prev = prev;
        prev = current;
        other_node = other_node->next;
    }
    last = AllocTraits::allocate(alloc, 1);
    prev->next = last;
    last->prev = current;
    last->next = nullptr;
}

template<typename T, typename Allocator>
List<T, Allocator>& List<T, Allocator>::operator=(const List<T, Allocator>& other) {
    if(this == &other) return *this;
    Node* current = first;
    Node* prev = nullptr;
    while(current){
        prev = current;
        current = current->next;
        if(prev != first && prev != last)
            AllocTraits::destroy(alloc, prev);
        AllocTraits::deallocate(alloc, prev, 1);
    }
    //std::cout << count << "!\n";
    if(AllocTraits::propagate_on_container_copy_assignment::value && &alloc != &other.alloc){
        alloc = other.alloc;
    }
    Node* other_node = other.first->next;
    first = AllocTraits::allocate(alloc, 1);
    first->prev = nullptr;
    current = nullptr;
    prev = first;
    sz = other.sz;
    for(size_t i = 0; i < sz; ++i){
        current = AllocTraits::allocate(alloc, 1);
        std::uninitialized_copy(other_node, other_node + 1, current);
        prev->next = current;
        current->prev = prev;
        prev = current;
        other_node = other_node->next;
    }
    last = AllocTraits::allocate(alloc, 1);
    prev->next = last;
    last->prev = current;
    last->next = nullptr;
    return *this;
}


template<typename T, typename Allocator>
void List<T, Allocator>::push_back(const T &value) {
    Node* node = AllocTraits::allocate(alloc, 1);
    AllocTraits::construct(alloc, node, value);
    node->next = last;
    node->prev = last->prev;
    last->prev->next = node;
    last->prev = node;
    ++sz;
}

template<typename T, typename Allocator>
void List<T, Allocator>::push_front(const T &value) {
    Node* node = AllocTraits::allocate(alloc, 1);
    AllocTraits::construct(alloc, node, value);
    node->prev = first;
    node->next = first->next;
    first->next->prev = node;
    first->next = node;
    ++sz;
}

template<typename T, typename Allocator>
void List<T, Allocator>::pop_back() {
    Node* node = last->prev;
    last->prev->prev->next = last;
    last->prev = last->prev->prev;
    AllocTraits::destroy(alloc, node);
    AllocTraits::deallocate(alloc, node, 1);
    --sz;
}

template<typename T, typename Allocator>
void List<T, Allocator>::pop_front() {
    Node* node = first->next;
    first->next->next->prev = first;
    first->next = first->next->next;
    AllocTraits::destroy(alloc, node);
    AllocTraits::deallocate(alloc, node, 1);
    --sz;
}

template<typename T, typename Allocator>
size_t List<T, Allocator>::size() const{
    return sz;
}

template<typename T, typename Allocator>
Allocator List<T, Allocator>::get_allocator() const{
    return alloc;
}

template<typename T, typename Allocator>
List<T, Allocator>::~List() {
    Node* current = first;
    Node* prev = nullptr;
    while(current){
        prev = current;
        current = current->next;
        if(prev != first && prev != last)
            AllocTraits::destroy(alloc, prev);
        AllocTraits::deallocate(alloc, prev, 1);
    }
}
