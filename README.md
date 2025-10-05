
# Carolyna Chess Engine 

**Carolyna** is a C++ chess engine built using bitboards, magic tables, and a UCI interface. Pure **stdc++**, no external libraries required.

----------

## How to build from source

### Requirements

-   **C++ compiler** (GCC, Clang)
    
-   **CMake ≥ 3.16**
    
-   **VS Code** (optional, but recommended)
    

----------

### 1.  Clone / Download

```
git clone https://github.com/YourUsername/Carolyna.git
cd Carolyna
```

### 2. Create build folder

```
mkdir build
cd build
```

### 3. Configure with CMake

```
cmake .. -DCMAKE_BUILD_TYPE=Release
```

-   `Release` = fast, optimized build
    
-   `Debug` = slower, with debug info
    

### 4. Build

```
cmake --build .
```

-   Output executable: `carolyna` (or `carolyna.exe` on Windows)
    

### 5. Run

```
./carolyna   # Linux / Mac
carolyna.exe # Windows
```

----------

##  Or just download prebuilt

-   Windows: `Carolyna.exe`
    
-   Linux / Mac: `carolyna`
    

> No need to build, just run!

----------

###  Notes

-   `magic_init/MagicInitiator.cpp` is a **helper program** to generate magic bitboard numbers, not required for running the engine.
    
-   All source files are **pure C++**, headers in `include/`.
    
-   Build type can be switched in CMake (`Release` vs `Debug`).
    

----------

Have fun flying!