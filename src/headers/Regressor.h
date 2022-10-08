#include <math.h>
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>

#include "DataManager.hpp"

/*
 * Runs Linear Regression and returns outputs
 */
class Regressor {
public: 
    Regressor(DataManager* dataManager);
    bool isRunning() const;
    void startRegresion();
    std::vector<float> thetas;

    float error;
private:
    bool threadRunning;
    DataManager* dataManager;
    std::mutex drawLock;
    std::thread* worker;
};