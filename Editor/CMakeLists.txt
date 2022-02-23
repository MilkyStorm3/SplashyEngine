project(Editor VERSION 0.1.0)

file(GLOB_RECURSE EDITOR_SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(Editor ${EDITOR_SRC})

set_property(TARGET Editor PROPERTY CXX_STANDARD 20)
set_target_properties(Editor PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(Editor PRIVATE Engine)