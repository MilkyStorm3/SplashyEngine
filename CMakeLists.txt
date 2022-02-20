cmake_minimum_required(VERSION 3.0.0)
project(SplashyEngine VERSION 0.1.0)

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#glfw ------------------------------------------------------------------
find_package(OpenGL REQUIRED)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/vendor/glfw)

#glew ------------------------------------------------------------------
set(glew-cmake_BUILD_STATIC TRUE)
set(glew-cmake_BUILD_SHARED FALSE)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/vendor/glew)

#stblog ----------------------------------------------------------------
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/vendor/spdlog)

#glm -------------------------------------------------------------------
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/glm)

#json ------------------------------------------------------------------
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(${PROJECT_SOURCE_DIR}/vendor/json)

#entt ------------------------------------------------------------------
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/vendor/entt)

#imgui -----------------------------------------------------------------
file(GLOB_RECURSE imgui_src ${PROJECT_SOURCE_DIR}/vendor/imgui/*.cpp) #all cpp

file(GLOB_RECURSE forbidden_imgui_src 
${PROJECT_SOURCE_DIR}/vendor/imgui/backends/*.cpp
${PROJECT_SOURCE_DIR}/vendor/imgui/examples/*.cpp
${PROJECT_SOURCE_DIR}/vendor/imgui/misc/*.cpp
) #excluded cpp

file(GLOB_RECURSE imgui_src_add 
    ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/*glfw*.cpp
    ${PROJECT_SOURCE_DIR}/vendor/imgui/backends/*opengl3*.cpp
    ) #add glfw and opengl specific cpp again
    
list(REMOVE_ITEM imgui_src ${forbidden_imgui_src})
list(APPEND imgui_src ${imgui_src_add})


# SPIRV-Cross ----------------------------------------------------------
# set(DSPIRV_CROSS_STATIC ON)
# set(DSPIRV_CROSS_SHARED OFF)
# set(DSPIRV_CROSS_CLI OFF)
# add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/vendor/spirv_cross)

# ShaderC --------------------------------------------------------------
set(SHADERC_SKIP_TESTS ON)
set(SHADERC_SKIP_EXAMPLES ON)
set(SHADERC_SKIP_COPYRIGHT_CHECK ON)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/vendor/shaderc)

# Engine ---------------------------------------------------------------
file(GLOB_RECURSE engine_SRC ${PROJECT_SOURCE_DIR}/Engine/src/*.cpp ${PROJECT_SOURCE_DIR}/Engine/vendor/stbimage/*.cpp)
add_library(Engine STATIC ${engine_SRC})

set_target_properties(Engine PROPERTIES LINKER_LANGUAGE CXX)

target_sources(Engine PUBLIC ${imgui_src})

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine/src/)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine/src/Splashy)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine/src/Pch)
set_property(TARGET Engine PROPERTY CXX_STANDARD 20)
# includes
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/stbimage/)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/spdlog/include/)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/entt/src)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/glfw/include/)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/glew/include/)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/shaderc/libshaderc/include/)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/shaderc/libshaderc_util/include/)
# target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/shaderc/third_party/spirv-tools/include/)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/)

target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Engine/src/)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/)
# libs
target_link_directories(Engine PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/Engine/vendor/glew/lib)
target_link_directories(Engine PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/Engine/vendor/spirv_cross)

target_link_libraries(Engine nlohmann_json::nlohmann_json)
target_link_libraries(Engine spdlog)
target_link_libraries(Engine glm)
target_link_libraries(Engine glewd)
target_link_libraries(Engine glfw)

target_link_libraries(Engine spirv-cross-core)
target_link_libraries(Engine spirv-cross-reflect)
# target_link_libraries(Engine spirv-cross-util)
target_link_libraries(Engine spirv-cross-glsl)
# target_link_libraries(Engine spirv-cross-cpp)

target_link_libraries(Engine shaderc)

target_link_libraries(Engine OpenGL::GL)

#Editor------------------------------------------------------------------

file(GLOB editor_SRC ${PROJECT_SOURCE_DIR}/Editor/src/*.cpp)
add_executable(Editor ${editor_SRC} )
set_property(TARGET Editor PROPERTY CXX_STANDARD 20)

set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(Editor PUBLIC ${PROJECT_SOURCE_DIR}/Engine/src)

target_link_libraries(Editor Engine)
target_include_directories(Editor PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui/)

# ADD_CUSTOM_COMMAND(TARGET main
    #   POST_BUILD
    #   COMMAND main
    #   WORKING_DIRECTORY /var/tmp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
