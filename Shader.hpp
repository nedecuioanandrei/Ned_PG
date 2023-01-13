#ifndef Shader_hpp
#define Shader_hpp

#include <GL/glew.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include "format.h"

#include "PointLight.hpp"

namespace gps {

class Shader
{
public:
    GLuint shaderProgram;
    void loadShader(std::string vertexShaderFileName, std::string fragmentShaderFileName);
    void useShaderProgram();
    void setPointLight(int index, PointLight pl);
private:
    std::string readShaderFile( std::string fileName);
    void shaderCompileLog(GLuint shaderId);
    void shaderLinkLog(GLuint shaderProgramId);
};

}

#endif /* Shader_hpp */
