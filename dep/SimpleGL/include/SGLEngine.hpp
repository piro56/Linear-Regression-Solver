#ifndef __SGL_ENGINE_H__
#define __SGL_ENGINE_H__
#include <iostream>
#include <string>
#include <glad/glad.h>  // Manages function pointers
#include <glfw3.h>      // Manages window

class SGLEngine{ 
private:
GLFWwindow* window = NULL;
public:
const int SCREEN_WIDTH;
const int SCREEN_HEIGHT;
const int VERSION_MAJOR = 4;
const int VERSION_MINOR = 3;
//void (*input_callback)(GLFWwindow* window);
void (*framebuffer_size_callback)(GLFWwindow* window, int width, int height);

SGLEngine(int width, int height);
int createWindow(std::string windowName);
GLFWwindow* get_window() { return window; }
void clearWindow(float r, float g, float b);

};
#endif