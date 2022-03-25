#pragma once

# define M_PI           3.14159265358979323846  /* pi */

#include <glm/glm.hpp>

#include <ecs/scene.hpp>

namespace racer
{
    class OfflineRenderSystem
    {
        Camera camera;
        std::vector<racer::Sphere> sphereDataVector;
        std::vector<Light> lightVector;
        glm::vec3 back;
        glm::vec3 ambient;

        glm::vec3 getPixelColor(glm::vec3 origin, glm::vec3 direction, int recursionLevel)
        {
            if (recursionLevel >= 3)
                return {0, 0, 0};

            glm::vec3 pixelColor = {0, 0, 0};
            if (recursionLevel == 0)
            {
                pixelColor = back;
            }

            double tmin = 10000000;
            bool intersected = false;
            racer::IntersectionData nearIntersectedSphereData;
            racer::Sphere nearSphere;

            for (size_t sphere = 0; sphere < sphereDataVector.size(); sphere++)
            {
                racer::IntersectionData intersectionData = sphereDataVector[sphere].DidIntersectWithRay(racer::Ray(origin, direction));

                if (intersectionData.intersected)
                {
                    if (intersectionData.t < tmin && intersectionData.t > 0)
                    {
                        intersected = true;
                        tmin = intersectionData.t;

                        nearIntersectedSphereData = intersectionData;

                        nearSphere = sphereDataVector[sphere];
                    }
                }
            }

            if (intersected)
            {

                pixelColor = nearSphere.renderingMaterial.Ka * ambient * nearSphere.renderingMaterial.color;

                for (size_t light_Source = 0; light_Source < lightVector.size(); light_Source++)
                {
                    glm::vec3 L = glm::normalize(lightVector[light_Source].holdingEntity->transform->position - nearIntersectedSphereData.pointOfIntersection);

                    bool shadowed = false;
                    for (size_t otherSphere = 0; otherSphere < sphereDataVector.size(); otherSphere++)
                    {
                        if (!sphereDataVector[otherSphere].holdingEntity->GetName()._Equal(nearSphere.holdingEntity->GetName()))
                        {

                            racer::IntersectionData shadowIntersection = sphereDataVector[otherSphere].DidIntersectWithRay(racer::Ray(nearIntersectedSphereData.pointOfIntersection, L));

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

                    pixelColor += nearSphere.renderingMaterial.Kd * lightVector[light_Source].color * NdolL * nearSphere.renderingMaterial.color;

                    glm::vec3 view = glm::normalize(origin - nearIntersectedSphereData.pointOfIntersection);

                    glm::vec3 r = (2 * NdolL * nearIntersectedSphereData.normalToIntersection - L);

                    double RdotV = std::max(0.0f, (r.x * view.x + r.y * view.y + r.z * view.z));

                    pixelColor += lightVector[light_Source].color * nearSphere.renderingMaterial.Ks * (float)std::pow(RdotV, nearSphere.renderingMaterial.n);
                }

                glm::vec3 view = glm::normalize(origin - nearIntersectedSphereData.pointOfIntersection);
                glm::vec3 reflectedRay = -view - 2.0f * nearIntersectedSphereData.normalToIntersection * glm::dot(-view, nearIntersectedSphereData.normalToIntersection);
                glm::vec3 reflectedCalculatedRayColor = getPixelColor(nearIntersectedSphereData.pointOfIntersection, reflectedRay, recursionLevel + 1);

                pixelColor += nearSphere.renderingMaterial.Kr * reflectedCalculatedRayColor;
            }

            pixelColor.r = std::min(1.0f, std::max(0.0f, pixelColor.r));
            pixelColor.g = std::min(1.0f, std::max(0.0f, pixelColor.g));
            pixelColor.b = std::min(1.0f, std::max(0.0f, pixelColor.b));
            return pixelColor;
        }

    public:
        void RenderScene(const Scene &scene, unsigned char *pixels)
        {
            std::set<Entity *> entities = scene.entities;
            for (std::set<Entity *>::iterator itr = entities.begin(); itr != entities.end(); itr++)
            {
                Camera *cameraComponent = (*itr)->GetComponent<Camera>();
                if (cameraComponent != NULL)
                    camera = *cameraComponent;
            }
            for (std::set<Entity *>::iterator itr = entities.begin(); itr != entities.end(); itr++)
            {
                Sphere *sphereComponent = (*itr)->GetComponent<Sphere>();
                if (sphereComponent != NULL)
                    sphereDataVector.push_back(*sphereComponent);
            }
            for (std::set<Entity *>::iterator itr = entities.begin(); itr != entities.end(); itr++)
            {
                Light *lightComponent = (*itr)->GetComponent<Light>();
                if (lightComponent != NULL)
                    lightVector.push_back(*lightComponent);
            }

            back.r = 1.0f;
            back.g = 1.0f;
            back.b = 1.0f;

            ambient.r = 0.2f;
            ambient.g = 0.2f;
            ambient.b = 0.2f;

            // This loop just creates a gradient for illustration purposes only. You will not use it.
            for (int i = 0; i < scene.screen.height; i++)
            {
                for (int k = 0; k < scene.screen.width; k++)
                {
                    //std::cout << tan ( (camera.fov * 3.14159265f) / 360.0f ) << std::endl;
                    float u = (2.0f * (((float)(k) + 0.5f) / (scene.screen.width)) - 1.0f) * scene.screen.aspectRatio * tan ( camera.GetVFov(scene.screen.aspectRatio) / 2.0);
                    float v = (1.0f - 2.0f * (((float)(i) + 0.5f) / (scene.screen.height))) * tan ( camera.GetVFov(scene.screen.aspectRatio) / 2.0);
                    float d = -camera.nearPlane;

                    glm::vec3 direction = {u, v, d};
                    glm::vec3 pixelColor = getPixelColor({0, 0, 0}, direction, 0);

                    pixels[i * scene.screen.width * 3 + k * 3] = (char)(pixelColor.r * 255);
                    pixels[i * scene.screen.width * 3 + k * 3 + 1] = (char)(pixelColor.g * 255);
                    pixels[i * scene.screen.width * 3 + k * 3 + 2] = (char)(pixelColor.b * 255);
                }
            }
        }
    };
}