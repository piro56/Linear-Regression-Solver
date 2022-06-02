#pragma once
#include <glad/glad.h>  // Manages function pointers
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <windows.h>

/*
 * Compiles and links vertex and fragment shader code together
 * Acts as an abstracted shader program.
 */

class ShaderProgram {
private:
    char* shaderData;
    std::string shaderName;
    unsigned int shaderProgramID;
    bool loaded;
public:

    void compileCheck(unsigned int vertexShader, bool vertex);
    void use();
    void load(const char* vertexPath, const char* fragPath);
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;
    void setVec2Float(const std::string &name, float value_one, float value_two) const;
    void setVec3Float(const std::string &name, float value_one, float value_two, float value_three) const;
    void setVec4Float(const std::string &name, float value_one, float value_two, float value_three, float value_four) const;
    void setMatrix4f(const std::string &name, GLboolean transpose, const GLfloat* value) const;
    inline static std::string getexepath() {
        char result[ MAX_PATH ];
        return std::string( result, GetModuleFileName( NULL, result, MAX_PATH ) );
    }
    
    std::string getName() { return shaderName; }
    void setName(std::string name) { shaderName = name; }
    ShaderProgram(std::string vertexShaderPath, std::string fragShaderPath);
    ShaderProgram(const char* vertexPath, const char* fragPath);
    ShaderProgram(std::string sourcePath);
    ShaderProgram();

    // unsure if we need this
    //~ShaderProgram();

    bool isLoaded() { return loaded; }
};