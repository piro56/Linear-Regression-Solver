#pragma once
#include <glad\glad.h>

class VertexArray {
private:
    unsigned int VAO;
public:
    VertexArray();
    void bind();
    void unbind();
    ~VertexArray();
};
