#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>                 //core glm functionality
#include <glm/gtc/matrix_inverse.hpp>  //glm extension for computing inverse matrices
#include <glm/gtc/matrix_transform.hpp>  //glm extension for generating common transformation matrices
#include <glm/gtc/type_ptr.hpp>  //glm extension for accessing the internal data structure of glm types

#include "Camera.hpp"
#include "Model3D.hpp"
#include "PointLight.hpp"
#include "Shader.hpp"
#include "SkyBox.hpp"
#include "Window.h"

#include <iostream>

void _update_fps_counter(GLFWwindow* window) {
  static double previous_seconds = glfwGetTime();
  static int frame_count;
  double current_seconds = glfwGetTime();
  double elapsed_seconds = current_seconds - previous_seconds;
  if (elapsed_seconds > 0.25) {
    previous_seconds = current_seconds;
    double fps = (double)frame_count / elapsed_seconds;
    char tmp[128];
    sprintf(tmp, "opengl @ fps: %.2f", fps);
    glfwSetWindowTitle(window, tmp);
    frame_count = 0;
  }
  frame_count++;
}

// window
gps::Window myWindow;

// matrices
glm::mat4 model;
glm::mat4 view;
glm::mat4 projection;
glm::mat3 normalMatrix;

// light parameters
glm::vec3 lightDir;
glm::vec3 lightColor;

// point lights
gps::PointLight pls[] = {gps::PointLight(glm::vec3(2.f, 2.f, 2.f),
                                         1.f,
                                         1.f,
                                         1.f,
                                         glm::vec3(1.f, 0.f, 0.f),
                                         glm::vec3(1.f, 0.f, 0.f),
                                         glm::vec3(1.f, 0.f, 0.f))};

// shader uniform locations
GLuint modelLoc;
GLuint viewLoc;
GLuint projectionLoc;
GLuint normalMatrixLoc;
GLuint lightDirLoc;
GLuint lightColorLoc;

// skybox
gps::SkyBox mySkyBox;
gps::Shader skyboxShader;

// camera
gps::Camera myCamera(glm::vec3(0.0f, 0.0f, 14.0f),   // pos
                     glm::vec3(0.0f, -10.0f, 0.0f),  // front
                     glm::vec3(0.0f, 1.0f, 0.0f),    // up
                     glm::vec3(0.0f, 0.0f, 0.0f),    // right
                     glm::vec3(0.0f, 1.0f, 0.0f),    // world up
                     -90.0f,
                     0.0f,
                     2.5f,
                     0.01,
                     45.0f);

float gxpos, gypos;
float lgxpos, lgypos;

GLfloat cameraSpeed = 0.01f;

GLboolean pressedKeys[1024];

// models
gps::Model3D teapot;
GLfloat angle;

// shaders
gps::Shader myBasicShader;

GLenum glCheckError_(const char* file, int line) {
  GLenum errorCode;
  while ((errorCode = glGetError()) != GL_NO_ERROR) {
    std::string error;
    switch (errorCode) {
      case GL_INVALID_ENUM:
        error = "INVALID_ENUM";
        break;
      case GL_INVALID_VALUE:
        error = "INVALID_VALUE";
        break;
      case GL_INVALID_OPERATION:
        error = "INVALID_OPERATION";
        break;
      case GL_STACK_OVERFLOW:
        error = "STACK_OVERFLOW";
        break;
      case GL_STACK_UNDERFLOW:
        error = "STACK_UNDERFLOW";
        break;
      case GL_OUT_OF_MEMORY:
        error = "OUT_OF_MEMORY";
        break;
      case GL_INVALID_FRAMEBUFFER_OPERATION:
        error = "INVALID_FRAMEBUFFER_OPERATION";
        break;
    }
    std::cout << error << " | " << file << " (" << line << ")" << std::endl;
  }
  return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
  fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width,
          height);
  // TODO
}

