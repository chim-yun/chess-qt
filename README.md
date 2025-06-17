# Chess Qt

This project implements a minimal Qt-based chess program in C++.
Features include:

- Login and simple registration stored in an SQLite database
- Basic chess board with piece images
- Skeleton for AI play via Stockfish (not yet fully implemented)
- Ten minute timers per side

Build with CMake and Qt5 development packages:

```bash
mkdir build && cd build
cmake ..
make
./chess-qt
```

