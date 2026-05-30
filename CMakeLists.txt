#Every time this file is changed, run: cmake -S . -B test
#Every re-compile, run: cmake --build test
#The game.exe file will appear in test/

cmake_minimum_required(VERSION 3.20)

project(test LANGUAGES CXX)

#Source, aka cpp files
add_executable(test 
	src/Math.cpp
	src/Implement.cpp
	src/main.cpp
)

add_custom_command(TARGET test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${CMAKE_SOURCE_DIR}/Assets
            $<TARGET_FILE_DIR:test>/Assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different
            ${CMAKE_SOURCE_DIR}/Shaders
            $<TARGET_FILE_DIR:test>/Shaders
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "D:/C++ libraries/SDL3/SDL3.dll"
            $<TARGET_FILE_DIR:test>/SDL3.dll
)

# Compiler flags
target_compile_options(test PRIVATE
    -Wno-narrowing
)

# Include directories (-I)
target_include_directories(test PRIVATE
    "D:/C++ libraries/SDL3/include"
    #"D:/C++ libraries/SDL3_image/include"
)

# Library directories (-L)
target_link_directories(test PRIVATE
    "D:/C++ libraries/SDL3/lib"
    #"D:/C++ libraries/SDL3_image/lib"
)

# Libraries (-l)
target_link_libraries(test PRIVATE
    mingw32
    SDL3
    #SDL3_image
)