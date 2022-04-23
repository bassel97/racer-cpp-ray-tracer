#include "sphere-renderer.h"

#include <entity.h>

racer::SphereRendererComponent::SphereRendererComponent()
{
    renderer_mesh_ = new PrimitiveModel(kSphereObjDataFile);
    border_shape_ = new AABBPrimitiveShape(glm::vec3(-1, -1, -1), glm::vec3(1, 1, 1));
}

racer::SphereRendererComponent::~SphereRendererComponent()
{
    delete border_shape_;
    delete renderer_mesh_;
}

racer::IntersectionData racer::SphereRendererComponent::RayTrace(Ray ray)
{
    if (!border_shape_->RayHit(ray))
    {
        return IntersectionData();
    }

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
    glm::mat4 model = holdingEntity->transform->GetTransformationMatrix();

    rendering_shader.setVec3("color", rendering_material_.color);

    rendering_shader.setMat4("model", model);
    rendering_shader.setMat4("VP", VP);

    renderer_mesh_->Render();
}