#ifndef UNIQUE_PTR_H
#define UNIQUE_PTR_H

template <typename T>
class Unique_ptr {
public:
    Unique_ptr(T* ptr) : ptr_(ptr) {}
    Unique_ptr(Unique_ptr&& that) : ptr_(that.ptr_) {
        that.ptr_ = nullptr;
    }
    Unique_ptr& operator=(Unique_ptr<T>&& that) {
        Unique_ptr<T>().swap(*this);
        ptr_ = that.ptr_;
        that.ptr_ = nullptr;
        return *this;
    };

    Unique_ptr(const Unique_ptr&) = delete;
    Unique_ptr& operator=(const Unique_ptr&) = delete;

    ~Unique_ptr() {
        delete ptr_;
    }

    void swap(Unique_ptr<T>& that) {
        std::swap(ptr_, that.ptr_);
    }

    T& operator*() const { return *ptr_;}

    T* operator->() const { return ptr_; }

    T* get() const { return ptr_; }

    T* release() {
        T* ptr = ptr_;
        ptr_ = nullptr;
        return ptr;
    }

private:
    T* ptr_;
};
#endif // UNIQUE_PTR_H