project(LinReg VERSION 0.1.0)
# set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g")
add_executable(${PROJECT_NAME} main.cpp WindowFuncs.cpp LinReg.cpp)

# Copies asset directory into build

add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/shaders ${CMAKE_CURRENT_BINARY_DIR}/shaders
)

add_dependencies(${PROJECT_NAME} copy_assets)

include_directories(../dep/GLFW/include ../dep/glm ../dep/IMGUI ../dep/IMPLOT .)


target_include_directories(${PROJECT_NAME}
    PUBLIC ../dep/glm
)

# Link Libraries
# target_include_directories(${PROJECT_NAME} PUBLIC ../dep/glm)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC OpenGL::GL
    PUBLIC sub::simplegl
    PUBLIC imgui
    PUBLIC implot
    PRIVATE glad
    PUBLIC glfw3 ${GLFW_LIBRARIES}
    )
# target_link_libraries(${PROJECT_NAME} PRIVATE glad)
# target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})