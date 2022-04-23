#include "triangle-renderer.h"

#include <iostream>
#include <entity.h>

racer::TriangleRendererComponent::TriangleRendererComponent()
    : trianglePrimitiveShape(
          glm::vec3(0.000000, 1.000000, 0.000000),
          glm::vec3(-0.866025, -0.500000, -0.000000),
          glm::vec3(0.866025, -0.500000, -0.000000))
{
    renderer_mesh_ = new PrimitiveModel(kTriangleObjDataFile);
}

racer::TriangleRendererComponent::~TriangleRendererComponent()
{
    delete renderer_mesh_;
}

racer::IntersectionData racer::TriangleRendererComponent::RayTrace(Ray ray)
{
    return trianglePrimitiveShape.RayTrace(ray, holdingEntity->transform->GetTransformationMatrix());
}

void racer::TriangleRendererComponent::Rastarize(glm::mat4 VP, const Shader &rendering_shader)
{
    // calculate the model matrix for each object and pass it to shader before drawing
    // TODO (Bassel): move to transform entity
    glm::mat4 model = holdingEntity->transform->GetTransformationMatrix();

    rendering_shader.setVec3("color", rendering_material_.color);

    rendering_shader.setMat4("model", model);
    rendering_shader.setMat4("VP", VP);

    renderer_mesh_->Render();
}