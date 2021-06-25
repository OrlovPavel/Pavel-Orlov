#include <vector>
#include <iostream>
#include <memory>


template<typename T, typename Alloc = std::allocator<T>>
class List {
public:
    struct Node {
        T* value = nullptr;
        Node *next = nullptr;
        Node *prev = nullptr;
    };

    using AllocTraits = std::allocator_traits<Alloc>;
    Node *first = nullptr;
    Node *last = nullptr;
    size_t sz = 0;
    Alloc alloc;
public:
    explicit List(const Alloc &alloc = Alloc());

    explicit List(size_t count, const T &value, const Alloc &alloc = Alloc());

    explicit List(size_t count);

    List(const List &other);

    List(List&& other) noexcept;

    List &operator=(const List &other);

    List &operator=(List&& other) noexcept;

    size_t size() const;

    Alloc get_allocator() const;

    void push_back(const T &value);

    void push_back(T&& value);

    void push_front(const T &value);

    void push_front(T&& value);

    void push_front(T* value){
        Node* node = new Node;
        node->value = value;
        node->prev = first;
        node->next = first->next;
        first->next->prev = node;
        first->next = node;
        ++sz;
    }

    void pop_back();

    void pop_front();

    ~List();

    //******************************************************************************************************************8
    template<bool isConst>
    struct template_iterator {
        using conditionalPtr = typename std::conditional<isConst, const T *, T *>::type;
        using conditionalReference = typename std::conditional<isConst, const T &, T &>::type;
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = std::conditional<isConst, const T, T>;
        using difference_type = int;
        using pointer = conditionalPtr;
        using reference = conditionalReference;
        friend List;
        friend template_iterator<true>;
        friend template_iterator<false>;
    private:
        Node *current = nullptr;
    public:
        template_iterator() = default;
        template<bool U> ///!!!!!!!!
        template_iterator(const template_iterator<U> &other) : current(other.current) {}

        explicit template_iterator(Node *node) : current(node) {}

        template_iterator &operator++() {
            current = current->next;
            return *this;
        }

        template_iterator<isConst> operator++(int) {
            template_iterator<isConst> copy = *this;
            ++(*this);
            return copy;
        }

        template_iterator &operator--() {
            current = current->prev;
            return *this;
        }

        template_iterator operator--(int) {
            template_iterator<isConst> copy = *this;
            --(*this);
            return copy;
        }

        bool operator==(const template_iterator<isConst> &other) const {
            return current == other.current;
        }

        bool operator!=(const template_iterator<isConst> &other) const {
            return current != other.current;
        }

        conditionalReference operator*() {
            return *current->value;
        }

        conditionalPtr operator->() {
            return current->value;
        }
    };

    using iterator = template_iterator<false>;
    using const_iterator = template_iterator<true>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    iterator begin() {
        iterator res(first->next);
        return res;
    }

    iterator end() {
        return iterator(last);
    }

    const_iterator cbegin() const {
        return const_iterator(first->next);
    }

    const_iterator cend() const {
        return const_iterator(last);
    }

    const_iterator begin() const {
        return cbegin();
    }

    const_iterator end() const {
        return cend();
    }

    reverse_iterator rbegin() {
        return std::make_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return std::make_reverse_iterator(begin());
    }

    const_reverse_iterator crbegin() const {
        return std::make_reverse_iterator(cend());
    }

    const_reverse_iterator crend() const {
        return std::make_reverse_iterator(cbegin());
    }

    const_reverse_iterator rbegin() const {
        return crbegin();
    }

    const_reverse_iterator rend() const {
        return crend();
    }

