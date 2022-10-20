project(ImGui)

file(GLOB_RECURSE imgui_src ${PROJECT_SOURCE_DIR}/imgui/*.cpp) #all cpp

file(GLOB_RECURSE forbidden_imgui_src 
${PROJECT_SOURCE_DIR}/imgui/backends/*.cpp
${PROJECT_SOURCE_DIR}/imgui/examples/*.cpp
${PROJECT_SOURCE_DIR}/imgui/misc/*.cpp
) #excluded cpp

file(GLOB_RECURSE imgui_src_add 
    ${PROJECT_SOURCE_DIR}/imgui/backends/*glfw*.cpp
    ${PROJECT_SOURCE_DIR}/imgui/backends/*opengl3*.cpp
    ) #add glfw and opengl specific cpp again
    
list(REMOVE_ITEM imgui_src ${forbidden_imgui_src})
list(APPEND imgui_src ${imgui_src_add})

add_library(ImGui STATIC ${imgui_src})

set_property(TARGET ImGui PROPERTY CXX_STANDARD 17)
set_target_properties(ImGui PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(ImGui PUBLIC ${PROJECT_SOURCE_DIR}/imgui)
target_compile_definitions(ImGui PRIVATE IMGUI_PROJECT=1)

target_link_libraries(ImGui PRIVATE glfw)
