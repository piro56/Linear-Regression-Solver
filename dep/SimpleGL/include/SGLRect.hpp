#pragma once
#include <glad/glad.h>
#include <iostream>

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp"

#include "ShaderProgram.hpp"
#include "ShaderManager.hpp"
#include "VertexArray.hpp"
#include "BufferObject.hpp"



/* RECTANGLE
 * Buffer of 12 floats, 3 per vertex (4 points total)
 * Uses EBO to draw.
 * 
 * With a shader program, the vertex shader sets "transform" mat4 from glm
 * Be sure to have a uniform mat4 in your vertex shader
 * Also modifies "aColor" to set color of rectangle
 * 
 * Without a shader, all this will do is have 1 vertex attrib ptrs
 * Layout 0: position
 * 
 * With a valid shaderpointer, transform can scale, rotate, and move the triangle.
 */

class SGLRect {
private:
VertexArray vao;
BufferObject ebo = BufferObject(GL_ELEMENT_ARRAY_BUFFER);
BufferObject vbo = BufferObject(GL_ARRAY_BUFFER);   // vertices, colors
float buffer[12] = {};   // xyz
unsigned int indices[6] = {};   // for EBO
float x = 0.0f;     // position x
float y = 0.0f;     // position y 
float scale = 1.0f;
float width;
float height;
float rotation = 0.0f;
float r,g,b = 0.0f;
glm::mat4 transform;
bool transUpdated = false;
ShaderProgram* shader = NULL;


public:
SGLRect(float width, float height);
SGLRect(float width, float height, ShaderProgram* sp);

/* Setters that modify the rectangle. */
void setColor(const float r, const float g, const float b);
void setScale(const float scale);
void setRotation(const float rot);
void setPosition(const float x, const float y);

/* Does not bind shader, binds VAO then draws.
 * Updates transform matrix with scale, rot, and pos if needed */
void draw();

float getHeight() const { return height; }
float getWidth() const { return width; }

};

