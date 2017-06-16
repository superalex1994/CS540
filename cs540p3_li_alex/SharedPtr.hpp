#ifndef SHAREDPTR_HPP
#define SHAREDPTR_HPP

#include <cstddef>
#include <mutex>
#include <assert.h>

namespace cs540{
    
    class BaseHelper{
    public:
        std::mutex m;
        unsigned count;
        BaseHelper(){
            count = 1;
        }
        virtual ~BaseHelper(){
            
        }
    };
    
    template <typename U> class Helper: public BaseHelper{
    public:
        U* pu;
        Helper(U* p){
            pu = p;
        }
        virtual ~Helper(){
            delete pu;
        }
    };
    
    template <typename T> class SharedPtr{
    private:
        void dec(){
            if(ptr != nullptr){
                pa->m.lock();
                pa->count = pa->count - 1;
                assert(pa->count >= 0);
                if(pa->count == 0){
                    pa->m.unlock();
                    delete pa;
                    ptr = nullptr;
                    pa = nullptr;
                }
                else{
                    pa->m.unlock();
                }
            }
        }
        void inc(){
            if(ptr != nullptr){
                pa->m.lock();
                pa->count = pa->count + 1;
                pa->m.unlock();
            }
        }
    public:
        mutable T* ptr;
        BaseHelper* pa;
        SharedPtr(){
            ptr = nullptr;
            pa = nullptr;
        }
        template <typename U> explicit SharedPtr(U *p){
            ptr = p;
            pa = new Helper<U>(p);
        }
        SharedPtr(const SharedPtr &p){
            if(p.ptr != nullptr){
                ptr = p.ptr;
                pa = p.pa;
                inc();
            }
            else{
                ptr = nullptr;
                pa = nullptr;
            }
        }
        template <typename U> SharedPtr(const SharedPtr<U> &p){
            if(p.ptr != nullptr){
                ptr = p.ptr;
                pa = p.pa;
                inc();
            }
            else{
                ptr = nullptr;
                pa = nullptr;
            }
        }
        template <typename U> SharedPtr(U* p1, BaseHelper* p2){
            if(p1 != nullptr){
                ptr = static_cast<T*>(p1);
                pa = p2;
                inc();
            }
        }
        SharedPtr(SharedPtr &&p){
            ptr = p.ptr;
            pa = p.pa;
            p.ptr = nullptr;
            p.pa = nullptr;
            
        }
        template <typename U> SharedPtr(SharedPtr<U> &&p){
            ptr = p.ptr;
            pa = p.pa;
            p.ptr = nullptr;
            p.pa = nullptr;
        }
        SharedPtr &operator=(const SharedPtr &p){
            if (this->pa != p.pa){
                dec();
                ptr = p.ptr;
                pa = p.pa;
                inc();
            }
            return *this;
        }
        template <typename U>
        SharedPtr<T> &operator=(const SharedPtr<U> &p){
	    if (this->pa != p.pa){
       		dec();
        	ptr = p.ptr;
       		pa = p.pa;
        	inc();
	    }
	    return *this;
        }
        SharedPtr &operator=(SharedPtr &&p){
            if (this->pa != p.pa){
                dec();
                ptr = p.ptr;
                pa = p.pa;
                p.ptr = nullptr;
                p.pa = nullptr;
	    }
            return *this;
        }
        template <typename U> SharedPtr &operator=(SharedPtr<U> &&p){
            if (this->pa != p.pa){
                dec();
                ptr = p.ptr;
                pa = p.pa;
                p.ptr = nullptr;
                p.pa = nullptr;
	    }
	    return *this;
        }
        ~SharedPtr(){
            dec();
            ptr = nullptr;
            pa = nullptr;
        }
        void reset(){
            dec();
            ptr = nullptr;
            pa = nullptr;
        }
        template <typename U> void reset(U *p){
            dec();
            ptr = p;
            pa = new Helper<U>(p);
        }
        T *get() const{
            return ptr;
        }
        T &operator*() const{
            return *ptr;
        }
        T *operator->() const{
            return ptr;
        }
        explicit operator bool() const{
            if(ptr == nullptr){
                return false;
            }
            return true;
        }
    };
    template <typename T1, typename T2>
    bool operator==(const SharedPtr<T1> &p1, const SharedPtr<T2> &p2){
        if(p1.ptr == p2.ptr){
            return true;
        }
        return false;
    }
    
    template <typename T>
    bool operator==(const SharedPtr<T> &p, std::nullptr_t){
        if(p.ptr == nullptr){
            return true;
        }
        return false;
    }
    
    template <typename T>
    bool operator==(std::nullptr_t, const SharedPtr<T> &p){
        if(p.ptr == nullptr){
            return true;
        }
        return false;
    }
    
    template <typename T1, typename T2>
    bool operator!=(const SharedPtr<T1>& p1, const SharedPtr<T2> & p2){
        if(p1.ptr != p2.ptr){
            return true;
        }
        return false;
    }
    
    template <typename T>
    bool operator!=(const SharedPtr<T> &p, std::nullptr_t){
        if(p.ptr == nullptr){
            return false;
        }
        return true;
    }
    
    template <typename T>
    bool operator!=(std::nullptr_t, const SharedPtr<T> &p){
        if(p.ptr == nullptr){
            return false;
        }
        return true;
    }
    
    template <typename T, typename U>
    SharedPtr<T> static_pointer_cast(const SharedPtr<U> &sp){
        SharedPtr<T> temp(sp.ptr, sp.pa);
        return temp;
    }
    
    template <typename T, typename U>
    SharedPtr<T> dynamic_pointer_cast(const SharedPtr<U> &sp){
        if(dynamic_cast<T*>(sp.ptr) != nullptr){
            SharedPtr<T> temp(sp.ptr, sp.pa);
            return temp;
        }
        else{
            SharedPtr<T> temp;
            return temp;
        }
    }
}

#endif
