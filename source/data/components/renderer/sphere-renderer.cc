#include "sphere-renderer.h"

#include <entity.h>

racer::SphereRendererComponent::SphereRendererComponent()
{
    renderer_mesh_ = new PrimitiveModel(kSphereObjDataFile);
}

racer::SphereRendererComponent::SphereRendererComponent(nlohmann::json shapeData)
{
    renderer_mesh_ = new PrimitiveModel(kSphereObjDataFile);

    // TO-DO duplicate code to helper functions
    auto materialData = shapeData["material"];
    rendering_material_.color.r = materialData["color"]["r"];
    rendering_material_.color.g = materialData["color"]["g"];
    rendering_material_.color.b = materialData["color"]["b"];

    rendering_material_.Ka = materialData["k"]["ka"];
    rendering_material_.Kd = materialData["k"]["kd"];
    rendering_material_.Kr = materialData["k"]["kr"];
    rendering_material_.Ks = materialData["k"]["ks"];

    rendering_material_.n = materialData["n"];
}

racer::SphereRendererComponent::~SphereRendererComponent()
{
    delete renderer_mesh_;
}

racer::IntersectionData racer::SphereRendererComponent::RayTrace(Ray ray)
{
    ray.origin = (ray.origin - holdingEntity->transform->position) / holdingEntity->transform->scale;

    glm::vec3 scaledDirection = ray.direction / holdingEntity->transform->scale;

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

    glm::vec3 normal = point / holdingEntity->transform->scale;

    normal = glm::normalize(normal);

    glm::vec3 pointOfIntersection = (point * holdingEntity->transform->scale) + holdingEntity->transform->position;

    glm::vec3 normalToIntersection = normal;

    return IntersectionData(true, pointOfIntersection, normalToIntersection, t);
}

void racer::SphereRendererComponent::Rastarize(glm::mat4 VP, const Shader &rendering_shader)
{
    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    model = glm::translate(model, holdingEntity->transform->position);
    model = glm::scale(model, holdingEntity->transform->scale);

    rendering_shader.setVec3("color", rendering_material_.color);

    rendering_shader.setMat4("MVP", VP * model);

    renderer_mesh_->Render();
}