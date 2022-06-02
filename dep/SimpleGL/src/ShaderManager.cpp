#include "ShaderManager.hpp"

ShaderProgram* ShaderManager::getShader(std::string shadername) {
    for (ShaderProgram& sp : shaders) {
        if (sp.getName() == shadername) {
            return &sp;
        }
    }
    std::cout << "[WARNING] Shader " << shadername << " was not found" << std::endl; 
    return nullptr;
}

bool ShaderManager::checkShader(std::string shadername) {
    for (ShaderProgram& sp : shaders) {
        if (sp.getName() == shadername) {
            return true;
        }
    }
    return false;
}
void ShaderManager::load_shader(std::string shadername) {
    // check if already loaded
    if (checkShader(shadername)) return;
    ShaderProgram sp;

    sp.load((DEFAULT_PATH + shadername + ".vs").c_str(), (DEFAULT_PATH + shadername + ".fs").c_str());
    if (sp.isLoaded()) {
        sp.setName(shadername);
        this->shaders.push_back(sp);
        }
}

void ShaderManager::load_shader_from_path(const char* pathvs, const char* pathfs) {
    std::string name = std::string(pathvs);
    name = name.substr(name.find_last_of("\\")+1, name.find_last_of('.'));
    if (checkShader(name)) return;
    ShaderProgram sp;
    
    sp.load(pathvs, pathfs);
        if (sp.isLoaded()) {
        sp.setName(name);
        this->shaders.push_back(sp);
    }
}

ShaderManager::ShaderManager() {
    char result[ MAX_PATH ];
    DEFAULT_PATH = std::string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
    DEFAULT_PATH = DEFAULT_PATH.substr(0, DEFAULT_PATH.find_last_of('\\')) + "\\shaders\\";
}