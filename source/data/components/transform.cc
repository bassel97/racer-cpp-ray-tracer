#include "transform.h"

racer::Transform::Transform()
{
    ResetTransform();
}

void racer::Transform::ResetTransform()
{
    position = glm::vec3(0.0);
    rotation = glm::vec3(0.0);
    scale = glm::vec3(0.0);
}