    void insert(iterator it, const T &value) {
        Node *node = new Node;// Нельзя использовать в данном моменте new, для этого есть аллокатор. Просто выдели всю память
        // для ноды сразу через алоцирование, а конструируй только для value
        node->value = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, node->value, value);
        node->next = it.current;
        node->prev = it.current->prev;
        if (it.current->prev)
            it.current->prev->next = node;
        it.current->prev = node;
        ++sz;
    }

    void insert(const_iterator it, const T &value) {
        Node *node = new Node;// Аналогично
        node->value = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, node->value, value);
        node->next = it.current;
        node->prev = it.current->prev;
        if (it.current->prev)
            it.current->prev->next = node;
        it.current->prev = node;
        ++sz;
    }

    void push_front(iterator it){
        Node *node = it.current;
        node->prev = first;
        node->next = first->next;
        first->next->prev = node;
        first->next = node;
        ++sz;
    }

    template<typename... Args>
    void emplace_front(Args&&... args){
        Node* node = new Node;
        node->value = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, node->value, std::forward<Args>(args)...);
        node->prev = first;
        node->next = first->next;
        first->next->prev = node;
        first->next = node;
        ++sz;
    }

    void erase(iterator it) {
        Node *node = it.current;
        if (node->prev)
            node->prev->next = node->next;
        if (node->next)
            node->next->prev = node->prev;
        AllocTraits::destroy(alloc, node->value);
        AllocTraits::deallocate(alloc, node->value, 1);
        delete node;
        --sz;
    }

    void erase(const_iterator it) {
        Node *node = it.current;
        if (node->prev)
            node->prev->next = node->next;
        if (node->next)
            node->next->prev = node->prev;
        AllocTraits::destroy(alloc, node->value);
        AllocTraits::deallocate(alloc, node->value, 1);
        delete node;
        --sz;
    }
};



//********************************************************************************************************

template<typename T, typename Alloc>
List<T, Alloc>::List(const Alloc& alloc1) : alloc(AllocTraits::select_on_container_copy_construction(alloc1)) {
    first = new Node;
    last = new Node;
    first->next = last;
    last->prev = first;
}

template<typename T, typename Alloc>
List<T, Alloc>::List(size_t count, const T &value, const Alloc& alloc1) : sz(count), alloc(AllocTraits::select_on_container_copy_construction(alloc1)) {
    first = new Node;
    Node *current = nullptr;
    Node *prev = first;
    for (size_t i = 0; i < sz; ++i) {
        current = new Node;
        current->value = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, current->value, value);
        prev->next = current;
        current->prev = prev;
        prev = current;
    }
    last = new Node;
    prev->next = last;
    last->prev = current;
}


template<typename T, typename Alloc>
List<T, Alloc>::List(size_t count) : sz(count), alloc(Alloc()) {
    first = new Node;
    Node *current = nullptr;
    Node *prev = first;
    for (size_t i = 0; i < sz; ++i) {
        current = new Node;
        current->value = AllocTraits::allocate(alloc, 1);
        AllocTraits::construct(alloc, current);
        prev->next = current;
        current->prev = prev;
        prev = current;
    }
    last = new Node;
    prev->next = last;
    last->prev = current;
}

template<typename T, typename Alloc>
List<T, Alloc>::List(const List &other) : sz(other.sz),
                                          alloc(AllocTraits::select_on_container_copy_construction(other.alloc)) {
    Node *other_node = other.first->next;
    first = new Node;
    Node *current = nullptr;
    Node *prev = first;
    for (size_t i = 0; i < sz; ++i) {
        current = new Node;
        current->value = AllocTraits::allocate(alloc, 1);
        std::uninitialized_copy(other_node->value, other_node->value + 1, current->value);
        prev->next = current;
        current->prev = prev;
        prev = current;
        other_node = other_node->next;
    }
    last = new Node;
    prev->next = last;
    last->prev = current;
}

template<typename T, typename Alloc>
List<T, Alloc> &List<T, Alloc>::operator=(const List<T, Alloc> &other) {
    if (this == &other) return *this;
    Node *current = first;
    Node *prev = nullptr;
    while (current) {
        prev = current;
        current = current->next;
        if (prev != first && prev != last) {
            AllocTraits::destroy(alloc, prev->value);
            AllocTraits::deallocate(alloc, prev->value, 1);
        }
        delete prev;
    }
    //std::cout << count << "!\n";
    if (AllocTraits::propagate_on_container_copy_assignment::value && &alloc != &other.alloc) {
        alloc = other.alloc;
    }
    Node *other_node = other.first->next;
    first = new Node;
    current = nullptr;
    prev = first;
    sz = other.sz;
    for (size_t i = 0; i < sz; ++i) {
        current = new Node;
        current->value = AllocTraits::allocate(alloc, 1);
        std::uninitialized_copy(other_node->value, other_node->value + 1, current->value);
        prev->next = current;
        current->prev = prev;
        prev = current;
        other_node = other_node->next;
    }
    last = new Node;
    prev->next = last;
    last->prev = current;
    return *this;
}


