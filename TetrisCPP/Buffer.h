#ifndef BUFFER_H
#define BUFFER_H

template<typename T>
class Buffer {
protected:
    T* data;
    size_t size;

public:
    // Constructor
    Buffer(size_t n);

    // Destructor
    ~Buffer();

    // Copy Constructor
    Buffer(const Buffer& other);

    // Copy Assignment Operator
    Buffer& operator=(const Buffer& other);

    // Move Constructor
    Buffer(Buffer&& other) noexcept;

    // Move Assignment Operator
    Buffer& operator=(Buffer&& other) noexcept;

    // Access operators
    T& operator[](size_t i);
    const T& operator[](size_t i) const;

    // Get raw pointer
    T* get();
    const T* get() const;

    // Get size
    size_t getSize() const;
};

template<typename T>
Buffer<T>::Buffer(size_t n) : size(n), data(new T[n]) {
    for (size_t i = 0; i < size; ++i) {
        data[i] = T();
    }
}

template<typename T>
Buffer<T>::~Buffer() {
    delete[] data;
}

template<typename T>
Buffer<T>::Buffer(const Buffer& other) : size(other.size), data(new T[other.size]) {
    for (size_t i = 0; i < size; ++i) {
        data[i] = other.data[i];
    }
}

template<typename T>
Buffer<T>& Buffer<T>::operator=(const Buffer& other) {
    if (this != &other) {
        delete[] data;
        size = other.size;
        data = new T[other.size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
    return *this;
}

template<typename T>
Buffer<T>::Buffer(Buffer&& other) noexcept : data(other.data), size(other.size) {
    other.data = nullptr;
    other.size = 0;
}

template<typename T>
Buffer<T>& Buffer<T>::operator=(Buffer&& other) noexcept {
    if (this != &other) {
        delete[] data;
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
    }
    return *this;
}

template<typename T>
T& Buffer<T>::operator[](size_t i) {
    return data[i];
}

template<typename T>
const T& Buffer<T>::operator[](size_t i) const {
    return data[i];
}

template<typename T>
T* Buffer<T>::get() {
    return data;
}

template<typename T>
const T* Buffer<T>::get() const {
    return data;
}

template<typename T>
size_t Buffer<T>::getSize() const {
    return size;
}

#endif // BUFFER_H
