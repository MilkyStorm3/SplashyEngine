cmake_minimum_required(VERSION 3.16.0)
project(SplashyEngine VERSION 0.1.0)
set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)

if(NOT DEFINED CMAKE_BUILD_TYPE)
    message(FATAL_ERROR "NO BUILD TYPE SET")    
endif()

if(UNIX AND NOT APPLE)
set(LINUX TRUE)
endif()

if(LINUX) # engine is build as shared library only for linux
    set(ENGINE_SHARED_BUILD ON)
else()
    set(ENGINE_SHARED_BUILD OFF)
endif()

if (WIN32)    
    if(ENGINE_SHARED_BUILD)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif(ENGINE_SHARED_BUILD)
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


