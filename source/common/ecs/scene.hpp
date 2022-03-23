#pragma once

#include <iostream>
#include <set>
#include <vector>

#include <json.hpp>

#include <ecs/entity.hpp>
#include <utilities/window/screen.hpp>

namespace racer
{
    class Scene
    {

    public:
        std::string name;
        std::set<Entity *> entities;

        Screen screen;

        Scene(std::string name, nlohmann::json sceneData) : name(name)
        {
            std::vector<Entity *> entitiesVector = Entity::PopulateEntities(sceneData["entities"]);
            for (auto &entity : entitiesVector)
            {
                AddEntity(entity);
            }

            screen.SetScreen(sceneData["scene-properties"]["screen"]);
        }

        void AddEntity(Entity *entity)
        {
            entities.insert(entity);
        }

        void RemoveEntity(Entity *entity)
        {
            entities.erase(entity);
        }

        int GetEntitiesCount() const
        {
            return entities.size();
        }

        static std::vector<Scene *> PopulateScenes(nlohmann::json scenes)
        {
            std::vector<Scene *> scenesVector;
            for (auto &sceneData : scenes.items())
            {
                Scene *scene = new Scene(sceneData.key(), sceneData.value());
                scenesVector.push_back(scene);
            }
            return scenesVector;
        }
    };
} // namespace racer
