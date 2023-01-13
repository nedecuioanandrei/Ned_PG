#include "Camera.hpp"
#include <cmath>
#include <glm/geometric.hpp>

namespace gps {

void debug_vector(glm::vec3& a) {
  printf("%f, %f, %f \n", a.x, a.y, a.z);
}

// Camera constructor
Camera::Camera(glm::vec3 position,
               glm::vec3 front,
               glm::vec3 up,
               glm::vec3 right,
               glm::vec3 worldUp,
               float yaw,
               float pitch,
               float movementSpeed,
               float mouseSensitivity,
               float zoom) {
  Position = position;
  Front = front, Up = up, Right = right, WorldUp = worldUp;
  this->yaw = yaw;
  this->pitch = pitch;
  this->movementSpeed = movementSpeed;
  this->mouseSensitivity = mouseSensitivity;
  this->zoom = zoom;
  updateCameraVectors();
}

// // return the view matrix, using the glm::lookAt() function
glm::mat4 Camera::getViewMatrix() {
  return glm::lookAt(Position, Position + Front, Up);
}

// update the camera internal parameters following a camera move event
void Camera::move(MOVE_DIRECTION direction, float deltaTime) {
  float velo = movementSpeed / 100;
  debug_vector(Position);
  debug_vector(Front);
  debug_vector(Up);
  debug_vector(Right);
  debug_vector(WorldUp);
  printf("movement speed %f", movementSpeed);

  // TODO
  switch (direction) {
    case MOVE_FORWARD:
      // this -> cameraPosition += speed * this -> cameraFrontDirection;
      // this -> cameraTarget += speed * this -> cameraFrontDirection;
      // printf("velo: %f, mov : %f, deltaTime: %f \n", velo, deltaTime,
      // movementSpeed);
      Position += Front * velo;
      break;
    case MOVE_BACKWARD:
      // this -> cameraPosition -= speed * this -> cameraFrontDirection;
      // this -> cameraTarget -= speed * this -> cameraFrontDirection;
      Position -= Front * velo;
      break;
    case MOVE_RIGHT:
      // this -> cameraPosition += speed * this -> getRight();
      // this -> cameraTarget += speed * this -> getRight();
      Position += Right * velo;
      break;
    case MOVE_LEFT:
      // this -> cameraPosition -= speed * this -> getRight();
      // this -> cameraTarget -= speed * this -> getRight();
      Position -= Right * velo;
      break;
    case MOVE_UP:
      // this -> cameraPosition += speed * this -> cameraUpDirection;
      // this -> cameraTarget += speed * this -> cameraUpDirection;
      break;
    case MOVE_DOWN:
      // this -> cameraPosition -= speed * this -> cameraUpDirection;
      // this -> cameraTarget -= speed * this -> cameraUpDirection;
      break;
    default:
      break;
  }
}

void Camera::ProcessMouseMovement(float xoffset,
                                  float yoffset,
                                  bool constrainPitch = true) {
  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;

  yaw += xoffset;
  pitch += yoffset;

  printf("pitch: %f\n", yoffset);
  printf("yaw: %f\n", yaw);
  // make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch) {
    if (pitch > 89.0f)
      pitch = 89.0f;
    if (pitch < -89.0f)
      pitch = -89.0f;
  }

  // update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
  zoom -= (float)yoffset;
  if (zoom < 1.0f)
    zoom = 1.0f;
  if (zoom > 45.0f)
    zoom = 45.0f;
}
// update the camera internal parameters following a camera rotate event
// yaw - camera rotation around the y axis
// pitch - camera rotation around the x axis
}  // namespace gps
