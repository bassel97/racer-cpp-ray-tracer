#ifndef RACER_DATA_SCENE_H_
#define RACER_DATA_SCENE_H_

#include <iostream>
#include <set>
#include <vector>

#include <json.hpp>

#include <entity.h>

#include <utilities/window/screen.hpp>

#include <components/renderer/renderer-component.h>
#include <render/components/light.hpp>
#include <render/components/camera.hpp>

namespace racer
{
    class Scene
    {

    public:
        std::string name;
        std::set<Entity *> entities;

        Screen screen;

        std::vector<RendererComponent *> shapesToRender;
        std::vector<Light *> ligths;
        std::vector<Camera *> cameras;
        Camera *activeCamera;

        glm::vec3 environmentColor;

        // Empty Scene
        Scene(std::string name);
        Scene(std::string name, nlohmann::json sceneData);

        void AddEntity(Entity *entity);

        void RemoveEntity(Entity *entity);

        int GetEntitiesCount() const
        {
            return static_cast<int>(entities.size());
        }

        static std::vector<Scene *> PopulateScenes(nlohmann::json scenes);
    };
} // namespace racer

#endif  // RACER_DATA_SCENE_H_