cmake_minimum_required(VERSION 3.16)

project(Attribute_Parser LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(Attribute_Parser
  main.cpp
  attributeparser.cpp attributeparser.h
)
target_link_libraries(Attribute_Parser Qt${QT_VERSION_MAJOR}::Core)

include(GNUInstallDirs)
install(TARGETS Attribute_Parser
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
