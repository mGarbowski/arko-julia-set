cmake_minimum_required(VERSION 3.12)
project(Julia C)

set(CMAKE_C_STANDARD 11)

# Set the path to the Allegro library
set(ALLEGRO_INCLUDE_DIR "/usr/include/x86_64-linux-gnu")
set(ALLEGRO_LIBRARIES "-lallegro_font -lallegro")

# Add the executable target
add_executable(julia julia.c)

# Include directories for Allegro
target_include_directories(julia PRIVATE ${ALLEGRO_INCLUDE_DIR})

# Link Allegro libraries
target_link_libraries(julia PRIVATE ${ALLEGRO_LIBRARIES})
