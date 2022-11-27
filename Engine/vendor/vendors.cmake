#glfw ------------------------------------------------------------------
set(GLFW_LIBRARY_TYPE STATIC)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glfw)

#glew ------------------------------------------------------------------
set(glew-cmake_BUILD_STATIC TRUE)
set(glew-cmake_BUILD_SHARED FALSE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/glew)

#stblog ----------------------------------------------------------------
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/spdlog)

#entt ------------------------------------------------------------------
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/entt)

# SPIRV-Cross ----------------------------------------------------------
set(SPIRV_CROSS_EXCEPTIONS_TO_ASSERTIONS ON)
set(SPIRV_CROSS_ENABLE_TESTS OFF)
set(SPIRV_CROSS_STATIC ON)
set(SPIRV_CROSS_SHARED OFF)
set(SPIRV_CROSS_CLI OFF)

set(SPIRV_CROSS_SKIP_INSTALL ON)

set(SPIRV_CROSS_ENABLE_GLSL ON)
set(SPIRV_CROSS_ENABLE_REFLECT ON)

set(SPIRV_CROSS_ENABLE_UTIL OFF)
set(SPIRV_CROSS_ENABLE_C_API OFF)
set(SPIRV_CROSS_ENABLE_MSL OFF)
set(SPIRV_CROSS_ENABLE_CPP OFF)
set(SPIRV_CROSS_ENABLE_HLSL OFF)

add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/vendor/spirv_cross)

# ShaderC --------------------------------------------------------------
set(SHADERC_SKIP_TESTS ON)
set(SHADERC_SKIP_EXAMPLES ON)
set(SHADERC_SKIP_INSTALL ON)
set(SHADERC_SKIP_COPYRIGHT_CHECK ON)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/vendor/shaderc)

# Assimp ---------------------------------------------------------------
set (ASSIMP_BUILD_TESTS OFF)
set (ASSIMP_INSTALL OFF)
set (ASSIMP_NO_EXPORT ON)
set (BUILD_SHARED_LIBS OFF)
add_subdirectory(${PROJECT_SOURCE_DIR}/Engine/vendor/assimp)

#stbimage --------------------------------------------------------------
add_library(stbimage STATIC ${CMAKE_CURRENT_LIST_DIR}/stbimage/stb_image_impl.cpp) 

target_include_directories(stbimage INTERFACE ${CMAKE_CURRENT_LIST_DIR}/stbimage)