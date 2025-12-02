#ifndef SCREEN_BUFFER_H
#define SCREEN_BUFFER_H

#include "GraphicsBuffer.h"

template<typename T>
class ScreenBuffer : public GraphicsBuffer<T> {
public:
    // Constructor
    ScreenBuffer(int w, int h) : GraphicsBuffer<T>(w, h) {
	    ScreenBuffer<T>::initialize();
    }

    // Destructor
    ~ScreenBuffer() override = default;

    // Implement pure virtual function
    void initialize() override {
        // Initialize screen with spaces
        for (int i = 0; i < this->width * this->height; i++) {
            this->data[i] = static_cast<T>(' ');
        }
    }

    // Screen-specific method to draw a character at 2D position
    void drawCharacter(int x, int y, T ch) {
        if (x >= 0 && x < this->width && y >= 0 && y < this->height) {
            this->at(x, y) = ch;
        }
    }

    // Clear the screen
    void clear() {
        initialize();
    }

    // Draw a string at position
    void drawString(int x, int y, const T* str, int maxLen) {
        int i = 0;
        while (str[i] != 0 && i < maxLen && (x + i) < this->width) {
            drawCharacter(x + i, y, str[i]);
            i++;
        }
    }
};

#endif // SCREEN_BUFFER_H
