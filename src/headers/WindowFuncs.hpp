#pragma once
#include <string>
#include <Windows.h>
#include <string>
#include <shobjidl.h>

/* Opens file selection dialogue
 * Retrieves path & selected file name
 */
bool openFile(std::string &sFilePath, std::string &sSelectedFile);

// void readCSV(std::vector<std::string> headers);
