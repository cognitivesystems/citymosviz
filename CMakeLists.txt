project(citymos_viz)
cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Qt library
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
find_package(Qt5Network)

set(CMAKE_AUTOMOC ON)

# OpenGL & GLEW library
find_package(OpenGL)

INCLUDE_DIRECTORIES(/usr/include/x86_64-linux-gnu/qt5/qmqtt)
# executable
add_executable(citymosviz src/main.cpp src/mainwindow.h src/mainwindow.cpp src/citymosinf.h src/citymosinf.cpp src/mqttsubscriber.h src/mqttsubscriber.cpp
    src/agent.h src/agent.cpp)
qt5_use_modules(citymosviz Widgets OpenGL Network)
target_link_libraries(citymosviz ${QT_LIBRARIES} ${OPENGL_LIBRARIES} qmqtt)
