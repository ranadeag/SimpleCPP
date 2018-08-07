cmake_minimum_required(VERSION 3.1)
project(SimpleCPP)

set (CMAKE_CXX_STANDARD 14)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/modules")

find_package(OpenGL REQUIRED)

include (FindPkgConfig)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
add_subdirectory(external/glm)

find_package(GLEW REQUIRED)
pkg_check_modules (FRIBIDI fribidi)
include_directories(${FRIBIDI_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS})

set(USE_SYSTEM_GLEW OFF CACHE BOOL "" FORCE)
add_subdirectory(external/QuesoGLC)

GET_FILENAME_COMPONENT(ProjectRoot . ABSOLUTE)
GET_FILENAME_COMPONENT(SourceRoot . ABSOLUTE)
# SET(SourceRoot ${ProjectRoot}/)

SET(EXECUTABLE_OUTPUT_PATH Output/bin/)
SET(LIBRARY_OUTPUT_PATH Output/lib/)

FILE(GLOB SimpleCPP_cpp ${SourceRoot}/src/*.cpp)
FILE(GLOB SimpleCPP_h ${SourceRoot}/include/*.h ${SourceRoot}/include/simplecpp )

add_subdirectory(${ProjectRoot}/external/soil)

include_directories(
  ${OPENGL_INCLUDE_DIRS} 
  ${glfw3_INCLUDE_DIRECTORIES}
  ${GLM_INCLUDE_DIRS}
  ${SourceRoot}/include 
  ${ProjectRoot}/external/soil/inc
  ${ProjectRoot}/external/QuesoGLC/include
)

ADD_LIBRARY(SimpleCPP STATIC ${SimpleCPP_cpp})

target_link_libraries(SimpleCPP ${OPENGL_LIBRARIES} glfw glm ${GLEW_LIBRARIES} GLC ${FRIBIDI_LIBRARY})
# if(NOT APPLE)
# 	target_link_libraries(SimpleCPP ${GLEW_LIBRARIES})
# endif(NOT APPLE)

# add_executable(LineReconstructionTest ${SourceRoot}/dummy.cpp ${SimpleCPP_cpp} ${SimpleCPP_h})
# target_link_libraries(LineReconstructionTest Eigen3::Eigen ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES} ${GUROBI_LIBRARIES} ${Boost_LIBRARIES} )

# add_executable(LineReconstruction ${SourceRoot}/main.cpp ${SimpleCPP_cpp} ${SimpleCPP_h})
# target_link_libraries(LineReconstruction Eigen3::Eigen ${OPENGL_LIBRARIES} glfw ${GLEW_LIBRARIES} ${GUROBI_LIBRARIES} ${Boost_LIBRARIES} )
