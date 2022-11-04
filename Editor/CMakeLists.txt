file(GLOB_RECURSE EDITOR_SRC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp
    )
add_executable(Editor ${EDITOR_SRC})

set_property(TARGET Editor PROPERTY CXX_STANDARD 20)
set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(Editor PRIVATE Engine)

set_property(TARGET Editor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${BIN_DIR})
set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)