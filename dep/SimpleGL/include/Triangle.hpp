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



/* TRIANGLE
 * Buffer of 3 points each with a color assigned
 * With a shader program, the vertex shader sets "transform" mat4 from glm
 * Be sure to have a uniform mat4 in your vertex shader
 * 
 * Without a shader, all this will do is have 2 vertex attrib ptrs
 * Layout 0: position
 * Layout 1: color
 * 
 * With a valid shaderpointer, transform can scale, rotate, and move the triangle.
 */

class Triangle {
private:
VertexArray vao;
BufferObject vbo = BufferObject(GL_ARRAY_BUFFER);   // vertices, colors
float buffer[18] = {0.0f};   // xyz rgb 6 * 3
float x = 0.5f;     // position x
float y = 0.5f;     // position y
float scale = 1.0f;
float width;
float height;
float rotation = 0.0f;
bool transUpdated = false;
glm::mat4 transform;
ShaderProgram* shader = NULL;


public:
Triangle(float width, float height);
Triangle(float width, float height, ShaderProgram* sp);

void setColor(float r, float g, float b);
void setScale(float scale);
void setRotation(float rot);
void setPosition(float x, float y);
void draw();

};