template<typename T, typename Alloc>
void List<T, Alloc>::push_back(const T &value) {
    Node *node = new Node;
    node->value = AllocTraits::allocate(alloc, 1);
    AllocTraits::construct(alloc, node->value, value);
    node->next = last;
    node->prev = last->prev;
    last->prev->next = node;
    last->prev = node;
    ++sz;
}

template<typename T, typename Alloc>
void List<T, Alloc>::push_back(T &&value) {
    Node *node = new Node;
    node->value = AllocTraits::allocate(alloc, 1);
    AllocTraits::construct(alloc, node->value, std::move(value));
    node->next = last;
    node->prev = last->prev;
    last->prev->next = node;
    last->prev = node;
    ++sz;
}

template<typename T, typename Alloc>
void List<T, Alloc>::push_front(const T &value) {
    Node *node = new Node;
    node->value = AllocTraits::allocate(alloc, 1);
    AllocTraits::construct(alloc, node->value, value);
    node->prev = first;
    node->next = first->next;
    first->next->prev = node;
    first->next = node;
    ++sz;
}

template<typename T, typename Alloc>
void List<T, Alloc>::push_front(T &&value) {
    Node *node = new Node;
    node->value = AllocTraits::allocate(alloc, 1);
    AllocTraits::construct(alloc, node->value, std::move(value));
    node->prev = first;
    node->next = first->next;
    first->next->prev = node;
    first->next = node;
    ++sz;
}

template<typename T, typename Alloc>
void List<T, Alloc>::pop_back() {
    Node *node = last->prev;
    last->prev->prev->next = last;
    last->prev = last->prev->prev;
    AllocTraits::destroy(alloc, node->value);
    AllocTraits::deallocate(alloc, node->value, 1);
    delete node;
    --sz;
}

template<typename T, typename Alloc>
void List<T, Alloc>::pop_front() {
    Node *node = first->next;
    first->next->next->prev = first;
    first->next = first->next->next;
    AllocTraits::destroy(alloc, node->value);
    AllocTraits::deallocate(alloc, node->value, 1);
    delete node;
    --sz;
}

template<typename T, typename Alloc>
size_t List<T, Alloc>::size() const {
    return sz;
}

template<typename T, typename Alloc>
Alloc List<T, Alloc>::get_allocator() const {
    return alloc;
}

template<typename T, typename Alloc>
List<T, Alloc>::~List() {
    Node *current = first;
    Node *prev = nullptr;
    while (current) {
        prev = current;
        current = current->next;
        if (prev != first && prev != last) {
            AllocTraits::destroy(alloc, prev->value);
            AllocTraits::deallocate(alloc, prev->value, 1);
        }
        delete prev;
    }
}

template<typename T, typename Alloc>
List<T, Alloc>::List(List&& other) noexcept : first(other.first), last(other.last), sz(other.sz), alloc(std::move(other.alloc)){
    other.first = new Node;
    other.last = new Node;
    other.first->next = last;
    other.last->prev = first;
    other.sz = 0;
}

template<typename T, typename Alloc>
List<T, Alloc> &List<T, Alloc>::operator=(List &&other) noexcept{
    List copy = std::move(other);
    std::swap(*this, copy);
    return *this;
}

//**********************************************************************************************************************

template<typename Key, typename Value, typename Hash = std::hash<Key>,
        typename Equal = std::equal_to<Key>, typename Alloc = std::allocator<std::pair<const Key, Value>>>
