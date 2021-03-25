cmake_minimum_required (VERSION 3.10.2)
project(FileRename)
install(DIRECTORY DESTINATION "out")

file(GLOB imgui "./libs/imgui/*.h" "./libs/imgui/*.cpp")
file(GLOB src "./src/*.h" "./src/*.cpp")

set(CMAKE_CXX_STANDARD 14)
set(EXECUTABLE_OUTPUT_PATH "out")
set(sources ${src} ${imgui})

find_package(Boost COMPONENTS regex system filesystem REQUIRED)

find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories("./include" "./libs/imgui" ${SDL2_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})
add_executable(FileRename ${sources})


target_link_libraries(FileRename ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_REGEX_LIBRARY}
								${SDL2_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})
