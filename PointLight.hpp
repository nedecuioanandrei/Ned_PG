#ifndef PointLight_hpp 
#define PointLight_hpp

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <iostream>

#include <string>

namespace gps {
    class PointLight {
        public:
            PointLight(
                glm::vec3 position,
                float constant,
                float linear,
                float quadratic,
                glm::vec3 ambient,
                glm::vec3 diffuse,
                glm::vec3 specular
            );

            glm::vec3 position;
            float constant;
            float linear;
            float quadratic;
            glm::vec3 ambient;
            glm::vec3 diffuse;
            glm::vec3 specular;
    };   
}

#endif