cmake_minimum_required (VERSION 3.10.2)
project(FileRename)
install(DIRECTORY DESTINATION "exec")

file(GLOB imgui "./libs/imgui/*.h" "./libs/imgui/*.cpp")
file(GLOB src "./src/*.h" "./src/*.cpp")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(EXECUTABLE_OUTPUT_PATH "exec")
set(sources ${src} ${imgui})

include_directories("./include" "./libs/imgui")
add_executable(FileRename ${sources})

find_package(Boost COMPONENTS regex system filesystem REQUIRED)

include(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)


target_link_libraries(FileRename ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY} SDL2main SDL2)
