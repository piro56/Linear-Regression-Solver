project(imgui)

file (GLOB imgui_src "*.cpp")
add_library(${PROJECT_NAME} ${imgui_src})
include_directories(../GLFW/include ../glm)
target_include_directories(${PROJECT_NAME} 
    PUBLIC .
    PUBLIC ../glm
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC OpenGL::GL
    PRIVATE glad
    PUBLIC glfw3 ${GLFW_LIBRARIES}
)