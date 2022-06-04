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
    engine = nullptr;
    if (plottedCols != nullptr) {
        plottedCols;
        plottedCols = nullptr;
    }
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
        if (displayDataWindow) {
            drawDataWindow();
        }
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
        if (displayDataWindow) {
            // Reset datamanager and GUI state
            dataManager.reset();
            dependentVariableSelected = false;
            int depVarSelection = 0;
            columnSelected = false;
            selectedData = false;
            dataLoaded = false;
        }
        std::string fPath;
        std::string fSelected;
        bool fileOpened = openFile(fPath, fSelected);
        if (fileOpened)
        {
            bool loadedCols = dataManager.loadColumns(fPath);
            if (loadedCols) {
                this->displayDataWindow = true;
            }
        }
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void LinReg::drawDataWindow() {
    ImGui::Begin("Data");

    // Prompt for Dependent Variable
    if (!dependentVariableSelected) 
    {
        selectedData = false;
        ImGui::Text("Select Dependent Variable");
        if (ImGui::BeginListBox(""))
        {
            for (int i = 0; i < dataManager.getTotalCols(); i++) 
            {
                const bool is_selected = (depVarSelection == i);    // if this is selected
                if (ImGui::Selectable(dataManager.cols[i].c_str(), is_selected))
                    depVarSelection = i;
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }
        selectedData = ImGui::Button("Done");
        dependentVariableSelected = selectedData;
        dataManager.dependentCol = depVarSelection;
    }
    // Enable/Disable Columns
    else if (!columnSelected) {
        selectedData = false;
        ImGui::Text("Select Independent Variables (numerical data)");
        if (ImGui::BeginTable("Enabled Columns", 5)) {
            for (int i = 0; i < dataManager.getTotalCols(); i++) {
                if (i == depVarSelection) continue;
                ImGui::TableNextColumn();
                ImGui::Checkbox(dataManager.cols[i].c_str(), &dataManager.enabledCols[i]);
            }
            ImGui::EndTable();
            selectedData = ImGui::Button("Done");
            columnSelected = selectedData;
            if (selectedData) {
                // Load in CSV now
                if (dataManager.loadCSV()) {
                    dataLoaded = true;
                    if (plottedCols != nullptr) {
                        delete[] plottedCols;
                    }
                    plottedCols = new bool[dataManager.cols.size()];
                    std::fill(plottedCols, plottedCols + dataManager.cols.size(), false);
                }
            }
        }
    }
    else if (dataLoaded) 
    {
        ImGui::Text("View Plots");
        if (ImGui::BeginTable("Plot Cols", 5)) {
            for (int i = 0; i < dataManager.cols.size(); i++) {
                ImGui::TableNextColumn();
                ImGui::Checkbox(dataManager.cols[i].c_str(), plottedCols + i);
            }
            ImGui::EndTable();
        }
        for (int i = 0; i < dataManager.cols.size(); i++) {
            if (!plottedCols[i]) {
                continue;
            }
            if (ImPlot::BeginPlot(dataManager.cols[i].c_str())) {
                ImPlot::SetupAxes(dataManager.cols[i].c_str(),
                    dataManager.dependentName.c_str(),
                    ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_LogScale|
                    ImPlotAxisFlags_RangeFit);
                ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
                ImPlot::PlotScatter(dataManager.cols[i].c_str(), 
                    &dataManager.data[i][0], 
                    &dataManager.dependentData[0],
                    dataManager.dependentData.size());
                ImPlot::PopStyleVar();
                ImPlot::EndPlot();  
            }
        }


        // // Create buttons for plot pages
        // // Button will open a plot up w/ that var
        // // Have a bool vector of whether or not plot open
        // for (int i = 0; i < dataManager.data.size(); i++) {
        //     if (ImPlot::BeginPlot(dataManager.cols[i].c_str(), ImVec2(-1,0))) {
        //         ImPlot::SetupAxes(dataManager.cols[i].c_str(),
        //             dataManager.dependentName.c_str(),
        //             ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_LogScale|
        //             ImPlotAxisFlags_RangeFit);

        //         //ImPlot::SetupAxesLimits(0,1,0,1);
        //         ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
        //         ImPlot::PlotScatter(dataManager.cols[i].c_str(), 
        //             &dataManager.data[i][0], 
        //             &dataManager.dependentData[0],
        //             dataManager.dependentData.size());
        //         ImPlot::PopStyleVar();
        //         ImPlot::EndPlot();  
        //     }
        // }



    }

    ImGui::End();
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