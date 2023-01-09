#include "Camera.hpp"

namespace gps {

// Camera constructor
Camera::Camera(glm::vec3 cameraPosition,
               glm::vec3 cameraTarget,
               glm::vec3 cameraUp) {
  this->cameraPosition = cameraPosition;
  this->cameraTarget = cameraTarget;
  this->cameraUpDirection = cameraUp;

  // TODO - Update the rest of camera parameters
  this -> cameraFrontDirection = glm::vec3(0, 0, -1.0f);
}

// return the view matrix, using the glm::lookAt() function
glm::mat4 Camera::getViewMatrix() {
   return glm::lookAt(cameraPosition, cameraTarget+cameraFrontDirection, cameraUpDirection);
}

// update the camera internal parameters following a camera move event
void Camera::move(MOVE_DIRECTION direction, float speed) {
  // TODO
  switch (direction) {
    case MOVE_FORWARD:
            this -> cameraPosition += speed * this -> cameraFrontDirection;
            this -> cameraTarget += speed * this -> cameraFrontDirection;
            break;
    case MOVE_BACKWARD:
            this -> cameraPosition -= speed * this -> cameraFrontDirection;
            this -> cameraTarget -= speed * this -> cameraFrontDirection;
            break;
    case MOVE_RIGHT:
            this -> cameraPosition += speed * this -> getRight();
            this -> cameraTarget += speed * this -> getRight();
            break;
    case MOVE_LEFT:
            this -> cameraPosition -= speed * this -> getRight();
            this -> cameraTarget -= speed * this -> getRight();
            break;
    default: break;
  }
}

// update the camera internal parameters following a camera rotate event
// yaw - camera rotation around the y axis
// pitch - camera rotation around the x axis
void Camera::rotate(float pitch, float yaw) {
    // TODO
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = -sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this -> cameraFrontDirection = glm::normalize(front);
}
}  // namespace gps
