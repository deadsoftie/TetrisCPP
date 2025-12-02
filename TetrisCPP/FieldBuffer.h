#ifndef FIELD_BUFFER_H
#define FIELD_BUFFER_H

#include "GraphicsBuffer.h"

class FieldBuffer : public GraphicsBuffer<unsigned char> {
public:
    // Constructor
    FieldBuffer(int w, int h) : GraphicsBuffer<unsigned char>(w, h) {
	    FieldBuffer::initialize();
    }

    // Destructor
    ~FieldBuffer() override = default;

    // Implement pure virtual function
    void initialize() override {
        // Initialize field with boundaries
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                at(x, y) = (x == 0 || x == width - 1 || y == height - 1) ? 9 : 0;
            }
        }
    }

    // Field-specific methods
    bool isCellOccupied(int x, int y) const {
        if (x < 0 || x >= width || y < 0 || y >= height)
            return true; // Out of bounds is treated as occupied
        return at(x, y) != 0;
    }

    void setCellValue(int x, int y, unsigned char value) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            at(x, y) = value;
        }
    }

    unsigned char getCellValue(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return at(x, y);
        }
        return 9; // Return boundary value for out of bounds
    }

    // Check if a line is complete (for Tetris line clearing)
    bool isLineComplete(int y) const {
        if (y < 0 || y >= height - 1) return false;

        for (int x = 1; x < width - 1; x++) {
            if (at(x, y) == 0) return false;
        }
        return true;
    }

    // Clear a line and shift everything down
    void clearLine(int y) {
        if (y < 0 || y >= height - 1) return;

        // Mark line for clearing
        for (int x = 1; x < width - 1; x++) {
            at(x, y) = 8; // Special marker
        }
    }

    void collapseLine(int y) {
        if (y < 0 || y >= height - 1) return;

        // Shift everything above this line down
        for (int x = 1; x < width - 1; x++) {
            for (int py = y; py > 0; py--) {
                at(x, py) = at(x, py - 1);
            }
            at(x, 0) = 0; // Clear top line
        }
    }
};

#endif // FIELD_BUFFER_H
