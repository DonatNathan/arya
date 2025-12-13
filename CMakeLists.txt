cmake_minimum_required(VERSION 3.28)
project(Arya LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3 -march=native")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -march=native")

include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(
    nlohmann_json
    URL https://github.com/nlohmann/json/releases/latest/download/json.tar.xz
)
FetchContent_MakeAvailable(nlohmann_json)

find_package(OpenCV REQUIRED)

add_subdirectory(external/whisper.cpp)

file(GLOB library_sources "src/*.cpp" "src/graphical/*.cpp")
add_library(GraphicalGameLib ${library_sources})
target_include_directories(GraphicalGameLib PUBLIC includes)
target_link_libraries(GraphicalGameLib
    ${OpenCV_LIBS}
    SFML::Audio
    SFML::Graphics
    SFML::Window
    SFML::System
    whisper
    nlohmann_json::nlohmann_json
)

add_executable(Arya src/main.cpp)
target_include_directories(Arya PRIVATE includes)
target_link_libraries(Arya
    ${OpenCV_LIBS}
    GraphicalGameLib
    whisper
    nlohmann_json::nlohmann_json
)

set_target_properties(Arya PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
