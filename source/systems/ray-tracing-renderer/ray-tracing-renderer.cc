#include "ray-tracing-renderer.h"

#include <iostream>

void racer::RayTracingRendererSystem::RenderScene(Scene *sceneToRender, unsigned char *pixels, int width, int height)
{
    this->scene = sceneToRender;

    float aspectRatio = (float)width / height;
    float fov_correction = static_cast<float>(tan(scene->active_camera_->GetTanFOV() / 2.0f));

    glm::mat4 camera_rotation_matrix = scene->active_camera_->holdingEntity->transform->GetRotationMatrix();

    for (size_t shapeIndex = 0; shapeIndex < scene->shapes_to_render_.size(); shapeIndex++)
    {
        scene->shapes_to_render_[shapeIndex]->UpdateBoundingBox();
    }

    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            std::cout << "progress: " << (((float)(i * height) + k) / (width * height)) * 100 << "%" << std::endl;

            float u = (2.0f * ((static_cast<float>(k) + 0.5f) / width) - 1.0f) * fov_correction;
            float v = (1.0f - 2.0f * ((static_cast<float>(i) + 0.5f) / height)) * (fov_correction / aspectRatio);
            float d = -1;

            glm::vec3 direction = {u, v, d};

            glm::vec4 direction_rotated = camera_rotation_matrix * glm::vec4(direction, 1.0f);
            direction = glm::vec3(direction_rotated);

            glm::vec3 cameraOrigin = scene->active_camera_->holdingEntity->transform->position_;
            glm::vec3 pixelColor = GetPixelColor(cameraOrigin, direction, 0);

            pixels[i * width * 3 + k * 3] = (char)(pixelColor.r * 255);
            pixels[i * width * 3 + k * 3 + 1] = (char)(pixelColor.g * 255);
            pixels[i * width * 3 + k * 3 + 2] = (char)(pixelColor.b * 255);
        }
    }
}

glm::vec3 racer::RayTracingRendererSystem::GetPixelColor(glm::vec3 origin, glm::vec3 direction, int recursionLevel)
{
    if (recursionLevel >= 3)
        return {0, 0, 0};

    glm::vec3 pixelColor = {0, 0, 0};
    if (recursionLevel == 0)
    {
        pixelColor = scene->environment_color_;
    }

    double tmin = 10000000;
    bool intersected = false;
    IntersectionData nearIntersectedSphereData;
    RendererComponent *nearShape;

    for (size_t shapeIndex = 0; shapeIndex < scene->shapes_to_render_.size(); shapeIndex++)
    {
        racer::IntersectionData intersectionData = scene->shapes_to_render_[shapeIndex]->RayTrace(racer::Ray(origin, direction));

        if (intersectionData.intersected)
        {
            if (intersectionData.t < tmin && intersectionData.t > 0)
            {
                intersected = true;
                tmin = intersectionData.t;

                nearIntersectedSphereData = intersectionData;

                nearShape = scene->shapes_to_render_[shapeIndex];
            }
        }
    }

    if (intersected)
    {
        pixelColor = nearShape->rendering_material_.Ka * scene->environment_color_ * nearShape->rendering_material_.color;

        for (size_t lightIndex = 0; lightIndex < scene->ligths_.size(); lightIndex++)
        {
            glm::vec3 L = glm::normalize(scene->ligths_[lightIndex]->holdingEntity->transform->position_ - nearIntersectedSphereData.pointOfIntersection);

            bool shadowed = false;
            for (size_t otherSphereIndex = 0; otherSphereIndex < scene->shapes_to_render_.size(); otherSphereIndex++)
            {
                if (!scene->shapes_to_render_[otherSphereIndex]->holdingEntity->GetName()._Equal(nearShape->holdingEntity->GetName()))
                {

                    racer::IntersectionData shadowIntersection = scene->shapes_to_render_[otherSphereIndex]->RayTrace(racer::Ray(nearIntersectedSphereData.pointOfIntersection, L));

                    if (shadowIntersection.intersected && shadowIntersection.t > 0.01f)
                    {
                        shadowed = true;
                        break;
                    }
                }
            }
            if (shadowed)
                continue;

            float NdolL = std::max(0.0f, (nearIntersectedSphereData.normalToIntersection.x * L.x + nearIntersectedSphereData.normalToIntersection.y * L.y + nearIntersectedSphereData.normalToIntersection.z * L.z));

            pixelColor += nearShape->rendering_material_.Kd * scene->ligths_[lightIndex]->color_ * NdolL * nearShape->rendering_material_.color;

            glm::vec3 view = glm::normalize(origin - nearIntersectedSphereData.pointOfIntersection);

            glm::vec3 r = (2 * NdolL * nearIntersectedSphereData.normalToIntersection - L);

            double RdotV = std::max(0.0f, (r.x * view.x + r.y * view.y + r.z * view.z));

            pixelColor += scene->ligths_[lightIndex]->color_ * nearShape->rendering_material_.Ks * (float)std::pow(RdotV, nearShape->rendering_material_.n);
        }

        glm::vec3 view = glm::normalize(origin - nearIntersectedSphereData.pointOfIntersection);
        glm::vec3 reflectedRay = -view - 2.0f * nearIntersectedSphereData.normalToIntersection * glm::dot(-view, nearIntersectedSphereData.normalToIntersection);
        glm::vec3 reflectedCalculatedRayColor = GetPixelColor(nearIntersectedSphereData.pointOfIntersection, reflectedRay, recursionLevel + 1);

        pixelColor += nearShape->rendering_material_.Kr * reflectedCalculatedRayColor;
    }

    pixelColor.r = std::min(1.0f, std::max(0.0f, pixelColor.r));
    pixelColor.g = std::min(1.0f, std::max(0.0f, pixelColor.g));
    pixelColor.b = std::min(1.0f, std::max(0.0f, pixelColor.b));

    return pixelColor;
}