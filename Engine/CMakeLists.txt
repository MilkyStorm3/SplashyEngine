include(vendor/vendors.cmake)

cmake_minimum_required(VERSION 3.16.0)
project(Engine VERSION 0.1.0)
file(GLOB_RECURSE ENGINE_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(Engine STATIC ${ENGINE_SRC})

target_precompile_headers(Engine PRIVATE ${PROJECT_SOURCE_DIR}/src/Pch/Pch.h)

set_property(TARGET Engine PROPERTY CXX_STANDARD 20)
set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Engine PRIVATE ${PROJECT_SOURCE_DIR}/src)
target_include_directories(Engine PRIVATE ${PROJECT_SOURCE_DIR}/src/Pch)
target_include_directories(Engine PUBLIC ${PROJECT_SOURCE_DIR}/src/Splashy)

target_link_libraries(Engine PRIVATE glfw) 
target_link_libraries(Engine PRIVATE libglew_static) 
target_link_libraries(Engine PRIVATE EnTT::EnTT) 
target_link_libraries(Engine PRIVATE stbimage) 

target_link_libraries(Engine PRIVATE spirv-cross-core) 
target_link_libraries(Engine PRIVATE spirv-cross-reflect)
target_link_libraries(Engine PRIVATE spirv-cross-glsl) 
target_link_libraries(Engine PRIVATE shaderc)

target_link_libraries(Engine PUBLIC spdlog)
target_link_libraries(Engine PUBLIC nlohmann_json)
target_link_libraries(Engine PUBLIC glm::glm)
target_link_libraries(Engine PUBLIC ImGui) 