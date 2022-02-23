cmake_minimum_required(VERSION 3.0.0)

#glm -------------------------------------------------------------------
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/glm)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/json)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor) # ImguiCmake

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
add_subdirectory("${CMAKE_SOURCE_DIR}/Engine")
add_subdirectory("${CMAKE_SOURCE_DIR}/Editor")
