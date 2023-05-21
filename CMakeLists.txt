cmake_minimum_required(VERSION 3.12)
project(Julia C ASM)

set(CMAKE_C_STANDARD 11)

# https://stackoverflow.com/questions/28241611/cmake-uses-nasm-for-linking-which-fails
enable_language(ASM_NASM)
set(CMAKE_ASM_NASM_LINK_EXECUTABLE "ld <FLAGS> <CMAKE_ASM_NASM_LINK_FLAGS> <LINK_FLAGS> <OBJECTS>  -o <TARGET> <LINK_LIBRARIES>")

# Set the path to the Allegro library
set(ALLEGRO_INCLUDE_DIR "/usr/include/x86_64-linux-gnu")
set(ALLEGRO_LIBRARIES "-lallegro_font -lallegro")

# Add the executable target
add_executable(julia julia.c generateJuliaSet.h generateJuliaSet.asm)
target_include_directories(julia PRIVATE ${ALLEGRO_INCLUDE_DIR}) # Include directories for Allegro
target_link_libraries(julia PRIVATE ${ALLEGRO_LIBRARIES}) # Link Allegro libraries
