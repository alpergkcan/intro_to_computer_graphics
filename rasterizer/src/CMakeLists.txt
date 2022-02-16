cmake_minimum_required(VERSION 3.2)

project(HW2_Rasterizer
  VERSION 0.1
  LANGUAGES C CXX
  )

add_executable(rasterizer
  Main.cpp
  Color.cpp Vec3.cpp Vec4.cpp Matrix4.cpp
  Rotation.cpp Translation.cpp Scaling.cpp
  Mesh.cpp Triangle.cpp
  Camera.cpp Scene.cpp
  Helpers.cpp tinyxml2.cpp
  Line.cpp    Line.h
  )

set_property(
  TARGET rasterizer
  PROPERTY CXX_STANDARD 11
  )
