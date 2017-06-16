#ifndef MAP_HPP
#define MAP_HPP

#include <iostream>

namespace cs540{
    
const int MAX = 32;

template<typename Key_T, typename Mapped_T>
class BaseNode{
public:
    BaseNode* next[MAX + 1];
    BaseNode* prev[MAX + 1];
    BaseNode(){
        for(int i = 1; i <= MAX; i++){
            next[i] = nullptr;
            prev[i] = nullptr;
        }
    }
    virtual ~BaseNode(){
        
    }
    const virtual Key_T* getKey(){
        return nullptr;
    }
    virtual Mapped_T* getMap(){
        return nullptr;
    }
};

template<typename Key_T, typename Mapped_T>
class Node: public BaseNode<Key_T, Mapped_T>{
public:
    typedef std::pair<const Key_T, Mapped_T> ValueType;
    ValueType v;

    Node(Key_T K, Mapped_T M):v(K, M){
        for(int i = 1; i <= MAX; i++){
            BaseNode<Key_T, Mapped_T>::next[i] = nullptr;
            BaseNode<Key_T, Mapped_T>::prev[i] = nullptr;
        }
    }
    
    const Key_T* getKey(){
        return &v.first;
    }
    
    Mapped_T* getMap(){
        return &v.second;
    }
};

template<typename Key_T, typename Mapped_T>
class Map
{
public:
    typedef BaseNode<Key_T, Mapped_T> BaseNodeType;
    typedef Node<Key_T, Mapped_T> NodeType;
    BaseNodeType* head;
    BaseNodeType* tail;
    int clvl; //current level
    int elements; //number of elements in skiplist
    
    class Iterator{
    public:
        BaseNode<Key_T, Mapped_T>* itn;
        Map<Key_T, Mapped_T>* itm;
        
        Iterator():itn(nullptr), itm(nullptr){
            
        }
        
        Iterator(BaseNodeType* n, Map* m):itn(n), itm(m){
            
        }
        
        Iterator &operator++(){
            itn = itn->next[1];
            return *this;
        }
        
        Iterator operator++(int){
            Iterator it(*this);
            itn = itn->next[1];
            return it;
        }
        
        Iterator &operator--(){
            itn = itn->prev[1];
            return *this;
        }
        
        Iterator operator--(int){
            Iterator it(*this);
            itn = itn->prev[1];
            return it;
        }
        
        typename Node<Key_T, Mapped_T>::ValueType &operator*() const{
            return static_cast<NodeType*>(itn)->v;
        }
        
        typename Node<Key_T, Mapped_T>::ValueType *operator->() const{
            return &static_cast<NodeType*>(itn)->v;
        }
        
        friend bool operator==(const Iterator & it1, const Iterator & it2){
            if(it1.itm == it2.itm){
                if(it1.itn == it2.itn){
                    return true;
                }
            }
            return false;
        }
        
        friend bool operator!=(const Iterator & it1, const Iterator & it2){
            return !(it1 == it2);
        }
        
    };
    
    class ConstIterator: public Iterator{
    public:
        const Map* itm;
        
        ConstIterator():Iterator(){
            
        }
        
        ConstIterator(BaseNodeType* n, const Map* m):Iterator(n, nullptr), itm(m){
            
        }
        const typename Node<Key_T, Mapped_T>::ValueType &operator*() const{
            return static_cast<NodeType*>(Iterator::itn)->v;
        }
        const typename Node<Key_T, Mapped_T>::ValueType *operator->() const{
            return &static_cast<NodeType*>(Iterator::itn)->v;
        }
    };
    
    class ReverseIterator: public Iterator{
    public:
        Iterator &operator++(){
            Iterator::itn = Iterator::itn->prev[1];
            return *this;
        }
        
        Iterator operator++(int){
            Iterator retVal(*this);
            Iterator::itn = Iterator::itn->prev[1];
            return retVal;
        }
        
        Iterator &operator--(){
            Iterator::itn = Iterator::itn->next[1];
            return *this;
        }
        
        Iterator operator--(int){
            Iterator retVal(*this);
            Iterator::itn = Iterator::itn->next[1];
            return retVal;
        }
    };
    
