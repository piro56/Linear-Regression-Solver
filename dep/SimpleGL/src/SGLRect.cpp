#include "SGLRect.hpp"

SGLRect::SGLRect(float width, float height) {
    this->width = width;
    this->height = height;
    float halfw = width/2;
    float halfh = height/2;
    float arr[] = {
        -halfw, -halfh, 0.0f,   // bottom left
        halfw, -halfh, 0.0f,    // bottom right
        halfw, halfh, 0.0f,     // top right
        -halfw, halfh, 0.0f     // top left
    };
    std::copy(arr, arr+12, buffer);
    
    float arr2[] = {
        0, 1, 2,    // bottom right triangle 
        2, 3, 0     // top left triangle
    };
    std::copy(arr2, arr2+6, indices);
    vao.bind();
    vbo.bind();
    //glGenBuffers(1, &EBO);
    vbo.setBufferData(sizeof(buffer), buffer, GL_DYNAMIC_DRAW);
    
    ebo.bind();
    ebo.setBufferData(sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
}

SGLRect::SGLRect(float width, float height, ShaderProgram* sp) {
    this->width = width;
    this->height = height;
    float halfw = width/2;
    float halfh = height/2;
    float arr[] = {
        -halfw, -halfh, 0.0f,   // bottom left
        halfw, -halfh, 0.0f,    // bottom right
        halfw, halfh, 0.0f,     // top right
        -halfw, halfh, 0.0f     // top left
    };
    std::copy(arr, arr+12, buffer);

    float arr2[] = {
        0, 1, 2,    // bottom right triangle 
        2, 3, 0     // top left triangle
    };
    std::copy(arr2, arr2+6, indices);
    vao.bind();
    vbo.bind();
    //glGenBuffers(1, &EBO);
    vbo.setBufferData(sizeof(buffer), buffer, GL_DYNAMIC_DRAW);

    ebo.bind();
    ebo.setBufferData(sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    this->transform = glm::mat4(1.0f);
    this->shader = sp;
    this->shader->use();
    this->shader->setMatrix4f("transform", GL_FALSE, glm::value_ptr(transform));
}

void SGLRect::draw() {
    vao.bind();
    ebo.bind();
    if (transUpdated) {
        transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(x, y, 0.0f));
        transform = glm::rotate(transform, rotation, glm::vec3(0.0,0.0,1.0));
        transform = glm::scale(transform, glm::vec3(scale, scale, 1.0));
        shader->setMatrix4f("transform", GL_FALSE, glm::value_ptr(transform));
    }
    shader->setVec3Float("aColor", r, g, b);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void SGLRect::setScale(const float scale) {
    this->scale = scale;
    transUpdated = true;
}

void SGLRect::setPosition(const float x, const float y) {
    this->x = x;
    this->y = y;
    transUpdated = true;
}

void SGLRect::setRotation(const float rot) {
    this->rotation = rot;
    transUpdated = true;
}

void SGLRect::setColor(const float r, const float g, const float b) {
    this->r = r;
    this->g = g;
    this->b = b;
    if (shader != NULL) {
        shader->setVec3Float("aColor", r, g, b);
    }
}