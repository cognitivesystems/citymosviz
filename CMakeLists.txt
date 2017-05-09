project(citymos_viz)
cmake_minimum_required(VERSION 2.8.12)

set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/modules)
set (CMAKE_PREFIX_PATH "/opt/Qt5")

# Qt library
find_package(Qt5Widgets)
find_package(Qt5OpenGL)
find_package(Qt5Network)
find_package(Qt5Xml)
find_package(Qt5Svg)
find_package(Qt5Charts)
find_package(OpenSceneGraph REQUIRED COMPONENTS osgDB osgGA osgUtil osgViewer)
find_package(Boost COMPONENTS system)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

# OpenGL & GLEW library

INCLUDE_DIRECTORIES(/usr/include/x86_64-linux-gnu/qt5/qmqtt
    {OPENSCENEGRAPH_INCLUDE_DIRS} ${Boost_INCLUDE_DIR})

QT5_WRAP_UI(MOC_UI_SRCS
            src/mainwindow.ui
)

#QT5_WRAP_CPP(MOC_SRCS
#            src/mainwindow.h
#            src/citymosinf.h
#            src/mqttsubscriber.h
#            src/agent.h
#)

SET(HDRS
    src/mainwindow.h
    src/osgwidget.h
    src/citymosinf.h
    src/mqttsubscriber.h
    src/mqttvehiclesubscriber.h
    src/mqttsrtsubscriber.h
    src/mqttpedestriansubscriber.h
    src/agent.h
    src/roadnetwork.h
    #src/defs.h
)

SET(SRCS
    src/mainwindow.cpp
    src/osgwidget.cpp
    src/citymosinf.cpp
    src/mqttsubscriber.cpp
    src/mqttsrtsubscriber.cpp
    src/mqttvehiclesubscriber.cpp
    src/mqttpedestriansubscriber.cpp
    src/agent.cpp
    src/roadnetwork.cpp
    src/main.cpp
)

add_executable(citymosviz ${MOC_UI_SRCS} ${MOC_SRCS} ${HDRS} ${SRCS})

qt5_use_modules(citymosviz Widgets OpenGL Network Xml Charts)

target_link_libraries(citymosviz
    ${OPENSCENEGRAPH_LIBRARIES} ${Boost_LIBRARIES}
    qmqtt)
