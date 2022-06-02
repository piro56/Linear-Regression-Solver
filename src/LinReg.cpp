#include "LinReg.hpp"

LinReg::LinReg()
{
    engine = new SGLEngine(SCREEN_WIDTH, SCREEN_HEIGHT);
    if (engine->createWindow("Linear Regression") == -1)
        LIN_REG_ERROR = -1; // engine handles window creation

    window = engine->get_window();

    // GUI
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImGui::SetNextWindowSize(ImVec2(SCREEN_HEIGHT / 4, SCREEN_WIDTH / 4));
}

LinReg::~LinReg()
{
    delete engine;
}

void LinReg::start()
{
    drawLoop();
}

/* Draw loop */
void LinReg::drawLoop()
{
    while (!glfwWindowShouldClose(window))
    {
        preDraw();
        displayGUI();
        postDraw();
    }
    // Cleanup ImGUI
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    ImPlot::DestroyContext();
}

void LinReg::displayGUI()
{
    // GUI
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    {
        ImGui::Begin("Linear Regression");
        fileButton = ImGui::Button("Open Data");
        ImGui::End();
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void LinReg::preDraw()
{
    input = process_input(window);
    engine->clearWindow(0.1, 0.1, 0.2);
}

void LinReg::postDraw()
{
    if (fileButton)
    {
        std::string fPath;
        std::string fSelected;
        bool fileOpened = openFile(fPath, fSelected);
        if (fileOpened)
        {
            std::cout << "Opened: " << fPath << std::endl;
        }
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

int LR_process_input(GLFWwindow *window)
{
    // ESC -> Close Window
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
        return -1;
    }
    return 0;
}