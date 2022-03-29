#pragma once

#define M_PI 3.14159265358979323846 /* pi */

#include <glm/glm.hpp>

#include <ecs/scene.hpp>

namespace racer
{
    class OfflineRenderSystem
    {
        Scene *scene;

        glm::vec3 getPixelColor(glm::vec3 origin, glm::vec3 direction, int recursionLevel)
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
            Shape *nearShape;

            for (size_t shapeIndex = 0; shapeIndex < scene->shapesToRender.size(); shapeIndex++)
            {
                racer::IntersectionData intersectionData = scene->shapesToRender[shapeIndex]->DidIntersectWithRay(racer::Ray(origin, direction));

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

                pixelColor = nearShape->renderingMaterial.Ka * scene->environmentColor * nearShape->renderingMaterial.color;

                for (size_t lightIndex = 0; lightIndex < scene->ligths.size(); lightIndex++)
                {
                    glm::vec3 L = glm::normalize(scene->ligths[lightIndex]->holdingEntity->transform->position - nearIntersectedSphereData.pointOfIntersection);

                    bool shadowed = false;
                    for (size_t otherSphereIndex = 0; otherSphereIndex < scene->shapesToRender.size(); otherSphereIndex++)
                    {
                        if (!scene->shapesToRender[otherSphereIndex]->holdingEntity->GetName()._Equal(nearShape->holdingEntity->GetName()))
                        {

                            racer::IntersectionData shadowIntersection = scene->shapesToRender[otherSphereIndex]->DidIntersectWithRay(racer::Ray(nearIntersectedSphereData.pointOfIntersection, L));

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

                    pixelColor += nearShape->renderingMaterial.Kd * scene->ligths[lightIndex]->color * NdolL * nearShape->renderingMaterial.color;

                    glm::vec3 view = glm::normalize(origin - nearIntersectedSphereData.pointOfIntersection);

                    glm::vec3 r = (2 * NdolL * nearIntersectedSphereData.normalToIntersection - L);

                    double RdotV = std::max(0.0f, (r.x * view.x + r.y * view.y + r.z * view.z));

                    pixelColor += scene->ligths[lightIndex]->color * nearShape->renderingMaterial.Ks * (float)std::pow(RdotV, nearShape->renderingMaterial.n);
                }

                glm::vec3 view = glm::normalize(origin - nearIntersectedSphereData.pointOfIntersection);
                glm::vec3 reflectedRay = -view - 2.0f * nearIntersectedSphereData.normalToIntersection * glm::dot(-view, nearIntersectedSphereData.normalToIntersection);
                glm::vec3 reflectedCalculatedRayColor = getPixelColor(nearIntersectedSphereData.pointOfIntersection, reflectedRay, recursionLevel + 1);

                pixelColor += nearShape->renderingMaterial.Kr * reflectedCalculatedRayColor;
            }

            pixelColor.r = std::min(1.0f, std::max(0.0f, pixelColor.r));
            pixelColor.g = std::min(1.0f, std::max(0.0f, pixelColor.g));
            pixelColor.b = std::min(1.0f, std::max(0.0f, pixelColor.b));
            return pixelColor;
        }

    public:
        void RenderScene(Scene *sceneToRender, unsigned char *pixels)
        {
            this->scene = sceneToRender;

            float fov_correction = static_cast<float>(tan(scene->activeCamera->GetVFov(scene->screen.aspectRatio) / 2.0));

            for (int i = 0; i < scene->screen.height; i++)
            {
                for (int k = 0; k < scene->screen.width; k++)
                {
                    float u = (2.0f * ((static_cast<float>(k) + 0.5f) / (scene->screen.width)) - 1.0f) * scene->screen.aspectRatio * fov_correction;
                    float v = (1.0f - 2.0f * ((static_cast<float>(i) + 0.5f) / (scene->screen.height))) * fov_correction;
                    float d = -scene->activeCamera->nearPlane;

                    glm::vec3 direction = {u, v, d};
                    glm::vec3 cameraOrigin = scene->activeCamera->holdingEntity->transform->position;
                    glm::vec3 pixelColor = getPixelColor(cameraOrigin, direction, 0);

                    pixels[i * scene->screen.width * 3 + k * 3] = (char)(pixelColor.r * 255);
                    pixels[i * scene->screen.width * 3 + k * 3 + 1] = (char)(pixelColor.g * 255);
                    pixels[i * scene->screen.width * 3 + k * 3 + 2] = (char)(pixelColor.b * 255);
                }
            }
        }
    };
}