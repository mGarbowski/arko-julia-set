cmake_minimum_required(VERSION 3.12)
project(Hello C)

set(CMAKE_C_STANDARD 11)

# Set the path to the Allegro library
set(ALLEGRO_INCLUDE_DIR "/usr/include/x86_64-linux-gnu")
set(ALLEGRO_LIBRARIES "-lallegro_font -lallegro")

# Add the executable target
add_executable(hello hello.c)

# Include directories for Allegro
target_include_directories(hello PRIVATE ${ALLEGRO_INCLUDE_DIR})

# Link Allegro libraries
target_link_libraries(hello PRIVATE ${ALLEGRO_LIBRARIES})
