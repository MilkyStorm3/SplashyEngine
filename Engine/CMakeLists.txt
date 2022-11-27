include(vendor/vendors.cmake)

file(GLOB_RECURSE ENGINE_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
    )

if(LINUX) # engine is build as shared library only for linux
    add_library(Engine SHARED ${ENGINE_SRC})
    target_compile_definitions(Engine PRIVATE SPL_BUILD_SHARED)
else()
    add_library(Engine STATIC ${ENGINE_SRC})
    
endif()

target_precompile_headers(Engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Pch/Pch.h)

set_property(TARGET Engine PROPERTY CXX_STANDARD 17)
set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(Engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Pch)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Splashy)

target_link_libraries(Engine PRIVATE glfw) 
target_link_libraries(Engine PRIVATE libglew_static)
target_link_libraries(Engine PRIVATE assimp::assimp)

if(LINUX)
    target_link_libraries(Engine PRIVATE libGL.so) 
endif()

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

# debug specific compile definitions
set(ENGINE_DEBUG_COMPILE_DEFINITIONS SPL_DEBUG)

list(APPEND ENGINE_DEBUG_COMPILE_DEFINITIONS SPL_ENABLE_LOGGING)
list(APPEND ENGINE_DEBUG_COMPILE_DEFINITIONS SPL_ENABLE_PROFILING)
list(APPEND ENGINE_DEBUG_COMPILE_DEFINITIONS SPL_ENABLE_ASSERTS)
list(APPEND ENGINE_DEBUG_COMPILE_DEFINITIONS SPL_ENABLE_RENDERER_STATS)

    FOREACH(DEF ${ENGINE_DEBUG_COMPILE_DEFINITIONS})
        target_compile_definitions(Engine PRIVATE "$<$<CONFIG:DEBUG>:${DEF}>")        
        target_compile_definitions(Engine PRIVATE "$<$<CONFIG:RELWITHDEBINFO>:${DEF}>")        
    ENDFOREACH()    

#general compile definitions
target_compile_definitions(Engine PRIVATE SPL_CORE)