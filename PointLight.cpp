#include "PointLight.hpp"

namespace gps {
PointLight::PointLight(glm::vec3 position,
                       float constant,
                       float linear,
                       float quadratic,
                       glm::vec3 ambient,
                       glm::vec3 diffuse,
                       glm::vec3 specular) {
  this->position = position;
  this->constant = constant;
  this->quadratic = quadratic;
  this->ambient = ambient;
  this->diffuse = diffuse;
  this->specular = specular;
}
}  // namespace gps