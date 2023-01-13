#ifndef Camera_hpp
#define Camera_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>
#include <string>

namespace gps {
    
    enum MOVE_DIRECTION {MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT, MOVE_UP, MOVE_DOWN};
    const float YAW = -90.f;
    const float PITCH = 0.f;
    const float SPEED = 2.5f;
    const float SENSITIVITY = 0.1f;
    const float ZOOM = 45.0f;

    class Camera
    {
    public:
        //Camera constructor
        Camera(
            glm::vec3 Position,
            glm::vec3 Front,
            glm::vec3 Up,
            glm::vec3 Right,
            glm::vec3 WorldUp,
            float yaw,
            float pitch,
            float movementSpeed,
            float mouseSensitivity,
            float zoom
        );

        //return the view matrix, using the glm::lookAt() function
        glm::mat4 getViewMatrix();
        void move(MOVE_DIRECTION direction, float speed);
        void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
        void ProcessMouseScroll(float yoffset);

    private:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Right;
        glm::vec3 Up;
        glm::vec3 WorldUp;
        float yaw;
        float pitch;
        float movementSpeed;
        float mouseSensitivity;
        float zoom;

        void updateCameraVectors() {
            // calculate the new Front vector
            glm::vec3 front;
            front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = sin(glm::radians(pitch));
            front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            Front = glm::normalize(front);
            // also re-calculate the Right and Up vector
            Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
            Up    = glm::normalize(glm::cross(Right, Front));
        }

    };
    
}

#endif /* Camera_hpp */
