#ifndef SHARED_PTR_H
#define SHARED_PTR_H
#include <stdlib.h>
#include <utility>

template <typename T>
class Shared_ptr;

// todo : 完整版的make_shared()
// namespace smart_ptr {
//     template<typename T, typename... Args>
//     Shared_ptr<T> make_shared(Args&&... args) {
//         void* mem = malloc(sizeof(T) + sizeof(int));
//         T* t = static_cast<T*>(mem);
//         char* cnt_ptr = static_cast<char*>(mem);
//         cnt_ptr += sizeof(T);
//         int* use_count = (int*)cnt_ptr;
//         *use_count = 0;
//         ::new (t) T(std::forward<Args>(args)...);

//         return Shared_ptr<T>(use_count, t);
//     }
// }

template <typename T>
class Shared_ptr {
public:
    Shared_ptr(T* ptr)
        : use_count_(new int(0)),
          ptr_(ptr) {
        ++(*use_count_);
    };

    // for make_shared()
    // Shared_ptr(int* use_count, T* const ptr)
    //     : use_count_(use_count),
    //       ptr_(ptr) {
    //     ++(*use_count_);
    // };
    
    Shared_ptr(const Shared_ptr<T>& that)
        : use_count_(that.use_count_),
          ptr_(that.ptr_) {
        ++(*use_count_);
    }

    Shared_ptr& operator= (const Shared_ptr<T>& that) {
        use_count_ = that.use_count_;
        ++use_count_;
        ptr_ = that.use_count_;
    }

    ~Shared_ptr() {
        if(--(*use_count_) == 0) {
            delete ptr_;
            delete use_count_;
        }
    }

    T& operator*() const {
        return *ptr_;
    }

    T* operator->() const {
        return ptr_;
    }

    int use_count() const {
        return *use_count_;
    }

    bool unique() const {
        return *use_count_ == 1;
    }
private:
    int* use_count_;
    T* ptr_;
};

#endif // SHARED_PTR_H