    Map(){
        srand(time(NULL));
        clvl = 1;
        elements = 0;
        head = new BaseNodeType();
        tail = new BaseNodeType();
        for(int i = 1; i <= MAX; i++){
            head->next[i] = tail;
            tail->prev[i] = head;
        }
    }
    
    void clear(){
        if(head->next[1] != tail){
            BaseNodeType* curr = head->next[1];
            while(curr != tail){
                BaseNodeType* temp = curr;
                curr = curr->next[1];
                delete temp;
            }
        }
        clvl = 1;
        elements = 0;
        for(int i = 1; i <= MAX; i++){
            head->next[i] = tail;
            tail->prev[i] = head;
        }
    }
    
    ~Map(){
        clear();
        delete head;
        delete tail;
    }
    
    size_t size() const{
        return elements;
    }

    bool empty() const{
        return (head->next[1] == tail);
    }
    
    Iterator begin(){
        Iterator it;
        it.itm = this;
        it.itn = head->next[1];
        return it;
    }
    
    Iterator end(){
        Iterator it;
        it.itm = this;
        it.itn = tail;
        return it;
    }
     
    ConstIterator begin() const{
        ConstIterator it(head->next[1], this);
        return it;
    }
     
    ConstIterator end() const{
        ConstIterator it(tail, this);
        return it;
    }
    
    ReverseIterator rbegin(){
        ReverseIterator it;
        it.itm = this;
        it.itn = tail->prev[1];
        return it;
    }
     
    ReverseIterator rend(){
        ReverseIterator it;
        it.itm = this;
        it.itn = head;
        return it;
    }
    
    Iterator find(const Key_T & k){
        int i = clvl;
        BaseNodeType* curr = head;
        while(i > 0){
            if(curr->next[i] == tail){
                i--;
            }
            else{
                if(k < *curr->next[i]->getKey()){
                    i--;
                }
                else if(*curr->next[i]->getKey() == k){
                    Iterator it;
                    it.itm = this;
                    it.itn = curr->next[i];
                    return it;
                }
                else{
                    curr = curr->next[i];
                }
            }
        }
        return end();
    }
    
    ConstIterator find(const Key_T & k) const{
        int i = clvl;
        BaseNodeType* curr = head;
        while(i > 0){
            if(curr->next[i] == tail){
                i--;
            }
            else{
                if(k < *curr->next[i]->getKey()){
                    i--;
                }
                else if(*curr->next[i]->getKey() == k){
                    ConstIterator it;
                    it.itm = this;
                    it.itn = curr->next[i];
                    return it;
                }
                else{
                    curr = curr->next[i];
                }
            }
        }
        return end();
    }
    
    Mapped_T &at(const Key_T & k){
        int i = clvl;
        BaseNodeType* curr = head;
        while(i > 0){
            if(curr->next[i] == tail){
                i--;
            }
            else{
                if(k < *curr->next[i]->getKey()){
                    i--;
                }
                else if(*curr->next[i]->getKey() == k){
                    return *curr->next[i]->getMap();
                }
                else{
                    curr = curr->next[i];
                }
            }
        }
        throw std::out_of_range("Key does not exist");
    }
    
    const Mapped_T &at(const Key_T & k) const{
        int i = clvl;
        BaseNodeType* curr = head;
        while(i > 0){
            if(curr->next[i] == tail){
                i--;
            }
            else{
                if(k < *curr->next[i]->getKey()){
                    i--;
                }
                else if(*curr->next[i]->getKey() == k){
                    return *curr->next[i]->getMap();
                }
                else{
                    curr = curr->next[i];
                }
            }
        }
        throw std::out_of_range("Key does not exist");
    }
    
