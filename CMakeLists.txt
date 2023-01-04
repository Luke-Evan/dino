cmake_minimum_required(VERSION 3.23)
project(dino)
set(CMAKE_C_STANDARD 23)


add_executable(dino2 dino2.c dino2.h )

target_link_libraries(dino2 mingw32)
target_link_libraries(dino2 SDL2main)
target_link_libraries(dino2 SDL2)
target_link_libraries(dino2 SDL2_image)
target_link_libraries(dino2 SDL2_ttf)
target_link_libraries(dino2 SDL2_mixer)

