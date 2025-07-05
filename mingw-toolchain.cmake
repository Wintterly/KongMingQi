# 设置目标系统
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# 设置编译器
set(CMAKE_C_COMPILER "C:/Program Files/mingw64/bin/gcc.exe")
set(CMAKE_CXX_COMPILER "C:/Program Files/mingw64/bin/g++.exe")

# 设置查找根路径
set(CMAKE_FIND_ROOT_PATH "C:/Program Files/mingw64")

# 调整查找行为
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# 设置静态链接
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++") 