    Mapped_T &operator[](const Key_T & k){
        int i = clvl;
        BaseNodeType* curr = head;
        
        while(i > 0){
            if(curr->next[i] == tail){
                i--;
            }
            else{
                if(k < *curr->next[i]->getKey()){
                    i--;
                }
                else if(*curr->next[i]->getKey() == k){
                    return *curr->next[i]->getMap();
                }
                else{
                    curr = curr->next[i];
                }
            }
        }
        
        elements++;
        BaseNodeType* temp = curr->next[1];
        Mapped_T m = Mapped_T();
        BaseNodeType* newNode = new NodeType(k, m);
        newNode->prev[1] = curr;
        newNode->next[1] = temp;
        temp->prev[1] = newNode;
        curr->next[1] = newNode;
        int l = rand() % 32 + 1;
        if(l > clvl){
            clvl = l;
        }
        for(int j = 2; j <= l; j++){
            BaseNodeType* curr2 = head;
            while(curr2->next[j] != tail){
                if(k < *curr2->next[j]->getKey()){
                    break;
                }
                else{
                    curr2 = curr2->next[j];
                }
            }
            BaseNodeType* temp2 = curr2->next[j];
            curr2->next[j] = newNode;
            newNode->prev[j] = curr2;
            temp2->prev[j] = newNode;
            newNode->next[j] = temp2;
        }
        
        /*
        Mapped_T m = Mapped_T();
        elements++;
        BaseNodeType* temp = curr->next[1];
        curr->next[1] = new NodeType(k, m);
        curr->next[1]->prev[1] = curr;
        temp->prev[1] = curr->next[1];
        curr->next[1]->next[1] = temp;
        for(int i = 2; i <= MAX; i++){
            curr->next[1]->prev[i] = temp->prev[i];
            curr->next[1]->next[i] = curr->next[i];
        }
        int l = rand() % 32 + 1;
        if(l > clvl){
            clvl = l;
        }
        for(int i = 2; i <= l; i++){
            curr->next[1]->prev[i]->next[i] = curr->next[1];
            curr->next[1]->next[i]->prev[i] = curr->next[1];
        }
         */
        return *curr->next[1]->getMap();
    }

    std::pair<Iterator, bool> insert(const typename Node<Key_T, Mapped_T>::ValueType & v){
        Key_T k = v.first;
        Mapped_T m = v.second;
        int i = clvl;
        BaseNodeType* curr = head;
            
        while(i > 0){
            if(curr->next[i] == tail){
                i--;
            }
            else{
                if(k < *curr->next[i]->getKey()){
                    i--;
                }
                else if(*curr->next[i]->getKey() == k){
                    Iterator it;
                    it.itm = this;
                    it.itn = curr->next[i];
                    return std::make_pair(it, false);
                }
                else{
                    curr = curr->next[i];
                }
            }
        }
        elements++;
        BaseNodeType* temp = curr->next[1];
        BaseNodeType* newNode = new NodeType(k, m);
        newNode->prev[1] = curr;
        newNode->next[1] = temp;
        temp->prev[1] = newNode;
        curr->next[1] = newNode;
        int l = rand() % 32 + 1;
        if(l > clvl){
            clvl = l;
        }
        for(int j = 2; j <= l; j++){
            BaseNodeType* curr2 = head;
            while(curr2->next[j] != tail){
                if(k < *curr2->next[j]->getKey()){
                    break;
                }
                else{
                    curr2 = curr2->next[j];
                }
            }
            BaseNodeType* temp2 = curr2->next[j];
            curr2->next[j] = newNode;
            newNode->prev[j] = curr2;
            temp2->prev[j] = newNode;
            newNode->next[j] = temp2;
        }
        
        /*
        elements++;
        BaseNodeType* temp = curr->next[1];
        BaseNodeType* newNode = new NodeType(k, m);
        newNode->prev[1] = curr;
        newNode->next[1] = temp;
        temp->prev[1] = newNode;
        curr->next[1] = newNode;
        for(int i = 2; i <= MAX; i++){
            newNode->prev[i] = temp->prev[i];
            newNode->next[i] = curr->next[i];
        }
        int l = rand() % 32 + 1;
        if(l > clvl){
            clvl = l;
        }
        for(int i = 2; i <= l; i++){
            newNode->prev[i]->next[i] = newNode;
            newNode->next[i]->prev[i] = newNode;
        }
         */
        
        /*
        std::cout << l << std::endl;
        for(int i = clvl; i > 0; i--){
            BaseNodeType* test = head;
            while(test->next[i] != tail){
                std::cout << " " << *test->next[i]->getKey() << " ";
                test = test->next[i];
            }
            std::cout << std::endl;
        }
        std::cout << "============================" << std::endl;
        for(int i = clvl; i > 0; i--){
            BaseNodeType* test = tail;
            while(test->prev[i] != head){
                std::cout << " " << *test->prev[i]->getKey() << " ";
                test = test->prev[i];
            }
            std::cout << std::endl;
        }
        std::cout << "============================" << std::endl;
         */
        Iterator it;
        it.itm = this;
        it.itn = newNode;
        return std::make_pair(it, true);
    }
    
