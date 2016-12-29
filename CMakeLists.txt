cmake_minimum_required(VERSION 2.8)
project(cppgo)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -static-libstdc++ -static-libgcc -Wl,--no-as-needed -pthread   -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -pthread")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin/")

file(GLOB_RECURSE SOURCES "src/*.cpp" "include/*.h")
add_executable(${PROJECT_NAME} ${SOURCES})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(CURL)

include(FindPkgConfig)
pkg_search_module(SDL2 REQUIRED sdl2)
pkg_search_module(SDL2_IMAGE REQUIRED SDL2_image)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include"
        ${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS}
        ${CURL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME}
        ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES}
        ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES}
        ${CURL_LIBRARIES})
