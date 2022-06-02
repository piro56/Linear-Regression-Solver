#pragma once
#include <glad/glad.h>  // Manages function pointers
#include "ShaderProgram.hpp"
#include <vector>
#include <string>

/* ShaderManager
 * loads ShaderPrograms and stores them in a vector to be referenced by their names later
 * load: (name).vs and (name).fs, from shaders directory in same dir as the .exe
 * Can also load given a path directly to the source vertex and fragment shader.
 * 
 * TODO: Use hashmap to reference faster?
 */

class ShaderManager {
private:
    std::vector<ShaderProgram> shaders;
    std::string DEFAULT_PATH;
public:
    ShaderProgram* getShader(std::string shadername);
    bool checkShader(std::string shadername);

    void load_shader(std::string shadername);
    void load_shader_from_path(const char* pathvs, const char* pathfs);
    //void load_compute_shader(std::string shadername);
    //bool unload_shader(std::string shadername);
    ShaderManager();
};