class UnorderedMap{
public:
    using NodeType = std::pair<const Key, Value>;
    using AllocTraits = std::allocator_traits<Alloc>;
private:
    struct Bucket{
        Bucket* prev = nullptr;
        Bucket* next = nullptr;
        List<NodeType, Alloc> chain;
        Bucket(const Alloc& alloc = Alloc()) : chain(alloc) {}
    };
    using BucketAlloc = std::allocator<Bucket>;
    using BucketAllocTraits = std::allocator_traits<BucketAlloc>;
    std::vector<Bucket*> buckets;
    Bucket* first = nullptr;
    Bucket* last = nullptr;
    Hash hash_func;
    Equal is_equal;
    Alloc alloc;
    BucketAlloc bucket_alloc;
    size_t sz = 0;
    float ml = 0.6;
    void push_bucket(Bucket* bucket);
    bool needRehash(){return float(sz) >= float(buckets.size()) * ml;}
public:
    UnorderedMap(size_t bucket_count, const Hash& hash = Hash(),
                 const Equal& equal = Equal(), const Alloc& alloc = Alloc()) :
            buckets(bucket_count, nullptr), hash_func(hash), is_equal(equal), alloc(AllocTraits::select_on_container_copy_construction(alloc)){
        first = BucketAllocTraits::allocate(bucket_alloc, 1);
        last = BucketAllocTraits::allocate(bucket_alloc, 1);
        first->next = last;
        first->prev = nullptr;
        last->prev = first;
        last->next = nullptr;
    }
    UnorderedMap() : UnorderedMap(10){}
    explicit UnorderedMap(const Alloc& alloc) : UnorderedMap(10, Hash(), Equal(), alloc){}
    UnorderedMap(const UnorderedMap& other);
    UnorderedMap(UnorderedMap&& other);

    UnorderedMap& operator=(const UnorderedMap& other);
    UnorderedMap& operator=(UnorderedMap&& other);

    size_t size();

    Value& operator[](const Key& key);
    Value& operator[](Key&& key);
    Value& at(const Key& key) const;

    //***********************************************************************************************************
    template<bool isConst>
    struct template_iterator{
        using conditionalPtr = typename std::conditional<isConst, const NodeType*, NodeType*>::type;
        using conditionalReference = typename std::conditional<isConst, const NodeType&, NodeType&>::type;
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::conditional<isConst, const NodeType, NodeType>;
        using difference_type = int;
        using pointer = conditionalPtr;
        using reference = conditionalReference;
        using ListIterator = typename List<NodeType, Alloc>::template template_iterator<isConst>;
        friend template_iterator<true>;
        friend template_iterator<false>;
    private:
        Bucket* bucket;
        ListIterator node;
    public:
        template<bool U>
        template_iterator(const template_iterator<U>& other) : bucket(other.bucket), node(other.node){}
        explicit template_iterator(Bucket* bucket) : bucket(bucket), node(ListIterator()) {}
        template<bool U>
        explicit template_iterator(Bucket* bucket, const typename List<NodeType, Alloc>::template template_iterator<U>& node) : bucket(bucket), node(node) {}

        template_iterator &operator++() {
            ++node;
            if(node == bucket->chain.end()){
                bucket = bucket->next;
                if(bucket->next)
                    node = bucket->chain.begin();
                else
                    node = ListIterator();
            }
            return *this;
        }

        template_iterator<isConst> operator++(int) {
            template_iterator<isConst> copy = *this;
            ++(*this);
            return copy;
        }

        bool operator==(const template_iterator<isConst> &other) const {
            return bucket == other.bucket && node == other.node;
        }

        bool operator!=(const template_iterator<isConst> &other) const {
            return bucket != other.bucket || node != other.node;
        }

        conditionalReference operator*(){
            return *node;
        }

        conditionalPtr operator->(){
            return &(*node);
        }
    };

    using iterator = template_iterator<false>;
    using const_iterator = template_iterator<true>;

    iterator begin() {
        return iterator(first->next, first->next->chain.begin());
    }

    const_iterator cbegin() const{
        return const_iterator(first->next, first->next->chain.cbegin());
    }

    iterator end() {
        return iterator(last);
    }

    const_iterator cend() const {
        return const_iterator(last);
    }

