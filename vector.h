#pragma once

#include <cstddef>
#include <utility>
#include <algorithm>
#include <type_traits>
#include <stdexcept>

template<typename T>
class Vector {
public:
    /////////////////////////////////////////////////////////////
    // Rule-of-5 lets gooo...

    // Default constructor
    Vector() noexcept : ptr_(nullptr), size_(0), capacity_(0) {        
    }

    // Value constructor
    explicit Vector(size_t size) {
        ptr_ = new T[size];
        size_ = size;
        capacity_ = size;
    }

    // Copy constructor
    Vector(const Vector& other) {
        ptr_ = new T[other.size_];
        size_ = other.size_;
        capacity_ = other.capacity_;
        for (size_t i = 0; i < size_; ++i) {
            ptr_[i] = other.ptr_[i];
        }
    }

    // Move constructor
    Vector(Vector&& other) noexcept {
        ptr_ = std::exchange(other.ptr_, nullptr);
        size_ = std::exchange(other.size_, 0);
        capacity_ = std::exchange(other.capacity_, 0);
    }

    // Copy=
    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] ptr_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            ptr_ = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                ptr_[i] = other.ptr_[i];
            }
        }
        return *this;
    }

    // Move=
    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] ptr_;
            ptr_ = std::exchange(other.ptr_, nullptr);
            size_ = std::exchange(other.size_, 0);
            capacity_ = std::exchange(other.capacity_, 0);
        }
        return *this;
    }

    // Destructor
    ~Vector() {
        delete[] ptr_;
    }

    /////////////////////////////////////////////////////////////
    // Element access

    // at() for nonconst vector
    T& at(size_t pos) {
        if (pos >= size_) {
            throw std::out_of_range("Vector index is out of range");
        }
        return ptr_[pos];
    }

    // at() for const vector
    const T& at(size_t pos) const {
        if (pos >= size_) {
            throw std::out_of_range("Vector index is out of range");
        }
        return ptr_[pos];
    }

    // operator[] for nonconst vector
    T& operator[](size_t pos) {
        return ptr_[pos];
    }

    // operator[] for const vector
    const T& operator[](size_t pos) const {
        return ptr_[pos];
    }

    // front() for nonconst vector
    T& front() {
        return ptr_[0];
    }
    
    // front() for const vector
    const T& front() const {
        return ptr_[0];
    }

    // back() for nonconst vector
    T& back() {
        return ptr_[size_ - 1];
    }
    
    // back() for const vector
    const T& back() const {
        return ptr_[size_ - 1];
    }

    // data() for nonconst vector
    T* data() noexcept {
        return ptr_;
    }
    
    // data() for const vector
    const T* data() const noexcept {
        return ptr_;
    }

    /////////////////////////////////////////////////////////////
    // Iterators
    // Not now

    /////////////////////////////////////////////////////////////
    // Capacity

    // Current size
    size_t size() const noexcept {
        return size_;
    }

    // Allocated space
    size_t capacity() const noexcept {
        return capacity_;
    }

    /////////////////////////////////////////////////////////////
    // Modifiers

    // push_back for lvalue
    void push_back(const T& value) {
        if (size_ + 1 > capacity_) {
            capacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
            T* new_ptr = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                new_ptr[i] = ptr_[i];
            }
            delete[] ptr_;
            ptr_ = new_ptr;
        }
        ptr_[size_] = value;
        ++size_;
    }

    // push_back for rvalue
    void push_back(T&& value) {
        if (size_ + 1 > capacity_) {
            capacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
            T* new_ptr = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                new_ptr[i] = std::move(ptr_[i]);
            }
            delete[] ptr_;
            ptr_ = new_ptr;
        }
        ptr_[size_] = std::move(value);
        ++size_;
    }

    void resize(size_t count) {
        if (count != size_) {
            capacity_ = std::max(capacity_ * 2, count);
            T* new_ptr = new T[capacity_];
            size_ = std::min(size_, count);
            for (size_t i = 0; i < size_; ++i) {
                new_ptr[i] = ptr_[i];
            }
            delete[] ptr_;
            ptr_ = new_ptr;
        }
    }

private:
    T* ptr_;
    size_t size_;
    size_t capacity_;
};
