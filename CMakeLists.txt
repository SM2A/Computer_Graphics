cmake_minimum_required(VERSION 3.17)
project(Bicycle)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(Bicycle Source/main.cpp Source/static.h)

target_link_libraries(Bicycle ${OPENGL_LIBRARIES})
target_link_libraries(Bicycle ${GLUT_LIBRARIES})
target_link_libraries(Bicycle -lopengl32 -lfreeglut)
