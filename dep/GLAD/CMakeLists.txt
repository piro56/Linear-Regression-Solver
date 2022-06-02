project(Glad)

add_library(glad include/glad/glad.h include/glad.c)

target_include_directories(glad PUBLIC include/)
