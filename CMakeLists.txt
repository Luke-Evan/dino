
add_executable(dino2 dino2.c dino2.h )

target_link_libraries(dino2 mingw32)
target_link_libraries(dino2 SDL2main)
target_link_libraries(dino2 SDL2)
target_link_libraries(dino2 SDL2_image)
target_link_libraries(dino2 SDL2_ttf)

