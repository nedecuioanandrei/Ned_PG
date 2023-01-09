#include "Camera.hpp"

namespace gps {

// Camera constructor
Camera::Camera(glm::vec3 cameraPosition,
               glm::vec3 cameraTarget,
               glm::vec3 cameraUp) {
  this->cameraPosition = cameraPosition;
  this->cameraTarget = cameraTarget;
  this->cameraUpDirection = cameraUp;
  this -> cameraFrontDirection = glm::vec3(0, 0, -1.0f);
}

// return the view matrix, using the glm::lookAt() function
glm::mat4 Camera::getViewMatrix() {
   return glm::lookAt(cameraPosition, cameraTarget+cameraFrontDirection, cameraUpDirection);
}

glm::vec3 Camera::getPosition() {
    return cameraPosition;
}

glm::vec3 Camera::getTarget() {
    return cameraTarget;
}

glm::vec3 Camera::getUp() {
    return cameraUpDirection;
}

// update the camera internal parameters following a camera move event
void Camera::move(MOVE_DIRECTION direction, float speed) {
  // TODO
  switch (direction) {
    case MOVE_FORWARD:
            // this -> cameraPosition += speed * this -> cameraFrontDirection;
            // this -> cameraTarget += speed * this -> cameraFrontDirection;
            cameraPosition += speed * cameraTarget;
            break;
    case MOVE_BACKWARD:
            // this -> cameraPosition -= speed * this -> cameraFrontDirection;
            // this -> cameraTarget -= speed * this -> cameraFrontDirection;
            cameraPosition -= speed * cameraTarget;
            break;
    case MOVE_RIGHT:
            // this -> cameraPosition += speed * this -> getRight();
            // this -> cameraTarget += speed * this -> getRight();
            cameraPosition += speed * glm::normalize(glm::cross(cameraTarget, cameraUpDirection));
            break;
    case MOVE_LEFT:
            // this -> cameraPosition -= speed * this -> getRight();
            // this -> cameraTarget -= speed * this -> getRight();
            cameraPosition -= speed * glm::normalize(glm::cross(cameraTarget, cameraUpDirection));
            break;
    case MOVE_UP:
            // this -> cameraPosition += speed * this -> cameraUpDirection;
            // this -> cameraTarget += speed * this -> cameraUpDirection;
            cameraPosition.y += speed;
            cameraTarget.y += speed;
            break;
    case MOVE_DOWN:
            // this -> cameraPosition -= speed * this -> cameraUpDirection;
            // this -> cameraTarget -= speed * this -> cameraUpDirection;
            cameraPosition -= speed;
            cameraTarget -= speed;
            break;
    default: break;
  }
}


float yaw;
float pitch;
bool first = true;
float lastX, lastY;

// update the camera internal parameters following a camera rotate event
// yaw - camera rotation around the y axis
// pitch - camera rotation around the x axis
void Camera::rotate(float xpos, float ypos) {
           //TODO
        //cameraTarget.x += pitch;
        //cameraTarget.y += yaw;

        //printf("%f : %f\n", xpos, ypos);
        if (first)
        {
            lastX = xpos;
            lastY = ypos;
            first = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.2f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
        	pitch = 89.0f;
        if (pitch < -89.0f)
        	pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        //front.x = cos(glm::radians(yaw));
        //front.y = 0;
        //front.z = sin(glm::radians(yaw));
        //cameraTarget= glm::normalize(direction);
        cameraTarget= glm::normalize(direction);
        //cameraTarget = cameraPosition + front;
        //cameraFrontDirection = glm::normalize(cameraPosition - cameraTarget);

        //glm::vec3 aux = glm::cross(cameraFrontDirection, )
        //forward camera - direction
        //foeward produs vect axa y = right direction
        //forward produs vect = camera up
}
}  // namespace gps
