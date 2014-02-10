# The name of our project is "P1".  CMakeLists files in this project can

MESSAGE ("**********************************************")
MESSAGE ("* Generating P1 demo project files ... *")
MESSAGE ("**********************************************")

PROJECT(P1)

# Find OpenGL
INCLUDE (${CMAKE_ROOT}/Modules/FindOpenGL.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLU.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindGLUT.cmake)
SET(CMAKE_CXX_FLAGS "-Wall -std=c++0x -g")
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_PATH})
INCLUDE_DIRECTORIES(${GLUT_INCLUDE_PATH})


# Make executable
ADD_EXECUTABLE(p1_bin main.cpp ase.h ase.cpp vector3f.h vector3f.cpp
    matrix4x4f.h matrix4x4f.cpp text.h text.cpp box3f.h box3f.cpp octree.h octree.cpp
    input.h input.cpp)

# Libraries to link
TARGET_LINK_LIBRARIES(p1_bin ${OPENGL_LIBRARY} ${GLUT_LIBRARY} )
