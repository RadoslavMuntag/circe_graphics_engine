# Circe Engine - Build & Debug Guide

## Project Structure

```
circe_engine/
├── build/
│   ├── debug/        # Debug builds (unoptimized, symbols, warnings)
│   └── release/      # Release builds (optimized, no debug symbols)
├── engine/           # Circe engine library (shared .so)
├── game/             # Game executable
├── external/         # Third-party dependencies
└── assets/           # Shaders, textures, models
```

## Build Configurations

### Debug Build
- **Optimization**: `-O0` (none)
- **Symbols**: Full debug symbols (`-g`)
- **Warnings**: `-Wall -Wextra`
- **Defines**: `CIRCE_DEBUG`, `CIRCE_BUILD_DLL`
- **Use for**: Development, debugging, profiling

### Release Build
- **Optimization**: `-O3` (maximum)
- **Symbols**: None
- **Warnings**: Disabled
- **Defines**: `NDEBUG`, `CIRCE_RELEASE`, `CIRCE_BUILD_DLL`
- **Use for**: Performance testing, distribution

---

## Building from Command Line

### Using Build Scripts

```bash
# Debug build
./build.sh debug

# Release build
./build.sh release

# Clean build
./build.sh debug clean
./build.sh release clean

# Run the game
./run.sh debug
./run.sh release
```

### Manual CMake

```bash
# Configure Debug
cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug

# Build Debug
cmake --build build/debug -j4

# Configure Release
cmake -B build/release -DCMAKE_BUILD_TYPE=Release

# Build Release
cmake --build build/release -j4

# Run
./build/debug/bin/Game
./build/release/bin/Game
```

---

## VSCode Tasks

Press `Ctrl+Shift+P` → "Tasks: Run Task" to access:

| Task | Description |
|------|-------------|
| **CMake Build Debug** | Build debug configuration (default: `Ctrl+Shift+B`) |
| **CMake Build Release** | Build release configuration |
| **Clean Debug** | Remove debug build directory |
| **Clean Release** | Remove release build directory |
| **Rebuild Debug** | Clean + build debug |
| **Rebuild Release** | Clean + build release |

---

## VSCode Debug Configurations

Press `F5` or open "Run and Debug" panel to select:

### Debug Configurations

| Configuration | Description | Use When |
|---------------|-------------|----------|
| **(gdb) Debug Build** | Full debugging with breakpoints | Debugging crashes, logic errors |
| **(gdb) Release Build** | Debug optimized code | Debugging release-only issues |
| **(run) Debug Build** | Quick run without debugger | Testing changes quickly |
| **(run) Release Build** | Run optimized build | Performance testing |

### Common Keyboard Shortcuts

- `F5` - Start debugging (selected configuration)
- `Ctrl+F5` - Run without debugging
- `F9` - Toggle breakpoint
- `F10` - Step over
- `F11` - Step into
- `Shift+F11` - Step out
- `Shift+F5` - Stop debugging
- `Ctrl+Shift+B` - Build (default task)

---

## Debugging Tips

### Setting Breakpoints
1. Click left of line number (red dot appears)
2. Or place cursor on line and press `F9`
3. Conditional breakpoints: Right-click → "Add Conditional Breakpoint"

### Watch Variables
- Hover over variable while debugging
- Add to "Watch" panel: Right-click → "Add to Watch"
- View in "Variables" panel (auto-populated)

### Call Stack
- "Call Stack" panel shows function call hierarchy
- Click frames to navigate up/down the stack

### Debug Console
- Evaluate expressions while paused
- Example: Type `camera->GetPosition()` and press Enter

### Debug vs Release Differences
- **Debug**: 
  - Slower execution
  - All symbols available
  - Better error messages
  - Variables not optimized away
  
- **Release**:
  - 3-10x faster
  - Code inlined/optimized
  - Some variables optimized away
  - Harder to debug but represents final performance

---

## Build Output

### Debug Build
```
build/debug/
├── bin/
│   └── Game              # ~753 KB (with symbols)
└── lib/
    └── libCirce.so.1.0.0 # ~4.7 MB (with symbols)
```

