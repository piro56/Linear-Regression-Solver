#include <math.h>
#include "DataManager.hpp"

// Calculates H(x): T_0 + T_1x + T_2x + ... + T_nx
float basicHthetaX(float xIndex, const DataManager& dataManager, const std::vector<float>& thetas) {
    float prediction = thetas[0];       // begin with T0
    for (int i = 0; i < dataManager.cols.size(); i++) {   // Calculate [T1x,Tnx]
        prediction += thetas[i+1] * dataManager.data[i][xIndex];
    }
    return prediction;
}
float basicMeanSqError(const DataManager& dataManager, const std::vector<float>& thetas) {
    float error = 0;
    for (int i = 0; i < dataManager.data[0].size(); i++) {
        float hx = basicHthetaX(i, dataManager, thetas);
        error += pow(hx - dataManager.dependentData[i], 2);
    }
    return (error / (2 * dataManager.data[0].size()));
}
void updateFeatures(const float a_size, std::vector<float>& temps, const DataManager& dataManager, std::vector<float>& thetas) {
        int m = dataManager.data[0].size();

        // theta0:
        float cost = 0;
        for (int i = 0; i < m; i++) {
            cost += basicHthetaX(i, dataManager, thetas) - dataManager.dependentData[i]; // prediction - actual
        }
        temps[0] = thetas[0] - (a_size / m * cost);
        // all other thetas
        for (int i = 1; i < thetas.size(); i++) {   // every feature
            if (i-1 < dataManager.getTotalCols() && !dataManager.addedTraits[i-1].enabled_linear) continue;
            
            cost = 0;
            for (int j = 0; j < m; j++) {           // each datapoint
                cost += (basicHthetaX(j, dataManager, thetas) - dataManager.dependentData[j]) * dataManager.data[i-1][j];
            }
            temps[i] = thetas[i] - (a_size / m * cost);
        }
        thetas = temps;
}
void basicLinearRegression(float a_size, DataManager& dataManager, std::vector<float>& thetas) {

    thetas = std::vector<float>(dataManager.cols.size()+1, 0.0f);
    std::vector<float> temps = std::vector<float>(dataManager.cols.size()+1, 0.0f);
    int m = dataManager.data[0].size();
    float error = std::numeric_limits<float>::infinity();
    bool run = true;
    while (run) 
    {
        updateFeatures(a_size, temps, dataManager, thetas);

        float nErr = basicMeanSqError(dataManager, thetas);
        
        if (nErr >= error) {
            std::cout << "FINAL ERROR: " << nErr << "with " << std::endl;
            for (int i = 0; i < thetas.size(); i++) {
                std::cout << "T" << i << ": " << thetas[i] << std::endl;
            }
            break;
        }
        error = nErr;
    }
    // // save plotted points now
    // outputX.clear();
    // for (int i = 0; i < m; i++) {
    //     plottedPoints.push_back(basicHthetaX(i));
    //     outputX.push_back(i);
    // }
    // lineCalculated = true;
    // leastSquaresError = error;
}

