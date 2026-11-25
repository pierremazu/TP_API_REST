cmake_minimum_required(VERSION 3.20)
project(tp_rest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug)
endif()

# -DTP_SOLUTION=ON pour compiler la solution au lieu du kit étudiant
option(TP_SOLUTION "Compiler solution/src au lieu de src" OFF)
if(TP_SOLUTION)
  set(SRC_DIR ${CMAKE_SOURCE_DIR}/solution/src)
else()
  set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
endif()

# Boost >= 1.81 (Ubuntu 24.04 : paquets libboost-dev et libboost-json-dev)
# Asio et Beast sont header-only ; seul Boost.JSON est une bibliothèque compilée.
find_package(Boost 1.81 REQUIRED COMPONENTS json)
find_package(Threads REQUIRED)

file(GLOB SOURCES CONFIGURE_DEPENDS ${SRC_DIR}/*.cpp)
add_executable(server ${SOURCES})
target_include_directories(server PRIVATE ${SRC_DIR})
target_link_libraries(server PRIVATE Boost::json Threads::Threads)
target_compile_options(server PRIVATE -Wall -Wextra -Wpedantic)
