add_library(SDL2 INTERFACE)

target_include_directories(SDL2 INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SDL2 INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/${ARCH_DIR}/SDL2.lib
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/${ARCH_DIR}/SDL2main.lib
)