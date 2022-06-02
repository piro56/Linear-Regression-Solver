#pragma once
#include <glad\glad.h>

class BufferObject {
private:
    unsigned int BO;
    GLenum target;
public:
    void bind();
    void unbind();
    void setBufferData(GLsizeiptr size,
                        const GLvoid* data, GLenum usage);
    unsigned int getBuffer() {return BO; }
    BufferObject(GLenum target);
    ~BufferObject();

};