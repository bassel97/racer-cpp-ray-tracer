#include "model-renderer.h"

#include <entity.h>

void racer::ModelRendererComponent::Rastarize(glm::mat4 VP, const Shader &rendering_shader)
{
    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 modelMat = holdingEntity->transform->GetTransformationMatrix();

    rendering_shader.setVec3("color", rendering_material_.color);

    rendering_shader.setMat4("model", modelMat);
    rendering_shader.setMat4("VP", VP);

    model->Draw(rendering_shader);
}

racer::IntersectionData racer::ModelRendererComponent::RayTrace(racer::Ray ray)
{
    double tmin = 10000000;
    bool intersected = false;
    IntersectionData nearIntersectedTriangle;
    TrianglePrimitiveShape *nearShape;

    for (size_t shapeIndex = 0; shapeIndex < model_triangles_.size(); shapeIndex++)
    {
        racer::IntersectionData intersectionData = model_triangles_[shapeIndex].RayTrace(ray, holdingEntity->transform->GetTransformationMatrix());

        if (intersectionData.intersected)
        {
            if (intersectionData.t < tmin && intersectionData.t > 0)
            {
                intersected = true;
                tmin = intersectionData.t;

                nearIntersectedTriangle = intersectionData;

                nearShape = &model_triangles_[shapeIndex];
            }
        }
    }

    return nearIntersectedTriangle;
}