    template <typename IT_T>
    void insert(IT_T range_beg, IT_T range_end){
        for(IT_T it = range_beg; it != range_end; it++){
            insert(*it);
        }
    }
    
    Map(const Map & other){
        elements = 0;
        clvl = 1;
        head = new BaseNodeType();
        tail = new BaseNodeType();
        for(int i = 1; i <= MAX; i++){
            head->next[i] = tail;
            tail->prev[i] = head;
        }
        BaseNodeType* curr = other.head->next[1];
        while(curr != other.tail){
            insert(static_cast<NodeType*>(curr)->v);
            curr = curr->next[1];
        }
    }
    
    Map &operator=(const Map & other){
        if(this != &other){
            clear();
            BaseNodeType* curr = other.head->next[1];
            while(curr != other.tail){
                insert(static_cast<NodeType*>(curr)->v);
                curr = curr->next[1];
            }
        }
        return *this;
    }
    
    Map(std::initializer_list<std::pair<const Key_T, Mapped_T>> l): Map(){
        for(auto i = l.begin(); i != l.end(); i++){
            insert(*i);
        }
    }

    void erase(const Key_T & k){
        int i = clvl;
        BaseNodeType* curr = head;
        while(i > 0){
            if(curr->next[i] == tail){
                i--;
            }
            else{
                if(k < *curr->next[i]->getKey()){
                    i--;
                }
                else if(*curr->next[i]->getKey() == k){
                    elements--;
                    int lvl = i;
                    BaseNodeType* dNode = curr->next[i];
                    for(int j = lvl; j > 0; --j){
                    //for(int j = MAX; j > 0; --j){
                        BaseNodeType* temp = dNode->prev[j];
                        temp->next[j] = dNode->next[j];
                        dNode->next[j]->prev[j] = temp;
                    }
                    if(lvl == clvl){
                        while(head->next[lvl] == tail){
                            if(clvl == 1){
                                break;
                            }
                            lvl--;
                            clvl--;
                        }
                    }
                    delete dNode;
                    return;
                }
                else{
                    curr = curr->next[i];
                }
            }
        }
        throw std::out_of_range("Key does not exist");
    }
    
    void erase(Iterator pos){
        const Key_T k = *pos.itn->getKey();
        erase(k);
    }
};

template<typename Key_T, typename Mapped_T>
bool operator==(const Map<Key_T, Mapped_T> & m1, const Map<Key_T, Mapped_T> & m2){
    if(m1.size() == m2.size()){
        auto it1 = m1.begin();
        auto it2 = m2.begin();
        while((it1 != m1.end()) && (it2 != m2.end())){
            if(*it1 != *it2){
                return false;
            }
            it1++;
            it2++;
        }
        return true;
    }
    return false;
}

template<typename Key_T, typename Mapped_T>
bool operator!=(const Map<Key_T, Mapped_T> & m1, const Map<Key_T, Mapped_T> & m2){
    return !(m1 == m2);
}

template<typename Key_T, typename Mapped_T>
bool operator<(const Map<Key_T, Mapped_T> & m1, const Map<Key_T, Mapped_T> & m2){
    auto it1 = m1.begin();
    auto it2 = m2.begin();
    while((it1 != m1.end()) || (it2 != m2.end())){
        if(*it1 < *it2){
            return true;
        }
        it1++;
        it2++;
    }
    if(m1.size() < m2.size()){
        it1 = m1.begin();
        it2 = m2.begin();
        while((it1 != m1.end()) || (it2 != m2.end())){
            if(*it1 != *it2){
                return false;
            }
            it1++;
            it2++;
        }
        return true;
    }
    return false;
}
    
}
#endif
