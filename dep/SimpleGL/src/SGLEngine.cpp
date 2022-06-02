#include "SGLEngine.hpp"

void DEFAULT_FRAMEBUFFER_SIZE_CALLBACK(GLFWwindow* window, int width, int height) {
    (void) window;
    glViewport(0, 0, width, height);
}

SGLEngine::SGLEngine(int width, int height) : 
    SCREEN_WIDTH(width), SCREEN_HEIGHT(height) {
    this->framebuffer_size_callback = DEFAULT_FRAMEBUFFER_SIZE_CALLBACK;
}

/*
 * Creates a window with given width and height, sets window name.
 * Also initializes GLAD
 */

int SGLEngine::createWindow(std::string name) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, name.c_str(), NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    // Make 'window' the current context on the current thread.
    glfwMakeContextCurrent(window);        

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    // When window is resized -> call framebuffer_size_callback.
    
    glfwSetFramebufferSizeCallback(window, DEFAULT_FRAMEBUFFER_SIZE_CALLBACK);
    this->window = window;
    return 0;

}

void SGLEngine::clearWindow(float r, float g, float b) {
    glClearColor(r, g, b, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}