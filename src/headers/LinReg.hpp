#pragma once
#include <iostream>
#include <vector>
#include <array>
#include <math.h>
#include <limits>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <implot.h>

#include <SGLEngine.hpp>
#include <ShaderManager.hpp>

#include "DataManager.hpp"
#include "WindowFuncs.hpp"

int LR_process_input(GLFWwindow *window);

class LinReg
{
public:
    const int SCREEN_WIDTH = 800;
    const int SCREEN_HEIGHT = 600;
    const char *glsl_version = "#version 150";
    int LIN_REG_ERROR = 0;
    int input;

    /* GUI VARIABLES */
    bool fileButton; // Pressed to load in data
    bool displayDataWindow = false;
    bool dependentVariableSelected = false;
    int depVarSelection = 0;
    bool columnSelected = false;
    bool selectedData = false;
    bool dataLoaded = false;
    std::vector<char> plottedCols;
    std::vector<float> thetas;
    std::vector<float> plottedPoints;
    bool lineCalculated = false;
    //bool* plottedCols = nullptr;


    /* Starts Draw Loop, essentially starts program */
    void start();

    /* Draw loop */
    void drawLoop();

    /* GUI Operations */
    void displayGUI();

    void preDraw();

    void postDraw();

    /* GUI Windows*/

    void drawDataWindow();

    void basicLinearRegression(float a_size);
    float basicHthetaX(float x);
    float basicMeanSqError();

    /* Get singleton instance */
    static LinReg &getInstance()
    {
        static LinReg instance;
        return instance;
    }
    ~LinReg();

private:
    LinReg();
    SGLEngine *engine;
    ShaderManager shaderManager;
    GLFWwindow *window;
    DataManager dataManager;
    int (*process_input)(GLFWwindow *window) = LR_process_input;

public:
    // Deleted operations
    LinReg(LinReg const &) = delete;
    void operator=(LinReg const &) = delete;
};