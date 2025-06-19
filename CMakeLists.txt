 cmake_minimum_required(VERSION 3.16)
project(Qt6Oscilloscope)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Găsește Qt6 și componentele necesare
find_package(Qt6 REQUIRED COMPONENTS Widgets SerialPort)

# Sursele proiectului
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

# Executabilul final
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS}
    DataGenerator.h DataGenerator.cpp)

# Leagă bibliotecile Qt necesare
target_link_libraries(${PROJECT_NAME}
    Qt6::Widgets
    Qt6::SerialPort
)