    const_iterator begin() const{
        return cbegin();
    }

    const_iterator end() const{
        return cend();
    }

    //****************************************************************************************************************

    std::pair<iterator, bool> insert(const NodeType& elem);
    std::pair<iterator, bool> insert(NodeType&& elem);
    template<typename iterator_type>
    void insert(iterator_type first1, iterator_type last1);

    template<typename... Args>
    std::pair<iterator, bool> emplace(Args&&... args);

    template<typename P>
    std::pair<iterator, bool> insert(P&& value){
        return emplace(std::forward<P>(value));
    }

    void erase(iterator pos);
    void erase(const_iterator first1, const_iterator last1);

    iterator find(const Key& key);
    const_iterator find(const Key& key) const;

    void reserve(size_t count);

    size_t max_size() const;
    float load_factor() const;
    float max_load_factor() const;
    void max_load_factor(float ml1);

    ~UnorderedMap();
};

//************************************************************************************************************

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::UnorderedMap(const UnorderedMap &other) :
        UnorderedMap(other.buckets.size(), other.hash_func, other.is_equal, other.alloc){
    sz = other.sz;
    ml = other.ml;
    first = BucketAllocTraits::allocate(bucket_alloc, 1);
    last = BucketAllocTraits::allocate(bucket_alloc, 1);
    first->next = last;
    first->prev = nullptr;
    last->prev = first;
    last->next = nullptr;
    for(size_t i = 0; i < buckets.size(); ++i){
        if(other.buckets[i]){
            buckets[i] = BucketAllocTraits::allocate(bucket_alloc, 1);
            //BucketAllocTraits::construct(bucket_alloc, buckets[i], *other.buckets[i]);
            std::uninitialized_copy(other.buckets[i], other.buckets[i] + 1, buckets[i]);
            push_bucket(buckets[i]);
        }
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::UnorderedMap(UnorderedMap &&other) :
        buckets(std::move(other.buckets)), first(other.first), last(other.last),
        hash_func(std::move(other.hash_func)), is_equal(std::move(other.is_equal)),
        alloc(other.alloc), bucket_alloc(other.bucket_alloc),
        sz(other.sz), ml(other.ml){
    other.sz = 0;
    other.first = BucketAllocTraits::allocate(other.bucket_alloc, 1);
    other.last = BucketAllocTraits::allocate(other.bucket_alloc, 1);
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc> &UnorderedMap<Key, Value, Hash, Equal, Alloc>::operator=(const UnorderedMap &other) {
    if(this == & other) return *this;
    for(size_t i = 0; i < buckets.size(); ++i){
        if(buckets[i]){
            BucketAllocTraits::destroy(bucket_alloc, buckets[i]);
            BucketAllocTraits::deallocate(bucket_alloc, buckets[i], 1);
            buckets[i] = nullptr;
        }
    }
    buckets.resize(other.buckets.size(), nullptr);
    hash_func = other.hash_func;
    is_equal = other.is_equal;
    if(AllocTraits::propagate_on_container_copy_assignment::value && alloc != other.alloc)
        alloc = other.alloc;
    if(BucketAllocTraits::propagate_on_container_copy_assignment::value && bucket_alloc != other.bucket_alloc)
        bucket_alloc = other.bucket_alloc;
    sz = other.sz;
    ml = other.ml;
    first->next = last;
    last->prev = first;
    for(size_t i = 0; i < buckets.size(); ++i){
        if(other.buckets[i]){
            buckets[i] = BucketAllocTraits::allocate(bucket_alloc, 1);
            //BucketAllocTraits::construct(bucket_alloc, buckets[i], *other.buckets[i]);
            std::uninitialized_copy(other.buckets[i], other.buckets[i] + 1, buckets[i]);
            push_bucket(buckets[i]);
        }
    }
    return *this;
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc> &UnorderedMap<Key, Value, Hash, Equal, Alloc>::operator=(UnorderedMap &&other) {
    for(size_t i = 0; i < buckets.size(); ++i){
        if(buckets[i]){
            BucketAllocTraits::destroy(bucket_alloc, buckets[i]);
            BucketAllocTraits::deallocate(bucket_alloc, buckets[i], 1);
            buckets[i] = nullptr;
        }
    }
    buckets = std::move(other.buckets);
    hash_func = std::move(other.hash_func);
    is_equal = std::move(other.is_equal);
    if(AllocTraits::propagate_on_container_move_assignment::value && alloc != other.alloc)
        alloc = other.alloc;
    if(BucketAllocTraits::propagate_on_container_move_assignment::value && bucket_alloc != other.bucket_alloc)
        bucket_alloc = other.bucket_alloc;
    sz = other.sz;
    ml = other.ml;
    first = other.first;
    last = other.last;
    other.sz = 0;
    other.first = BucketAllocTraits::allocate(other.bucket_alloc, 1);
    other.last = BucketAllocTraits::allocate(other.bucket_alloc, 1);
    return *this;
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
Value &UnorderedMap<Key, Value, Hash, Equal, Alloc>::operator[](const Key &key) {// Много кода повторяется... можно было бы выделить
    size_t i = hash_func(key) % buckets.size();
    if(buckets[i]) {
        for (auto it = buckets[i]->chain.begin(); it != buckets[i]->chain.end(); ++it) {
            if (is_equal((*it).first, key))
                return (*it).second;
        }
        ++sz;
        if(needRehash()) {
            reserve(float(buckets.size()) / ml);
            i = hash_func(key) % buckets.size();
        }
        buckets[i]->chain.push_front(NodeType(key, Value()));
        return (*(buckets[i]->chain.begin())).second;
    }
    ++sz;
    if(needRehash()) {
        reserve(float(buckets.size()) / ml);
        i = hash_func(key) % buckets.size();
    }
    buckets[i] = BucketAllocTraits::allocate(bucket_alloc, 1);
    BucketAllocTraits::construct(bucket_alloc, buckets[i], alloc);
    buckets[i]->chain.push_front(NodeType(key, Value()));
    push_bucket(buckets[i]);
    return (*(buckets[i]->chain.begin())).second;
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
Value &UnorderedMap<Key, Value, Hash, Equal, Alloc>::operator[](Key &&key) {
    size_t i = hash_func(key) % buckets.size();
    if(buckets[i]) {
        for (auto it = buckets[i]->chain.begin(); it != buckets[i]->chain.end(); ++it) {
            if (is_equal((*it).first, key))
                return (*it).second;
        }
        ++sz;
        if(needRehash()) {
            reserve( float(buckets.size()) / ml);
            i = hash_func(key) % buckets.size();
        }
        buckets[i]->chain.push_front(NodeType(std::move(key), Value()));
        return (*(buckets[i]->chain.begin())).second;
    }
    ++sz;
    if(needRehash()) {
        reserve(float(buckets.size()) / ml);
        i = hash_func(key) % buckets.size();
    }
    buckets[i] = BucketAllocTraits::allocate(bucket_alloc, 1);
    BucketAllocTraits::construct(bucket_alloc, buckets[i], alloc);
    buckets[i]->chain.push_front(NodeType(std::move(key), Value()));
    push_bucket(buckets[i]);
    return (*(buckets[i]->chain.begin())).second;
}


template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
Value &UnorderedMap<Key, Value, Hash, Equal, Alloc>::at(const Key &key) const {// Какая то каша, не const элемент возвращается
// в const методе. По идее их 2 должно быть, один const ...at() const, второй просто at
    size_t i = hash_func(key) % buckets.size();
    if(buckets[i]){
        for (auto it = buckets[i]->chain.begin(); it != buckets[i]->chain.end(); ++it) {
            if (is_equal((*it).first, key))
                return (*it).second;
        }
        throw std::out_of_range("Out of range");
    }
    throw  std::out_of_range("Out of range");
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
size_t UnorderedMap<Key, Value, Hash, Equal, Alloc>::size() {
    return sz;
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::push_bucket(UnorderedMap::Bucket *bucket) {
    Bucket* prev = last->prev;
    prev->next = bucket;
    bucket->prev = prev;
    last->prev = bucket;
    bucket->next = last;
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
std::pair<typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::iterator, bool>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::insert(const UnorderedMap::NodeType &elem) {
    size_t i = hash_func(elem.first) % buckets.size();
    if(buckets[i]){
        for(auto it = buckets[i]->chain.begin(); it != buckets[i]->chain.end(); ++it) {
            if (is_equal(elem.first, (*it).first))
                return std::make_pair(iterator(buckets[i], it), false);
        }
        ++sz;
        if(needRehash()) {
            reserve(float(buckets.size()) / ml);
            i = hash_func(elem.first) % buckets.size();
        }
        buckets[i]->chain.push_front(elem);
        return std::make_pair(iterator(buckets[i], buckets[i]->chain.begin()), true);
    }
    ++sz;
    if(needRehash()) {
        reserve(float(buckets.size()) / ml);
        i = hash_func(elem.first) % buckets.size();
    }
    buckets[i] = BucketAllocTraits::allocate(bucket_alloc, 1);
    BucketAllocTraits::construct(bucket_alloc, buckets[i], alloc);
    buckets[i]->chain.push_front(elem);
    push_bucket(buckets[i]);
    return std::make_pair(iterator(buckets[i], buckets[i]->chain.begin()), true);
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
std::pair<typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::iterator, bool>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::insert(UnorderedMap::NodeType &&elem) {
    size_t i = hash_func(elem.first) % buckets.size();
    if(buckets[i]){
        for(auto it = buckets[i]->chain.begin(); it != buckets[i]->chain.end(); ++it) {
            if (is_equal(elem.first, (*it).first))
                return std::make_pair(iterator(buckets[i], it), false);
        }
        ++sz;
        if(needRehash()) {
            reserve(float(buckets.size()) / ml);
            i = hash_func(elem.first) % buckets.size();
        }
        buckets[i]->chain.push_front(std::move(elem));
        return std::make_pair(iterator(buckets[i], buckets[i]->chain.begin()), true);
    }
    ++sz;
    if(needRehash()) {
        reserve(float(buckets.size()) / ml);
        i = hash_func(elem.first) % buckets.size();
    }
    buckets[i] = BucketAllocTraits::allocate(bucket_alloc, 1);
    BucketAllocTraits::construct(bucket_alloc, buckets[i], alloc);
    buckets[i]->chain.push_front(std::move(elem));
    push_bucket(buckets[i]);
    return std::make_pair(iterator(buckets[i], buckets[i]->chain.begin()), true);
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
template<typename iterator_type>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::insert(iterator_type first1, iterator_type last1) {
    for(auto it = first1; it != last1; ++it){
        insert(*it);
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
template<typename... Args>
std::pair<typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::iterator, bool> UnorderedMap<Key, Value, Hash, Equal, Alloc>::emplace(Args &&... args) {
    NodeType* elem = AllocTraits::allocate(alloc, 1);
    AllocTraits::construct(alloc, elem, std::forward<Args>(args)...);
    size_t i = hash_func((*elem).first) % buckets.size();
    if(buckets[i]){
        for(auto it = buckets[i]->chain.begin(); it != buckets[i]->chain.end(); ++it) {
            if (is_equal((*elem).first, (*it).first)) {
                AllocTraits::destroy(alloc, elem);
                AllocTraits::deallocate(alloc, elem, 1);
                return std::make_pair(iterator(buckets[i], it), false);
            }
        }
        ++sz;
        if(needRehash()) {
            reserve(float(buckets.size()) / ml);
            i = hash_func((*elem).first) % buckets.size();
        }
        buckets[i]->chain.push_front(elem);
        return std::make_pair(iterator(buckets[i], buckets[i]->chain.begin()), true);
    }
    ++sz;
    if(needRehash()) {
        reserve(float(buckets.size()) / ml);
        i = hash_func((*elem).first) % buckets.size();
    }
    buckets[i] = BucketAllocTraits::allocate(bucket_alloc, 1);
    BucketAllocTraits::construct(bucket_alloc, buckets[i], alloc);
    buckets[i]->chain.push_front(elem);
    push_bucket(buckets[i]);
    return std::make_pair(iterator(buckets[i], buckets[i]->chain.begin()), true);
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::erase(UnorderedMap::iterator pos) {
    size_t i = hash_func((*pos).first) % buckets.size();
    if(buckets[i]){
        for(auto it = buckets[i]->chain.begin(); it != buckets[i]->chain.end(); ++it) {
            if (is_equal((*pos).first, (*it).first)) {
                buckets[i]->chain.erase(it);
                --sz;
                break;
            }
        }
        if(buckets[i]->chain.size() == 0){
            buckets[i]->prev->next = buckets[i]->next;
            buckets[i]->next->prev = buckets[i]->prev;
            BucketAllocTraits::destroy(bucket_alloc, buckets[i]);
            BucketAllocTraits::deallocate(bucket_alloc, buckets[i], 1);
            buckets[i] = nullptr;
        }
    }
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::erase(UnorderedMap::const_iterator first1, UnorderedMap::const_iterator last1) {
    for(auto it = first1; it != last1;)
        erase(it++);
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::iterator UnorderedMap<Key, Value, Hash, Equal, Alloc>::find(const Key &key) {
    size_t i = hash_func(key) % buckets.size();
    if(buckets[i]){
        for(auto it = buckets[i]->chain.begin(); it != buckets[i]->chain.end(); ++it) {
            if (is_equal(key, (*it).first))
                return iterator(buckets[i], it);
        }
        return end();
    }
    return end();
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
typename UnorderedMap<Key, Value, Hash, Equal, Alloc>::const_iterator UnorderedMap<Key, Value, Hash, Equal, Alloc>::find(const Key &key) const {
    size_t i = hash_func(key) % buckets.size();
    if(buckets[i]){
        for(auto it = buckets[i]->chain.begin(); it != buckets[i]->chain.end(); ++it) {
            if (is_equal(key, (*it).first))
                return const_iterator(buckets[i], it);
        }
        return cend();
    }
    return cend();
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::reserve(size_t count) {
    std::vector<Bucket*> temp(count, nullptr);
    first->next = last;
    last->prev = first;
    for(size_t i = 0; i < buckets.size(); ++i){
        if(buckets[i]){
            for(auto it = buckets[i]->chain.begin(); it != buckets[i]->chain.end();) {
                size_t j = hash_func((*it).first) % count;
                if(temp[j]){
                    temp[j]->chain.push_front(it++);
                }
                else{
                    temp[j] = BucketAllocTraits::allocate(bucket_alloc, 1);
                    BucketAllocTraits::construct(bucket_alloc, temp[j], alloc);
                    temp[j]->chain.push_front(it++);
                    push_bucket(temp[j]);
                }
            }
            delete buckets[i]->chain.first;
            delete buckets[i]->chain.last;
            BucketAllocTraits::deallocate(bucket_alloc, buckets[i], 1);
            buckets[i] = nullptr;
        }
    }
    buckets = std::move(temp);
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
size_t UnorderedMap<Key, Value, Hash, Equal, Alloc>::max_size() const {
    return 4000000000;
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
float UnorderedMap<Key, Value, Hash, Equal, Alloc>::load_factor() const {
    return float(sz) / float(buckets.size());
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
float UnorderedMap<Key, Value, Hash, Equal, Alloc>::max_load_factor() const {
    return ml;
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
void UnorderedMap<Key, Value, Hash, Equal, Alloc>::max_load_factor(float ml1) {
    ml = ml1;
}

template<typename Key, typename Value, typename Hash, typename Equal, typename Alloc>
UnorderedMap<Key, Value, Hash, Equal, Alloc>::~UnorderedMap() {
    for(size_t i = 0; i < buckets.size(); ++i){
        if(buckets[i]){
            BucketAllocTraits::destroy(bucket_alloc, buckets[i]);
            BucketAllocTraits::deallocate(bucket_alloc, buckets[i], 1);
        }
    }
    BucketAllocTraits::deallocate(bucket_alloc, first, 1);
    BucketAllocTraits::deallocate(bucket_alloc, last, 1);
}