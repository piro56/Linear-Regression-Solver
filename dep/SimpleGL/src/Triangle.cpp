#include "Triangle.hpp"

/*
 * Generate a triangle with width and height
 */
Triangle::Triangle(float width, float height) {
    this->width = width;
    this->height = height;

    float halfX = width/2;
    float halfY = height/2;
    
    // Generate default triangle x and y points (probably equilatoral)

    buffer[0] = -halfX;
    buffer[1] = -halfY;
    buffer[6] = halfX;
    buffer[7] = -halfY;
    buffer[13] = halfY;

    vao.bind();
    vbo.bind();
    vbo.setBufferData(sizeof(buffer), this->buffer, GL_DYNAMIC_DRAW);
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, false, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    this->transform = glm::mat4(1.0f);
}

/*
 * Generate a triangle with width and height, also set shaderprogram*
 */
Triangle::Triangle(float width, float height, ShaderProgram* shader) {
    this->width = width;
    this->height = height;

    float halfX = width/2;
    float halfY = height/2;
    
    // Generate default triangle x and y points (probably equilatoral)

    buffer[0] = halfX;
    buffer[1] = -halfY;
    buffer[6] = -halfX;
    buffer[7] = -halfY;
    buffer[13] = halfY;
    vao.bind();
    vbo.bind();
    vbo.setBufferData(sizeof(buffer), buffer, GL_STATIC_DRAW);
    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    
    this->transform = glm::mat4(1.0f);
    this->shader = shader;
    this->shader->use();
}


void Triangle::draw() {
    if (this->shader != NULL && this->transUpdated) {
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(x, y, 0.0f));
        transform = glm::rotate(transform, rotation, glm::vec3(0.0,0.0,1.0));
        transform = glm::scale(transform, glm::vec3(scale, scale, 1.0));
        shader->setMatrix4f("transform", GL_FALSE, glm::value_ptr(transform));
        transUpdated = false;
    }
    vao.bind();
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Triangle::setRotation(float rot) {
    this->rotation = rot;
    this->transUpdated = true;
}

void Triangle::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
    this->transUpdated = true;
}

void Triangle::setScale(float scale) {
    this->scale = scale;
    this->transUpdated = true;
}