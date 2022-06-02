project(SimpleGL)

# Add a library with the below sources

file(GLOB simplegl_SRC "src/*.cpp")
add_library(${PROJECT_NAME} ${simplegl_SRC})
add_library(sub::simplegl ALIAS ${PROJECT_NAME})
include_directories(../GLFW/include ../glm)
target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ../glm
)

target_link_libraries(${PROJECT_NAME} 
    PUBLIC OpenGL::GL
    PRIVATE glad
    PUBLIC glfw3 ${GLFW_LIBRARIES}
)