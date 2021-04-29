cmake_minimum_required(VERSION 3.10.2)
project(rename-test)
install(DIRECTORY DESTINATION "output")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(EXECUTABLE_OUTPUT_PATH "output")

include_directories("../include")
add_executable(rename-test catch2-tests.cpp catch2-main.cpp)

include(FindPkgConfig)
find_package(Boost COMPONENTS system filesystem REQUIRED)

target_link_libraries(rename-test ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})
