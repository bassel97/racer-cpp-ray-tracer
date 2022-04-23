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
    scale_ = glm::vec3(1.0);
}

glm::mat4 racer::Transform::GetRotationMatrix()
{
    // camera/view transformation
    return glm::yawPitchRoll(rotation_.y, rotation_.x, rotation_.z);
}

glm::mat4 racer::Transform::GetTransformationMatrix()
{
    glm::mat4 transform_matrix = glm::translate(glm::mat4(1), position_);
    transform_matrix *= GetRotationMatrix();
    transform_matrix = glm::scale(transform_matrix, scale_);

    return transform_matrix;
}

glm::mat4 racer::Transform::GetTSMatrix()
{
    glm::mat4 transform_matrix = glm::translate(glm::mat4(1), position_);
    transform_matrix = glm::scale(transform_matrix, scale_);

    return transform_matrix;
}