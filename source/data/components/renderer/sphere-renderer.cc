#include "sphere-renderer.h"

#include <entity.h>

racer::SphereRendererComponent::SphereRendererComponent()
{
    renderer_mesh_ = new PrimitiveModel(kSphereObjDataFile);
}

racer::SphereRendererComponent::~SphereRendererComponent()
{
    delete renderer_mesh_;
}

racer::IntersectionData racer::SphereRendererComponent::RayTrace(Ray ray)
{
    ray.origin = (ray.origin - holdingEntity->transform->position_) / holdingEntity->transform->scale_;

    glm::vec3 scaledDirection = ray.direction / holdingEntity->transform->scale_;

    float A = glm::dot(scaledDirection, scaledDirection);

    float B = -2 * glm::dot(ray.origin, scaledDirection);

    float C = glm::dot(ray.origin, ray.origin) - 1;

    float D = B * B - 4 * A * C;

    if (D < 0)
    {
        return IntersectionData();
    }

    float tpos = (-B + sqrt(D)) / (2.0f * A);
    float tneg = (-B - sqrt(D)) / (2.0f * A);

    float t = -(SMALL_NUMBER + std::max(tpos, tneg));

    glm::vec3 point = ray.origin + t * scaledDirection;

    glm::vec3 normal = point / holdingEntity->transform->scale_;

    normal = glm::normalize(normal);

    glm::vec3 pointOfIntersection = (point * holdingEntity->transform->scale_) + holdingEntity->transform->position_;

    glm::vec3 normalToIntersection = normal;

    return IntersectionData(true, pointOfIntersection, normalToIntersection, t);
}

void racer::SphereRendererComponent::Rastarize(glm::mat4 VP, const Shader &rendering_shader)
{
    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::translate(model, holdingEntity->transform->position_);
    model = glm::scale(model, holdingEntity->transform->scale_);

    rendering_shader.setVec3("color", rendering_material_.color);

    rendering_shader.setMat4("MVP", VP * model);

    renderer_mesh_->Render();
}