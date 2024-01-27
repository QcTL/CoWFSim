cmake_minimum_required(VERSION 3.27)
project(CityOfWeirdFishes)

set(CMAKE_CXX_STANDARD 17)



if(WIN32)
include_directories("C:/Program Files/SFML-2.6.1/include")
link_directories("C:/Program Files/SFML-2.6.1/lib")
elseif(UNIX AND NOT APPLE)
include_directories("path/to/linux/SFML/include")
link_directories("path/to/linux/SFML/lib")
endif()

add_executable(CityOfWeirdFishes main.cpp)

target_link_libraries(CityOfWeirdFishes sfml-graphics sfml-window sfml-system)