void keyboardCallback(GLFWwindow* window,
                      int key,
                      int scancode,
                      int action,
                      int mode) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, GL_TRUE);
  }

  if (key >= 0 && key < 1024) {
    if (action == GLFW_PRESS) {
      pressedKeys[key] = true;
    } else if (action == GLFW_RELEASE) {
      pressedKeys[key] = false;
    }
  }
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
  // TODO
  static bool first_mouse = true;
  static double last_x, last_y;

  if (first_mouse) {
    last_x = xpos;
    last_y = ypos;
    first_mouse = false;
  }

  float x_offset = xpos - last_x;
  float y_offset = -ypos + last_y;
  gxpos = xpos;
  gypos = ypos;

  glfwSetCursorPos(window, myWindow.getWindowDimensions().width / 2,
                   myWindow.getWindowDimensions().height / 2);
}

void initOpenGLWindow() {
  myWindow.Create(1024, 768, "Ned_PG");
}

void setWindowCallbacks() {
  glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
  glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
  glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
}

void initOpenGLState() {
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
  glViewport(0, 0, myWindow.getWindowDimensions().width,
             myWindow.getWindowDimensions().height);
  glEnable(GL_FRAMEBUFFER_SRGB);
  glEnable(GL_DEPTH_TEST);  // enable depth-testing
  glDepthFunc(GL_LESS);  // depth-testing interprets a smaller value as "closer"
  glEnable(GL_CULL_FACE);  // cull face
  glCullFace(GL_BACK);     // cull back face
  glFrontFace(GL_CCW);     // GL_CCW for counter clock-wise
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void init_skybox() {
  std::vector<const GLchar*> faces;
  faces.push_back("textures/skybox/right.tga");
  faces.push_back("textures/skybox/left.tga");
  faces.push_back("textures/skybox/top.tga");
  faces.push_back("textures/skybox/bottom.tga");
  faces.push_back("textures/skybox/back.tga");
  faces.push_back("textures/skybox/front.tga");
  mySkyBox.Load(faces);
  skyboxShader.loadShader("shaders/skyboxShader.vert",
                          "shaders/skyboxShader.frag");
}

void draw_skybox() {
  skyboxShader.useShaderProgram();
  view = myCamera.getViewMatrix();
  glUniformMatrix4fv(glGetUniformLocation(skyboxShader.shaderProgram, "view"),
                     1, GL_FALSE, glm::value_ptr(view));

  projection =
      glm::perspective(glm::radians(45.0f),
                       (float)myWindow.getWindowDimensions().width /
                           (float)myWindow.getWindowDimensions().height,
                       0.1f, 1000.0f);
  glUniformMatrix4fv(
      glGetUniformLocation(skyboxShader.shaderProgram, "projection"), 1,
      GL_FALSE, glm::value_ptr(projection));
  mySkyBox.Draw(skyboxShader, view, projection);
}

void initModels() {
  teapot.LoadModel("models/testing/casa.obj");
}

void initShaders() {
  myBasicShader.loadShader("shaders/basic.vert", "shaders/basic.frag");
}

void initUniforms() {
  myBasicShader.useShaderProgram();

  // create model matrix for teapot
  model = glm::rotate(glm::mat4(1.0f), glm::radians(angle),
                      glm::vec3(0.0f, 1.0f, 0.0f));
  modelLoc = glGetUniformLocation(myBasicShader.shaderProgram, "model");

  // get view matrix for current camera
  view = myCamera.getViewMatrix();
  viewLoc = glGetUniformLocation(myBasicShader.shaderProgram, "view");
  // send view matrix to shader
  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

  // compute normal matrix for teapot
  normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
  normalMatrixLoc =
      glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");

  // create projection matrix
  projection =
      glm::perspective(glm::radians(45.0f),
                       (float)myWindow.getWindowDimensions().width /
                           (float)myWindow.getWindowDimensions().height,
                       0.1f, 20.0f);
  projectionLoc =
      glGetUniformLocation(myBasicShader.shaderProgram, "projection");
  // send projection matrix to shader
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

  // set the light direction (direction towards the light)
  lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
  lightDirLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir");
  // send light dir to shader
  glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

  // set light color
  lightColor = glm::vec3(1.0f, 1.0f, 1.0f);  // white light
  lightColorLoc =
      glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
  // send light color to shader
  glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

  // adding pointLights
  for (int index = 0; index < sizeof(pls) / sizeof(pls[0]); ++index) {
    myBasicShader.setPointLight(index, pls[index]);
  }
}

void renderTeapot(gps::Shader shader) {
  // select active shader program
  shader.useShaderProgram();

  // send teapot model matrix data to shader
  glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

  // send teapot normal matrix data to shader
  glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE,
                     glm::value_ptr(normalMatrix));

  glUniformMatrix3fv(glGetUniformLocation(shader.shaderProgram, "view"), 1,
                     GL_FALSE, glm::value_ptr(myCamera.getViewMatrix()));

  // draw teapot
  teapot.Draw(shader);
}

