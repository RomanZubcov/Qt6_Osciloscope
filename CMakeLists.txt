cmake_minimum_required(VERSION 3.16)
project(Qt6Oscilloscope)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets SerialPort)

set(SOURCES
    main.cpp
    MainWindow.cpp
    OscilloscopeView.cpp
    SerialReader.cpp
)

set(HEADERS
    MainWindow.h
    OscilloscopeView.h
    SerialReader.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} DataGenerator.h DataGenerator.cpp)

target_link_libraries(${PROJECT_NAME}
    Qt6::Widgets
    Qt6::SerialPort
)
