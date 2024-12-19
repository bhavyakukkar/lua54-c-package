1. Build C shared obj file
  ```sh
  gcc -shared -fPIC mylibrary.c -o mylibrary.so -lm -llua5.4 -I/usr/include/lua5.4
  ```

2. Run Lua script using the C package
  ```sh
  lua5.4 mybinary.lua
  ```
