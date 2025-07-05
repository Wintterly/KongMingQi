cmake_minimum_required(VERSION 3.10)
project(KongMingQi)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 设置工具链文件
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/mingw-toolchain.cmake)

# 设置本地SFML路径
set(SFML_ROOT "C:/Users/winter/Desktop/SFML-2.6.2")
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIBRARY_DIR "${SFML_ROOT}/lib")

# 添加头文件目录
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${SFML_INCLUDE_DIR})

# 添加库文件目录
link_directories(${SFML_LIBRARY_DIR})

# 添加源文件
file(GLOB SOURCES "src/*.cpp")

# 创建可执行文件
add_executable(KongMingQi ${SOURCES})

# 设置静态链接（改为静态链接以生成独立可执行文件）
set(SFML_STATIC_LIBRARIES TRUE)

# 链接SFML库（使用静态库版本）
target_link_libraries(KongMingQi 
    sfml-graphics-s-d
    sfml-window-s-d
    sfml-system-s-d
    opengl32
    freetype
    winmm
    gdi32
)

# 设置静态运行时库
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# 复制SFML DLL到输出目录
if(WIN32)
    add_custom_command(TARGET KongMingQi POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${SFML_ROOT}/bin"
        $<TARGET_FILE_DIR:KongMingQi>
    )
endif() 