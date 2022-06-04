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
    return true;
}