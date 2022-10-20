include(vendor/vendors.cmake)

cmake_minimum_required(VERSION 3.16.0)
project(Engine VERSION 0.1.0)
file(GLOB_RECURSE ENGINE_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)

if(ENGINE_SHARED_BUILD)
    add_library(Engine SHARED ${ENGINE_SRC})
    target_compile_definitions(Engine PUBLIC SPL_BUILD_SHARED=1)
    message("ENGINE-> Building shared engine library")
else()
    add_library(Engine STATIC ${ENGINE_SRC})
    message("ENGINE-> Building static engine library")
endif()

target_compile_definitions(Engine PRIVATE SPL_CORE=1)

target_precompile_headers(Engine PRIVATE ${PROJECT_SOURCE_DIR}/src/Pch/Pch.h)

set_property(TARGET Engine PROPERTY CXX_STANDARD 17)
set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Engine PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(Engine PRIVATE ${PROJECT_SOURCE_DIR}/src/Pch)
target_include_directories(Engine PUBLIC ${PROJECT_SOURCE_DIR}/src/Splashy)

target_link_libraries(Engine PRIVATE glfw) 
target_link_libraries(Engine PRIVATE libglew_static) 

if (LINUX)
    message(STATUS "Linux build detected using libGL.so")
    target_link_libraries(Engine PRIVATE libGL.so) 
endif ()

target_link_libraries(Engine PRIVATE EnTT::EnTT) 
target_link_libraries(Engine PRIVATE stbimage) 

target_link_libraries(Engine PRIVATE spirv-cross-core) 
target_link_libraries(Engine PRIVATE spirv-cross-reflect)
target_link_libraries(Engine PRIVATE spirv-cross-glsl) 
target_link_libraries(Engine PRIVATE shaderc_shared)

target_link_libraries(Engine PUBLIC spdlog)
target_link_libraries(Engine PUBLIC nlohmann_json)
target_link_libraries(Engine PUBLIC glm::glm)
target_link_libraries(Engine PUBLIC ImGui) 


if( ${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    target_compile_definitions(Engine PRIVATE SPL_DEBUG=1)
    
    target_compile_definitions(Engine PRIVATE SPL_ENABLE_PROFILING=1)
    target_compile_definitions(Engine PRIVATE SPL_ENABLE_ASSERTS=1)
    target_compile_definitions(Engine PRIVATE SPL_ENABLE_RENDERER_STATS=1)
    target_compile_definitions(Engine PRIVATE SPL_ENABLE_LOGGING=1)
endif()
