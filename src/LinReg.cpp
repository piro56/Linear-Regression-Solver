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
            resetData();
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
        drawDepVarPrompt();
    }
    // Enable/Disable Columns
    else if (!columnSelected) {
        drawIndVarPrompt();
    }
    else if (dataLoaded) 
    {
        /* Enable Plot Checklist */
        
        ImGui::Text("View Plots");
        if (ImGui::BeginTable("Plot Cols", 5)) {
            for (int i = 0; i < dataManager.getTotalCols(); i++) {
                ImGui::TableNextColumn();
                ImGui::Checkbox(dataManager.cols[i].c_str(), (bool*) &plottedCols[i]);
            }
            ImGui::EndTable();
        }

        /* Independent variable & Dependent Variable Plot */
        for (int i = 0; i < dataManager.getTotalCols(); i++) {
            if (!plottedCols[i]) {
                continue;
            }
            const float* lineData = nullptr;
            if (lineCalculated) {
                lineData = &plottedPoints[0];
            }
            const char* title = (dataManager.cols[i] + " and " + dataManager.dependentName).c_str();
            drawPlot(title, dataManager.cols[i].c_str(), dataManager.dependentName.c_str(), 
                    dataManager.dependentName.c_str(), &dataManager.data[i][0], 
                    &dataManager.dependentData[0], "H(x)", lineData, dataManager.dependentData.size());
                // Create Buttons
            if (ImGui::BeginTable("Enabled Columns", 5)) {
                ImGui::TableNextColumn();
                ImGui::Checkbox("Enabled", &dataManager.addedTraits[i].enabled_linear);
                ImGui::TableNextColumn();
                ImGui::Checkbox("Mean Normalization", &dataManager.addedTraits[i].meanNormalization);
                ImGui::TableNextColumn();
                ImGui::Checkbox("Squared", &dataManager.addedTraits[i].squared);
                ImGui::TableNextColumn();
                ImGui::Checkbox("Cubic", &dataManager.addedTraits[i].cubed);
                ImGui::TableNextColumn();
                ImGui::Checkbox("Sqrt", &dataManager.addedTraits[i].sqrt);
                ImGui::EndTable();
            }
        }

        if (ImGui::Button("Apply Extra Features")) {
            dataManager.applyTraits();
        }

        // Plot nth row and Y
        if (lineCalculated) {
            drawPlot("Prediction Plot", "Data Index", dataManager.dependentName.c_str(), 
                    dataManager.dependentName.c_str(), &outputX[0], 
                    &dataManager.dependentData[0], "H(x)", &plottedPoints[0], 
                    dataManager.dependentData.size());
        }
        
        // Linear Regression Button
        bool linRegButton = ImGui::Button("Start Regression");
        if (linRegButton) {
            basicLinearRegression(0.001);
        }
        if (leastSquaresError != -1) {
            ImGui::Text("Error: %f", leastSquaresError);
        }
    }
    ImGui::End();
}
void LinReg::drawDepVarPrompt() {
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

void LinReg::drawIndVarPrompt() {
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
                plottedCols.resize(dataManager.cols.size(), false); // plot enable data
            }
        }
    }
}

void LinReg::drawPlot(const char* title, const char* xAxis, 
    const char* yAxis, const char* scatterName, 
    const float* xData, const float* yData, const char* lineName, 
    const float* lineData, int entries) {
    

    if (ImPlot::BeginPlot(title)) {
        // ImPlot::SetupAxes(xAxis,
        //     yAxis,
        //     ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_LogScale|
        //     ImPlotAxisFlags_RangeFit);
        ImPlot::SetupAxes(xAxis,
            yAxis, ImPlotAxisFlags_AutoFit|ImPlotAxisFlags_LogScale);
        ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
        ImPlot::PlotScatter(yAxis, 
            xData, yData, entries);
        ImPlot::PopStyleVar();
        if (lineData != nullptr) {
        ImPlot::PlotLine(lineName, xData, lineData, entries);
        }
        ImPlot::EndPlot();  
    }
}

void LinReg::resetData() {
    // Reset datamanager and GUI state
    dataManager.reset();
    dependentVariableSelected = false;
    int depVarSelection = 0;
    columnSelected = false;
    selectedData = false;
    dataLoaded = false;
    lineCalculated = false;
    this->plottedPoints.clear();
}

float LinReg::basicMeanSqError() {
    float error = 0;
    for (int i = 0; i < dataManager.data[0].size(); i++) {
        float hx = basicHthetaX(i);
        error += pow(hx - dataManager.dependentData[i], 2);
    }
    return (error / (2 * dataManager.data[0].size()));
}

// Calculates H(x): T_0 + T_1x + T_2x + ... + T_nx
float LinReg::basicHthetaX(float xIndex) {
    float prediction = thetas[0];       // begin with T0
    for (int i = 0; i < dataManager.cols.size(); i++) {   // Calculate [T1x,Tnx]
        prediction += thetas[i+1] * dataManager.data[i][xIndex];
    }
    return prediction;
}

void LinReg::basicLinearRegression(float a_size) {
    thetas = std::vector<float>(dataManager.cols.size()+1, 0.0f);
    std::vector<float> temps = std::vector<float>(dataManager.cols.size()+1, 0.0f);
    int m = dataManager.data[0].size();
    float error = std::numeric_limits<float>::infinity();
    bool run = true;
    while (run) 
    {
        // theta0:
        float cost = 0;
        for (int i = 0; i < m; i++) {
            cost += basicHthetaX(i) - dataManager.dependentData[i]; // prediction - actual
        }
        temps[0] = thetas[0] - (a_size / m * cost);

        // all other thetas
        for (int i = 1; i < thetas.size(); i++) {   // every feature
            if (i < dataManager.getTotalCols() && !dataManager.addedTraits[i-1].enabled_linear) continue;
            
            cost = 0;
            for (int j = 0; j < m; j++) {           // each datapoint
                cost += (basicHthetaX(j) - dataManager.dependentData[j]) * dataManager.data[i-1][j];
            }
            temps[i] = thetas[i] - (a_size / m * cost);
        }
        thetas = temps;
        float nErr = basicMeanSqError();
        if (nErr >= error) {
            std::cout << "FINAL ERROR: " << nErr << "with " << std::endl;
            for (int i = 0; i < thetas.size(); i++) {
                std::cout << "T" << i << ": " << thetas[i] << std::endl;
            }
            break;
        }
        error = nErr;
    }
    // save plotted points now
    outputX.clear();
    for (int i = 0; i < m; i++) {
        plottedPoints.push_back(basicHthetaX(i));
        outputX.push_back(i);
    }
    lineCalculated = true;
    leastSquaresError = error;
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