### Release Build
```
build/release/
├── bin/
│   └── Game              # Smaller, optimized
└── lib/
    └── libCirce.so.1.0.0 # Smaller, optimized
```

---

## Common Issues

### "Cannot find libCirce.so"
The game needs to find the shared library at runtime:
```bash
# Option 1: Run from project root (RPATH is set)
./build/debug/bin/Game

# Option 2: Set LD_LIBRARY_PATH
export LD_LIBRARY_PATH=./build/debug/lib:$LD_LIBRARY_PATH
./build/debug/bin/Game
```

### "Build files have been written to wrong directory"
Clean and reconfigure:
```bash
./build.sh debug clean
```

### Breakpoint not hit
1. Ensure you're running Debug build (not Release)
2. Check you saved the file (`Ctrl+S`)
3. Rebuild with `Ctrl+Shift+B`
4. Check "Breakpoints" panel - breakpoints should be solid red (not hollow)

### Code changes not reflected
The build task runs automatically before debug, but if issues persist:
```bash
./build.sh debug clean  # Force clean rebuild
```

---

## CMake Configuration

### Default Build Type
Set in `CMakeLists.txt`:
```cmake
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()
```

### Compiler Flags
**Debug**: `-g -O0 -Wall -Wextra -DCIRCE_DEBUG`
**Release**: `-O3 -DNDEBUG -DCIRCE_RELEASE`

### Custom Flags
Edit `CMakeLists.txt` to add project-specific flags:
```cmake
target_compile_options(Circe PRIVATE -Wconversion)
```

---

## Shared Library Info

The Circe engine is built as a **shared library** (`libCirce.so`):

### Benefits
- ✅ Faster iteration (no relinking game when engine changes)
- ✅ Smaller executable
- ✅ Multiple programs can share the library
- ✅ Easier to update engine separately

### Technical Details
- **Version**: 1.0.0 (SOVERSION: 1)
- **Type**: ELF 64-bit LSB shared object
- **Symbols**: Exported via `CIRCE_API` macro
- **PIC**: Position Independent Code enabled

### Checking Library Dependencies
```bash
# List shared library dependencies
ldd build/debug/bin/Game

# Should show:
# libCirce.so.1 => /path/to/circe_engine/build/debug/lib/libCirce.so.1
```

---

## Performance Profiling

### Using `perf` (Linux)
```bash
# Build release
./build.sh release

# Profile
perf record -g ./build/release/bin/Game

# View results
perf report
```

### Using `gprof`
```bash
# Add to CMakeLists.txt:
# target_compile_options(Circe PRIVATE -pg)
# target_link_options(Circe PRIVATE -pg)

./build/release/bin/Game
gprof build/release/bin/Game gmon.out > analysis.txt
```

### Using `valgrind`
```bash
# Memory leaks
valgrind --leak-check=full ./build/debug/bin/Game

# Profiling
valgrind --tool=callgrind ./build/debug/bin/Game
kcachegrind callgrind.out.*
```

---

## Project Compilation Database

CMake generates `compile_commands.json` for LSP/IntelliSense:
```
build/debug/compile_commands.json
build/release/compile_commands.json
```

VSCode automatically uses this for C++ IntelliSense.

---

## Quick Reference

| Action | Command/Shortcut |
|--------|------------------|
| Build debug | `Ctrl+Shift+B` or `./build.sh debug` |
| Build release | Tasks menu or `./build.sh release` |
| Run debug | `F5` (select Debug configuration) |
| Run release | `F5` (select Release configuration) |
| Clean build | `./build.sh debug clean` |
| Toggle breakpoint | `F9` |
| Step over | `F10` |
| Step into | `F11` |
| Continue | `F5` |
| Stop | `Shift+F5` |

---

## Additional Resources

- **CMake Documentation**: https://cmake.org/documentation/
- **GDB Tutorial**: https://www.gnu.org/software/gdb/documentation/
- **VSCode C++ Debugging**: https://code.visualstudio.com/docs/cpp/cpp-debug

---

*Last updated: February 20, 2026*
