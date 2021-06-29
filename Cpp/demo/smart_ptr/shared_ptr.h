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
    Shared_ptr() : ptr_(nullptr), use_count_(nullptr) {}
    explicit Shared_ptr(T* ptr) : ptr_(ptr), use_count_(ptr_ ? new int(1) : nullptr) {};
    Shared_ptr(const Shared_ptr<T>& that)
        : ptr_(that.ptr_),
          use_count_(that.use_count_) {
        if(ptr_) ++(*use_count_);
    }
    Shared_ptr(Shared_ptr<T>&& that)
        : ptr_(that.ptr_),
          use_count_(that.use_count_) {

        // 源指针是右值，将其置空
        that.ptr_ = nullptr;
        that.use_count_ = nullptr;
    }

    Shared_ptr& operator= (const Shared_ptr<T>& that) noexcept {
        // 自我赋值
        if(that == *this) return *this;

        Shared_ptr<T>().swap(*this); // 将自己置空，控制权交给一个临时匿名对象
        ptr_ = that.ptr_;
        use_count_ = that.use_count_;
        if(ptr_) ++use_count_;
        return *this;
    }
    Shared_ptr& operator= (Shared_ptr<T>&& that) noexcept {
        // 不会有右值的自我赋值
        // p = std::move(p); // error

        Shared_ptr<T>().swap(*this); // 将自己置空，控制权交给一个临时匿名对象
        ptr_ = that.ptr_;
        use_count_ = that.use_count_;

        // 源指针是右值，将其置空
        that.ptr_ = nullptr;
        that.use_count_ = nullptr;
        return *this;
    }

    // for make_shared()
    // Shared_ptr(int* use_count, T* const ptr)
    //     : use_count_(use_count),
    //       ptr_(ptr) {
    //     ++(*use_count_);
    // };
    

    ~Shared_ptr() {
        if(ptr_ && --(*use_count_) == 0) {
            delete ptr_;
            delete use_count_;
        }
    }

    void swap(Shared_ptr<T>& that) {
        std::swap(ptr_, that.ptr_);
        std::swap(use_count_, that.use_count_);
    }

    bool operator!() const { return !ptr_;}

    T& operator*() const { return *ptr_; }

    T* operator->() const { return ptr_; }

    T* get() const { return ptr_; }

    int use_count() const { return *use_count_;}

    bool unique() const { return *use_count_ == 1;}

private:
    T* ptr_;
    int* use_count_;
};

#endif // SHARED_PTR_H