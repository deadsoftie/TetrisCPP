#ifndef GRAPHICS_BUFFER_H
#define GRAPHICS_BUFFER_H
#include "Buffer.h"

// Base graphics buffer class - inherits from Buffer
template<typename T>
class GraphicsBuffer : public Buffer<T> {
protected:
    int width;
    int height;

public:
    // Constructor
    GraphicsBuffer(int w, int h)
        : Buffer<T>(w* h), width(w), height(h) {}

    // Virtual destructor
    virtual ~GraphicsBuffer() = default;

    // Copy Constructor
    GraphicsBuffer(const GraphicsBuffer& other)
        : Buffer<T>(other), width(other.width), height(other.height) {}

    // Copy Assignment Operator
    GraphicsBuffer& operator=(const GraphicsBuffer& other) {
        if (this != &other) {
            Buffer<T>::operator=(other);
            width = other.width;
            height = other.height;
        }
        return *this;
    }

    // Move Constructor
    GraphicsBuffer(GraphicsBuffer&& other) noexcept
        : Buffer<T>(std::move(other)), width(other.width), height(other.height) {
        other.width = 0;
        other.height = 0;
    }

    // Move Assignment Operator
    GraphicsBuffer& operator=(GraphicsBuffer&& other) noexcept {
        if (this != &other) {
            Buffer<T>::operator=(std::move(other));
            width = other.width;
            height = other.height;
            other.width = 0;
            other.height = 0;
        }
        return *this;
    }

    // 2D access methods
    T& at(int x, int y) {
        return this->data[y * width + x];
    }

    const T& at(int x, int y) const {
        return this->data[y * width + x];
    }

    // Getters
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // Pure virtual method - forces derived classes to implement
    virtual void initialize() = 0;
};

#endif // GRAPHICS_BUFFER_H
