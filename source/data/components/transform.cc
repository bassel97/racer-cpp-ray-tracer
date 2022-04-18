#include "transform.h"

racer::Transform::Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
    position_ = position;
    rotation_ = rotation;
    scale_ = scale;
}

void racer::Transform::ResetTransform()
{
    position_ = glm::vec3(0.0);
    rotation_ = glm::vec3(0.0);
    scale_ = glm::vec3(0.0);
}