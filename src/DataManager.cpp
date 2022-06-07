#include "DataManager.hpp"

DataManager::DataManager()
{
}
DataManager::~DataManager()
{
    if (enabledCols != nullptr) {
        delete[] enabledCols;
        enabledCols = nullptr;
    }
}
bool DataManager::loadColumns(std::string filePath) {
    if (cols.size() > 0) {
        #ifdef DATAMANAGER_DEBUG
            #if DATAMANAGER_DEBUG >= 1
                std::cout << "[ERROR] >> DataManager tried to load in columns when it already has them!" << std::endl;
            #endif
        #endif
    }
    std::ifstream csvfile;
    csvfile.open(filePath);
    std::string colString;
    std::getline(csvfile, colString);
    csvfile.close();
    if (colString.length() == 0) {
        #ifdef DATAMANAGER_DEBUG
            #if DATAMANAGER_DEBUG >= 2
                std::cout << "[WARNING] >> DataManager failed to load columns from " << filePath << std::endl;
            #endif
        #endif
        csvfile.close();
        return false;
    }
    std::istringstream ss(colString);
    while (ss) {
        std::string colName;
        if (!std::getline(ss, colName, ',')) break;
        this->cols.push_back(colName);
    }
    this->totalCols = cols.size();
    #ifdef DATAMANAGER_DEBUG
        #if DATAMANAGER_DEBUG >= 3
            std::cout << "[INFO] Loaded in " << totalCols << " columns." << std::endl;
        #endif
    #endif
    this->enabledCols = new bool[totalCols];
    std::fill(enabledCols, enabledCols + totalCols, false);
    this->dataPath = filePath;
    return true;
}
void DataManager::reset() {
    this->cols.clear();
    this->data.clear();
    this->dependentData.clear();
    delete[] enabledCols;
    this->enabledCols = nullptr;
}
bool DataManager::loadCSV() {

    std::ifstream csvfile;
    csvfile.open(dataPath);
    std::string buffer;
    std::getline(csvfile, buffer);  // get first line
    std::vector<std::string> newColNames;

    // Initialize vector w/ enabled columns
    for (int i = 0; i < totalCols; i++) {
        if (this->enabledCols[i]) {
            this->data.push_back(std::vector<float>());
            newColNames.push_back(cols[i]);
        }
        if (this->dependentCol == i) {
            dependentName = cols[i];
        }
    }
    while (csvfile) {
        std::string row;
        if (!std::getline(csvfile, row)) break;
        std::istringstream ss(row);
        int currCol = 0;
        int enabledCol = 0;
        while (ss) {
            std::string dataVal;
            if (!getline(ss, dataVal, ',')) break;
            if (enabledCols[currCol]) {
                float val;
                try {
                    val = std::stof(dataVal);
                } catch (std::invalid_argument) {
                    val = 0;
                }
                data[enabledCol].push_back(val);
                enabledCol++;
            }
            else if (currCol == dependentCol) {
                float val;
                try {
                    val = std::stof(dataVal);
                } catch (std::invalid_argument) {
                    val = 0;
                }
                dependentData.push_back(val);
            }
            currCol++;
        }
    }
    csvfile.close();
    cols = newColNames;
    this->addedTraits = std::vector<DataTrait>(cols.size());
    this->totalCols = cols.size();
    // initialize all columns
    return true;
}

void DataManager::applyTraits() {
    if (data.size() > totalCols) {
        cols.erase(cols.begin() + totalCols, cols.end());
        data.erase(data.begin() + totalCols, data.end());
    }
    for (int i = 0; i < totalCols; i++) {
        relatedFeatures[i] = std::vector<int>();
        if (addedTraits[i].meanNormalization) {
            // Mean normalize column
            mean_normalize(i);
            for (int j = 0; j < data[i].size(); j++) {
                std::cout << data[i][j] << ", ";
            }
            std::cout << std::endl;
        }
        if (addedTraits[i].squared) {
            cols.push_back(cols[i] + "²-SQUARED");
            data.push_back(std::vector<float>());
            add_feature(i, data.back(), [](const float x) {return x * x;});
            relatedFeatures.find(i)->second.push_back(data.size()-1);
        }
        if (addedTraits[i].cubed) {
            cols.push_back(cols[i] + "³-CUBED");
            data.push_back(std::vector<float>());
            add_feature(i, data.back(), [](const float x) {return (float) pow(x,3);});
            relatedFeatures.find(i)->second.push_back(data.size()-1);
        }
    }
}

void DataManager::mean_normalize(int col) {
    float mean = 0;
    float standardDev;
    mean = std::accumulate(std::begin(data[col]), std::end(data[col]), 0);
    mean /= data[col].size();
    for (int i = 0; i < data[col].size(); i++) {
        standardDev += pow(data[col][i] - mean,2);
    }
    standardDev = sqrt(standardDev/data[col].size());
    // Now we mean normalize >:)
    for (int i = 0; i < data[col].size(); i++) {
        float normalizedVal = (data[col][i] - mean) / standardDev;
        data[col][i] = normalizedVal;
    }
}

void DataManager::add_feature(int origin, std::vector<float>& dest, float (*mathFunc)(const float orig)) {
    dest.clear();
    dest.resize(data[origin].size());
    for (int i = 0; i < dest.size(); i++) {
        dest[i] = mathFunc(data[origin][i]);
    }
}