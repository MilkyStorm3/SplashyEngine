cmake_minimum_required(VERSION 3.16.0)
project(SplashyEngine VERSION 0.1.0)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

if(UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if(NOT LINUX AND NOT WIN32)
    message(FATAL_ERROR "Unsupported platform!")
endif()

# global dependencies
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/glm)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/json)
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor) # ImguiCmake

# requiered include paths for imgui user configs
target_include_directories(ImGui PUBLIC "${CMAKE_SOURCE_DIR}/Engine/src/Splashy")
target_include_directories(ImGui PUBLIC "${CMAKE_SOURCE_DIR}/Engine/src/Splashy/Configs")
target_compile_definitions(ImGui PUBLIC IMGUI_USER_CONFIG="ImguiUserConfig.hpp")

# projects
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
add_subdirectory("${CMAKE_SOURCE_DIR}/Engine")
add_subdirectory("${CMAKE_SOURCE_DIR}/Editor")

file(COPY ${CMAKE_SOURCE_DIR}/assets
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})


