cmake_minimum_required(VERSION 3.3)
project(RayTracing)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Widgets finds its own dependencies (QtGui and QtCore).
find_package(Qt5Widgets)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pg -g")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp Vector.h Primitives.cpp Primitives.h Scene.cpp Scene.h Material.h Render.cpp Render.h
        ConvertToQImage.cpp ConvertToQImage.h LoadFromStl.cpp LoadFromStl.h KDTree.cpp KDTree.h LoadFromRt.cpp
        LoadFromRt.h)
add_executable(RayTracing ${SOURCE_FILES})

qt5_use_modules(RayTracing Widgets)