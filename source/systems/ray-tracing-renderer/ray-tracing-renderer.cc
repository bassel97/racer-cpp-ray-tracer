#include "ray-tracing-renderer.h"

void racer::RayTracingRendererSystem::RenderScene(Scene *sceneToRender, unsigned char *pixels, int width, int height)
{
    this->scene = sceneToRender;

    float aspectRatio = (float)width / height;
    float fov_correction = static_cast<float>(tan(scene->activeCamera->GetVFov(aspectRatio) / 2.0));

    for (int i = 0; i < height; i++)
    {
        for (int k = 0; k < width; k++)
        {
            float u = (2.0f * ((static_cast<float>(k) + 0.5f) / width) - 1.0f) * aspectRatio * fov_correction;
            float v = (1.0f - 2.0f * ((static_cast<float>(i) + 0.5f) / height)) * fov_correction;
            float d = -scene->activeCamera->nearPlane;

            glm::vec3 direction = {u, v, d};
            glm::vec3 cameraOrigin = scene->activeCamera->holdingEntity->transform->position;
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
        pixelColor = scene->environmentColor;
    }

    double tmin = 10000000;
    bool intersected = false;
    IntersectionData nearIntersectedSphereData;
    RendererComponent *nearShape;

    for (size_t shapeIndex = 0; shapeIndex < scene->shapesToRender.size(); shapeIndex++)
    {
        racer::IntersectionData intersectionData = scene->shapesToRender[shapeIndex]->RayTrace(racer::Ray(origin, direction));

        if (intersectionData.intersected)
        {
            if (intersectionData.t < tmin && intersectionData.t > 0)
            {
                intersected = true;
                tmin = intersectionData.t;

                nearIntersectedSphereData = intersectionData;

                nearShape = scene->shapesToRender[shapeIndex];
            }
        }
    }

    if (intersected)
    {
        pixelColor = nearShape->rendering_material_.Ka * scene->environmentColor * nearShape->rendering_material_.color;

        for (size_t lightIndex = 0; lightIndex < scene->ligths.size(); lightIndex++)
        {
            glm::vec3 L = glm::normalize(scene->ligths[lightIndex]->holdingEntity->transform->position - nearIntersectedSphereData.pointOfIntersection);

            bool shadowed = false;
            for (size_t otherSphereIndex = 0; otherSphereIndex < scene->shapesToRender.size(); otherSphereIndex++)
            {
                if (!scene->shapesToRender[otherSphereIndex]->holdingEntity->GetName()._Equal(nearShape->holdingEntity->GetName()))
                {

                    racer::IntersectionData shadowIntersection = scene->shapesToRender[otherSphereIndex]->RayTrace(racer::Ray(nearIntersectedSphereData.pointOfIntersection, L));

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

            pixelColor += nearShape->rendering_material_.Kd * scene->ligths[lightIndex]->color * NdolL * nearShape->rendering_material_.color;

            glm::vec3 view = glm::normalize(origin - nearIntersectedSphereData.pointOfIntersection);

            glm::vec3 r = (2 * NdolL * nearIntersectedSphereData.normalToIntersection - L);

            double RdotV = std::max(0.0f, (r.x * view.x + r.y * view.y + r.z * view.z));

            pixelColor += scene->ligths[lightIndex]->color * nearShape->rendering_material_.Ks * (float)std::pow(RdotV, nearShape->rendering_material_.n);
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