#include "camera.h"

#include <entity.h>

glm::mat4 racer::Camera::GetViewMatrix()
{
    glm::vec4 cam_fwd = glm::yawPitchRoll(holdingEntity->transform->rotation_.y, holdingEntity->transform->rotation_.x, holdingEntity->transform->rotation_.z) * glm::vec4(0, 0, -1, 1);
    glm::vec3 cam_fwd_vec3 = glm::vec3(cam_fwd.x, cam_fwd.y, cam_fwd.z);

    glm::vec4 cam_up = glm::yawPitchRoll(holdingEntity->transform->rotation_.y, holdingEntity->transform->rotation_.x, holdingEntity->transform->rotation_.z) * glm::vec4(0, 1, 0, 1);
    glm::vec3 cam_up_vec3 = glm::vec3(cam_up.x, cam_up.y, cam_up.z);

    // camera/view transformation
    return glm::lookAt(holdingEntity->transform->position_,
                       holdingEntity->transform->position_ + cam_fwd_vec3,
                       cam_up_vec3);
}

glm::mat4 racer::Camera::GetProjectionMatrix(float frame_width, float frame_height)
{
    return glm::perspective(
        GetTanFOV() / 2.0f, ((float)frame_width / (float)frame_height), near_plane_, far_plane_);
}