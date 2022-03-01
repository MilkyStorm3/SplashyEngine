cmake_minimum_required(VERSION 3.16.0)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "NO BUILD TYPE SET")    
endif()

# global dependencies
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/glm)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/json)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor) # ImguiCmake

# projects
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
add_subdirectory("${CMAKE_SOURCE_DIR}/Engine")
add_subdirectory("${CMAKE_SOURCE_DIR}/Editor")

file(COPY ${CMAKE_SOURCE_DIR}/assets
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


