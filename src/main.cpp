#include <iostream>
#include <random>

#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

#include <SGLEngine.hpp>
#include <ShaderManager.hpp>

#include "LinReg.hpp"
#include "WindowFuncs.hpp"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const char *glsl_version = "#version 150";

int main(int, char **)
{
    std::ios_base::sync_with_stdio(0);
    LinReg &program = LinReg::getInstance();
    program.start();
}
