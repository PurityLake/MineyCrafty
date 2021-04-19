# MineyCrafty
Minecraft clone

# Compile
## Windows
### For vcpkg
```
vcpkg install sdl2 sdl2-ttf sdl2-image sdl2-mixer sdl2-net glew --triplet=x64-windows
```

### CMake
```
mkdir build
cd build
cmake ..
cmake --build . --config Release
.\Release\mineycrafty.exe
```