cmake_minimum_required(VERSION 3.10)
project(HW3_Proj)

set(CMAKE_CXX_STANDARD 11)

add_executable(hw3
  main.cpp
  # external
  glad.c
  # code
  Callbacks.cpp
  window.cpp
  shader.cpp
  Sphere.cpp
  variables.cpp
  texture.cpp
  input.cpp
  )

target_include_directories( hw3 PRIVATE ../libraries/include/ )
target_link_directories   ( hw3 PRIVATE ../libraries/lib/ )
target_link_libraries     ( hw3 PRIVATE dl glfw3 X11 Xinerama Xcursor Xxf86vm Xrandr pthread jpeg )
