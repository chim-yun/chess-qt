# Chess Qt

This project implements a Qt-based chess program in C++.
Features include:

- Login and simple registration stored in an SQLite database
- Start dialog offering two player or vs AI play
- Basic chess board with piece images and selectable pieces
- Simple move generation and highlight of legal moves
- Ten minute timers per side
- Placeholder for Stockfish integration

Build with CMake and Qt5 development packages:

```bash
mkdir build && cd build
cmake ..
make
./chess-qt
```

