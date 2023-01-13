#include "Shader.hpp"

namespace gps {
std::string Shader::readShaderFile(std::string fileName) {
  std::ifstream shaderFile;
  std::string shaderString;

  // open shader file
  shaderFile.open(fileName.c_str());

  std::stringstream shaderStringStream;

  // read shader content into stream
  shaderStringStream << shaderFile.rdbuf();

  // close shader file
  shaderFile.close();

  // convert stream into GLchar array
  shaderString = shaderStringStream.str();
  return shaderString;
}

void Shader::shaderCompileLog(GLuint shaderId) {
  GLint success;
  GLchar infoLog[512];

  // check compilation info
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
    std::cout << "Shader compilation error\n" << infoLog << std::endl;
  }
}

void Shader::shaderLinkLog(GLuint shaderProgramId) {
  GLint success;
  GLchar infoLog[512];

  // check linking info
  glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "Shader linking error\n" << infoLog << std::endl;
  }
}

void Shader::loadShader(std::string vertexShaderFileName,
                        std::string fragmentShaderFileName) {
  // read, parse and compile the vertex shader
  std::string v = readShaderFile(vertexShaderFileName);
  const GLchar* vertexShaderString = v.c_str();
  GLuint vertexShader;
  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderString, NULL);
  glCompileShader(vertexShader);
  // check compilation status
  shaderCompileLog(vertexShader);

  // read, parse and compile the vertex shader
  std::string f = readShaderFile(fragmentShaderFileName);
  const GLchar* fragmentShaderString = f.c_str();
  GLuint fragmentShader;
  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderString, NULL);
  glCompileShader(fragmentShader);
  // check compilation status
  shaderCompileLog(fragmentShader);

  // attach and link the shader programs
  this->shaderProgram = glCreateProgram();
  glAttachShader(this->shaderProgram, vertexShader);
  glAttachShader(this->shaderProgram, fragmentShader);
  glLinkProgram(this->shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  // check linking info
  shaderLinkLog(this->shaderProgram);
}

void Shader::useShaderProgram() {
  glUseProgram(this->shaderProgram);
}

void Shader::setPointLight(int index, PointLight pl) {
  uint32_t plsPosition = glGetUniformLocation(
      this->shaderProgram, fmt::format("pls[{}].position", index).c_str());
  glUniform3f(plsPosition, pl.position.x, pl.position.y, pl.position.z);

  uint32_t plsConstant = glGetUniformLocation(
      shaderProgram, fmt::format("pls[{}].constant", index).c_str());
  glUniform1f(plsConstant, pl.constant);

  uint32_t plsLinear = glGetUniformLocation(
      this->shaderProgram, fmt::format("pls[{}].linear", index).c_str());
  glUniform1f(plsLinear, pl.linear);

  uint32_t plsQuadratic = glGetUniformLocation(
      this->shaderProgram, fmt::format("pls[{}].quadratic", index).c_str());
  glUniform1f(plsQuadratic, pl.quadratic);

  uint32_t plsAmbient = glGetUniformLocation(
      this->shaderProgram, fmt::format("pls[{}].ambient", index).c_str());
  glUniform3f(plsAmbient, pl.ambient.x, pl.ambient.y, pl.ambient.z);

  uint32_t plsDiffuse = glGetUniformLocation(
      this->shaderProgram, fmt::format("pls[{}].diffuse", index).c_str());
  glUniform3f(plsDiffuse, pl.diffuse.x, pl.diffuse.y, pl.diffuse.z);

  uint32_t plsSpecular = glGetUniformLocation(
      this->shaderProgram, fmt::format("pls[{}].specular", index).c_str());
  glUniform3f(plsSpecular, pl.specular.x, pl.specular.y, pl.specular.z);
  printf("Da");
}

}  // namespace gps
