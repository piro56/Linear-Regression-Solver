#include "BufferObject.hpp"

/* Generates Buffer Object with type target*/
BufferObject::BufferObject(GLenum target) {
    this->target = target;
    glGenBuffers(1, &BO);
}

/* Binds buffer to it's target */
void BufferObject::bind() {
    glBindBuffer(target, BO);
}

/* Unbinds target */
void BufferObject::unbind() {
    glBindBuffer(target, 0);
}

/* 
 * Sets the data for the buffer. Size is the size of the input data
 * usage: GL_DYNAMIC_DRAW, GL_STATIC_DRAW, etc.
 */
void BufferObject::setBufferData(GLsizeiptr size, const GLvoid* data, GLenum usage) {
        glBufferData(target, size, data, usage);
}

/* Delete buffer upon destruction */
BufferObject::~BufferObject() {
    glDeleteBuffers(1, &BO);
}