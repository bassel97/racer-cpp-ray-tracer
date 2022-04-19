#include "triangle-renderer.h"

#include <iostream>
#include <entity.h>

racer::TriangleRendererComponent::TriangleRendererComponent()
{
    renderer_mesh_ = new PrimitiveModel(kTriangleObjDataFile);

    v1_ = glm::vec3(0.000000, 1.000000, 0.000000);
    v2_ = glm::vec3(-0.866025, -0.500000, -0.000000);
    v3_ = glm::vec3(0.866025, -0.500000, -0.000000);

    normal_ = glm::normalize(glm::cross(v1_ - v2_, v2_ - v3_));
}

racer::TriangleRendererComponent::~TriangleRendererComponent()
{
    delete renderer_mesh_;
}

racer::IntersectionData racer::TriangleRendererComponent::RayTrace(Ray ray)
{
    glm::mat4 transform_matrix = glm::translate(glm::mat4(1), holdingEntity->transform->position_);
    transform_matrix *= holdingEntity->transform->GetRotationMatrix();
    transform_matrix = glm::scale(transform_matrix, holdingEntity->transform->scale_);

    glm::vec3 normal = glm::vec3(transform_matrix * glm::vec4(normal_, 0.0f));
    glm::vec3 v1 = glm::vec3(transform_matrix * glm::vec4(v1_, 1.0f));
    glm::vec3 v2 = glm::vec3(transform_matrix * glm::vec4(v2_, 1.0f));
    glm::vec3 v3 = glm::vec3(transform_matrix * glm::vec4(v3_, 1.0f));

    float dominator = glm::dot(normal, ray.direction);

    if (dominator == 0)
        return IntersectionData();

    float t = -(glm::dot(normal, ray.origin) - glm::dot(normal, v1)) / dominator;

    if (t < 0)
        return IntersectionData();

    glm::vec3 p_hit = ray.origin + t * ray.direction;

    glm::vec3 edge0 = v2 - v1;
    glm::vec3 edge1 = v3 - v2;
    glm::vec3 edge2 = v1 - v3;
    glm::vec3 C0 = p_hit - v1;
    glm::vec3 C1 = p_hit - v2;
    glm::vec3 C2 = p_hit - v3;

    if (glm::dot(normal, glm::cross(edge0, C0)) > 0 &&
        glm::dot(normal, glm::cross(edge1, C1)) > 0 &&
        glm::dot(normal, glm::cross(edge2, C2)) > 0)
    {
        return IntersectionData(true, p_hit, normal, t);
    }
    else
    {
        return IntersectionData();
    }
}

void racer::TriangleRendererComponent::Rastarize(glm::mat4 VP, const Shader &rendering_shader)
{
    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::translate(model, holdingEntity->transform->position_);
    model *= glm::yawPitchRoll(holdingEntity->transform->rotation_.y, holdingEntity->transform->rotation_.x, holdingEntity->transform->rotation_.z);
    model = glm::scale(model, holdingEntity->transform->scale_);

    rendering_shader.setVec3("color", rendering_material_.color);

    rendering_shader.setMat4("MVP", VP * model);

    renderer_mesh_->Render();
}