void renderScene() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // render the scene

  // render the teapot'
  draw_skybox();
  renderTeapot(myBasicShader);
}

void cleanup() {
  myWindow.Delete();
  // cleanup code for your own data
}

void processMovement() {
  static float time = 0;
  float current_time = glfwGetTime();
  float delta_time = current_time - time;
  time = current_time;
  // printf("delta_time : %f\n", delta_time);

  if (pressedKeys[GLFW_KEY_W]) {
    myCamera.move(gps::MOVE_FORWARD, delta_time);
    // update view matrix
    view = myCamera.getViewMatrix();
    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
  }

  if (pressedKeys[GLFW_KEY_S]) {
    myCamera.move(gps::MOVE_BACKWARD, delta_time);
    // update view matrix
    view = myCamera.getViewMatrix();
    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
  }

  if (pressedKeys[GLFW_KEY_A]) {
    myCamera.move(gps::MOVE_LEFT, delta_time);
    // update view matrix
    view = myCamera.getViewMatrix();
    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
  }

  if (pressedKeys[GLFW_KEY_D]) {
    myCamera.move(gps::MOVE_RIGHT, delta_time);
    // update view matrix
    view = myCamera.getViewMatrix();
    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
  }

  if (pressedKeys[GLFW_KEY_L]) {
    myCamera.move(gps::MOVE_UP, delta_time);
    // update view matrix
    view = myCamera.getViewMatrix();
    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
  }

  if (pressedKeys[GLFW_KEY_K]) {
    myCamera.move(gps::MOVE_DOWN, delta_time);
    // update view matrix
    view = myCamera.getViewMatrix();
    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
  }

  if (pressedKeys[GLFW_KEY_Q]) {
    angle -= 1.0f;
    // update model matrix for teapot
    model =
        glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
    // update normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
  }

  if (pressedKeys[GLFW_KEY_E]) {
    angle += 1.0f;
    // update model matrix for teapot
    model =
        glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
    // update normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
  }

  if (pressedKeys[GLFW_KEY_PAGE_UP]) {
    printf("sus");
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  if (pressedKeys[GLFW_KEY_PAGE_DOWN]) {
    printf("jos");
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  }

  myCamera.ProcessMouseMovement(
      gxpos - myWindow.getWindowDimensions().width / 2 ,
      -gypos + myWindow.getWindowDimensions().height / 2);
  
    view = myCamera.getViewMatrix();
    myBasicShader.useShaderProgram();
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    // compute normal matrix for teapot
    normalMatrix = glm::mat3(glm::inverseTranspose(view * model));

}

int main(int argc, const char* argv[]) {
  try {
    initOpenGLWindow();
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  initOpenGLState();
  initModels();
  initShaders();
  initUniforms();
  setWindowCallbacks();

  init_skybox();
  glCheckError();

  while (!glfwWindowShouldClose(myWindow.getWindow())) {
    glfwPollEvents();

    processMovement();
    renderScene();

    glfwSwapBuffers(myWindow.getWindow());
    _update_fps_counter(myWindow.getWindow());
  }

  cleanup();

  return EXIT_SUCCESS;
}
