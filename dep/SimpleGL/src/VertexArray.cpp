#include "VertexArray.hpp"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &VAO);
}

void VertexArray::bind() {
    glBindVertexArray(this->VAO);
}

void VertexArray::unbind() {
    glBindVertexArray(0);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &this->VAO);
}

