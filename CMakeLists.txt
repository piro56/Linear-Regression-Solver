cmake_minimum_required(VERSION 3.0.0)
project(LinReg VERSION 0.1.0)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package( OpenGL REQUIRED )
# glm
set(GLM_LIB_NAME "glm")
set(GLM_INC_PATH dep/glm)
# Add subdirectories
add_subdirectory(src)
add_subdirectory(dep/SimpleGL)
add_subdirectory(dep/IMGUI)
add_subdirectory(dep/IMPLOT)

add_subdirectory(dep/GLAD)

