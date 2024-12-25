cmake_minimum_required(VERSION 3.10)
cmake_policy(SET CMP0072 NEW)

project(mvsync C CXX)
set(CXX_STANDARD_REQUIRED 14)

if(APPLE)
    list(APPEND EXTRA_LIBS
        "-framework OpenGL"
    )

elseif(WIN32)
    list(APPEND EXTRA_LIBS
        "-lglu32 -lopengl32"
    )
    set(CMAKE_EXE_LINKER_FLAGS "-std=gnu99 -static -static-libgcc -static-libstdc++ -mwindows")
else()
    list(APPEND EXTRA_LIBS
        "-lGL -lGLU -lX11"
    )
endif()

add_executable(mvsync src/main.cpp)
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

target_link_libraries(mvsync PRIVATE OpenGL::GL glfw dl)