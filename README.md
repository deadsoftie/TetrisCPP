# Templatized Tetris - C++

## Core Design Overview

This implementation focuses on using templates and resource management using the Big 4. The templates are used for reusability and type safety.

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