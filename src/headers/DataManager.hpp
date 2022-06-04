#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

// 1 Errors, 2 Warnings, 3 Info
#define DATAMANAGER_DEBUG 3

class DataManager
{
public:
    DataManager();
    ~DataManager();
    bool loadCSV();
    bool loadColumns(std::string filepath);
    void reset();
    std::string dataPath;

    int getTotalCols() { return totalCols; }
    bool* enabledCols;
    int dependentCol;

    std::vector<std::string> cols;
    std::vector<std::vector<float>> data;
    std::vector<float> dependentData;
    std::string dependentName = "";
private:
    int totalCols = 0;


};