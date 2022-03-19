#pragma once

#include <iostream>
#include <set>
#include <vector>

#include <json.hpp>

#include <ecs/entity.hpp>

namespace racer
{
    class Scene{

        std::set<Entity*> entities;

        public:
        void AddEntity (Entity* entity)
        {
            entities.insert(entity);
        }

        void RemoveEntity (Entity* entity)
        {
            entities.erase(entity);
        }

        int GetEntitiesCount() const
        {
            return entities.size();
        }

        static std::vector<Scene*> PopulateScenes (nlohmann::json scenes)
        {
            std::vector<Scene*> scenesVector;
            for (auto& scene : scenes.items()) {
                //std::cout << scene.key() << " : " << scene.value() << "\n";
                std::cout << scene.key() << ":\n";
                std::vector<Entity*> entitiesVector = Entity::PopulateEntities(scene.value()["entities"]);

            }
            return scenesVector;
        }
    };
} // namespace racer
