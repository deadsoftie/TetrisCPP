#ifndef TETROMINO_MANAGER_H
#define TETROMINO_MANAGER_H

#include <cstddef>

template<typename StringType, size_t N>
class TetrominoManager {
private:
    StringType* pieces;
    size_t count;

public:
    // Constructor
    TetrominoManager();

    // Destructor
    ~TetrominoManager();

    // Copy Constructor
    TetrominoManager(const TetrominoManager& other);

    // Copy Assignment Operator
    TetrominoManager& operator=(const TetrominoManager& other);

    // Move Constructor
    TetrominoManager(TetrominoManager&& other) noexcept;

    // Move Assignment Operator
    TetrominoManager& operator=(TetrominoManager&& other) noexcept;

    // Access operators
    StringType& operator[](size_t i);
    const StringType& operator[](size_t i) const;

    // Get count
    size_t getCount() const;
};

// Template implementation must be in header file
template<typename StringType, size_t N>
TetrominoManager<StringType, N>::TetrominoManager() : count(N), pieces(new StringType[N]) {}

template<typename StringType, size_t N>
TetrominoManager<StringType, N>::~TetrominoManager() {
    delete[] pieces;
}

template<typename StringType, size_t N>
TetrominoManager<StringType, N>::TetrominoManager(const TetrominoManager& other)
    : count(other.count), pieces(new StringType[other.count]) {
    for (size_t i = 0; i < count; ++i) {
        pieces[i] = other.pieces[i];
    }
}

template<typename StringType, size_t N>
TetrominoManager<StringType, N>& TetrominoManager<StringType, N>::operator=(const TetrominoManager& other) {
    if (this != &other) {
        delete[] pieces;
        count = other.count;
        pieces = new StringType[other.count];
        for (size_t i = 0; i < count; ++i) {
            pieces[i] = other.pieces[i];
        }
    }
    return *this;
}

template<typename StringType, size_t N>
TetrominoManager<StringType, N>::TetrominoManager(TetrominoManager&& other) noexcept
    : pieces(other.pieces), count(other.count) {
    other.pieces = nullptr;
    other.count = 0;
}

template<typename StringType, size_t N>
TetrominoManager<StringType, N>& TetrominoManager<StringType, N>::operator=(TetrominoManager&& other) noexcept {
    if (this != &other) {
        delete[] pieces;
        pieces = other.pieces;
        count = other.count;
        other.pieces = nullptr;
        other.count = 0;
    }
    return *this;
}

template<typename StringType, size_t N>
StringType& TetrominoManager<StringType, N>::operator[](size_t i) {
    return pieces[i];
}

template<typename StringType, size_t N>
const StringType& TetrominoManager<StringType, N>::operator[](size_t i) const {
    return pieces[i];
}

template<typename StringType, size_t N>
size_t TetrominoManager<StringType, N>::getCount() const {
    return count;
}

#endif // TETROMINO_MANAGER_H
