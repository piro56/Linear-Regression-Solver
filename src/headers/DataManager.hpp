#pragma once
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <math.h>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

// 1 Errors, 2 Warnings, 3 Info
#define DATAMANAGER_DEBUG 3

/* 
 * Applied to each column, adds columns to the regression.
 */ 
struct DataTrait {
    bool enabled_linear = true;
    bool meanNormalization = false;
    bool squared = false;
    bool sqrt = false;
    bool cubed = false;
};

class DataManager
{
public:
    DataManager();
    ~DataManager();
    bool loadCSV();
    bool loadColumns(std::string filepath);
    void reset();
    void applyTraits();

    std::string dataPath;

    int getTotalCols() { return totalCols; }
    bool* enabledCols;
    int dependentCol;

    std::vector<std::string> cols;
    std::vector<DataTrait> addedTraits;
    std::vector<std::vector<float>> data;
    std::vector<float> dependentData;
    std::unordered_map<int, std::vector<int>> relatedFeatures;
    std::string dependentName = "";
private:
    int totalCols = 0;
    void mean_normalize(int column);

    /* Adds a new feature row from existing feature */
    void add_feature(int origin, std::vector<float>& dest, float (*mathFunc)(const float orig));

};