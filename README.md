# Templatized Tetris - C++

## Core Design Overview

This implementation focuses on using templates and resource management using the Big 5. The templates are used for reusability and type safety.

`Buffer<T>` manages any type of 1D array buffer (e.g., the game field or the screen), handling memory allocation, copying, and cleanup generically and safely.

`TetrominoManager<StringType, N>` stores the tetromino shapes, supporting any string type and hardcoded number of pieces (here, 7). This removes code duplication, makes the codebase flexible for future changes (e.g., different field or string types), and assures that memory management is strong and more importantly type-safe.

Both template classes implement constructor, destructor, copy/move constructors, and copy/move assignment. This guarantees correct behavior when objects of these classes are copied, assigned, or destroyed, avoiding memory leaks or double deletes.

## Main Game Logic Flow

The main function creates instances of `TetrominoManager` and `Buffer`, initializing tetromino shapes and the game field. The playfield includes boundaries for wall and floor detection.

### Input Handling
Reads keyboard controls for piece movement (left/right/down) and rotation.

### Logic and Collision
All movement and rotation operations first check collision by calling `DoesPieceFit`, which uses field and tetromino data—now managed by the generic template classes. The piece only moves/rotates if the position is valid.

### Piece Placement and Line Clearing
When a piece cannot move down, it is cemented into the field. The code checks for complete lines, marks, and clears them, updating the score.

### Rendering
The field and active piece are redrawn on the screen buffer, which the Windows API flushes to the console.

## Resource Manangement
No explicit memory freeing is required. When game objects go out of scope, template destructors automatically release memory, preventing leaks.

## Template Class Benefits in Context

- The same buffer template works for both the game field (unsigned char) and the display buffer (wchar_t), showing true code reuse and simplifying bug fixes or enhancements—change the logic in one place, and all uses are improved.

- The tetromino manager, by templating both string type and count, is extensible; use more/less shapes or switch string types without rewriting logic.

- Template instantiation at compile time means what we get is optimized, type-correct code for each buffer and manager type with no runtime penalty, unlike OOP with virtual functions.

## Inheritance Implementation

### Class Heirarchy

This project demonstrates object-oriented inheritance through a specialized buffer hierarchy:

```
Buffer<T> (Base Template Class)
↓
GraphicsBuffer<T> (Abstract Intermediate Class)
↓
├── ScreenBuffer<T> (Concrete Derived Class)
└── FieldBuffer (Concrete Derived Class)
```

### GraphicsBuffer<T>

An abstract template class that extends `Buffer<T>` by adding 2D coordinate functionality:

- **Inheritance**: Inherits from `Buffer<T>` and maintains all base class functionality
- **Additional Features**:
  - Width and height dimensions for 2D access
  - `at(x, y)` method for convenient 2D indexing
  - Pure virtual `initialize()` method enforcing derived class implementation
- **Big Five**: Properly implements all special member functions, calling base class versions appropriately
- **Polymorphism**: Uses virtual destructor and pure virtual methods to enable runtime polymorphism

### ScreenBuffer<T>

A specialized graphics buffer for console screen rendering:

- **Purpose**: Manages the visual display buffer for the Tetris game
- **Specialized Methods**:
  - `drawCharacter(x, y, ch)`: Draw individual characters at 2D coordinates
  - `drawString(x, y, str, maxLen)`: Draw strings at specified positions
  - `clear()`: Reset the screen to default state
- **Initialization**: Automatically fills the buffer with space characters
- **Type Safety**: Template parameter allows different character types (char, wchar_t)

### FieldBuffer

A specialized graphics buffer for the Tetris game field:

- **Purpose**: Manages the game state grid where pieces are placed
- **Specialized Methods**:
  - `isCellOccupied(x, y)`: Check if a cell contains a piece or boundary
  - `setCellValue(x, y, value)`: Set piece values at specific coordinates
  - `getCellValue(x, y)`: Retrieve cell contents safely
  - `isLineComplete(y)`: Check if a horizontal line is full (for clearing)
  - `clearLine(y)`: Mark a line for removal
  - `collapseLine(y)`: Remove a line and shift blocks down
- **Initialization**: Automatically creates boundaries (walls and floor)
- **Boundary Handling**: Returns boundary values for out-of-bounds access