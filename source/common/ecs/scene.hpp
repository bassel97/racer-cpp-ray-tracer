#pragma once

#include <iostream>
#include <set>
#include <vector>

#include <json.hpp>

#include <ecs/entity.hpp>

namespace racer
{
    class Scene{

        public:
        std::string name;
        std::set<Entity*> entities;

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
            for (auto& sceneData : scenes.items()) {
                Scene* scene = new Scene();
                scene->name = sceneData.key();
                //std::cout << scene.key() << " : " << scene.value() << "\n";
                std::cout << sceneData.key() << ":\n";
                std::vector<Entity*> entitiesVector = Entity::PopulateEntities(sceneData.value()["entities"]);

                for (auto& entity : entitiesVector) {
                    scene->AddEntity (entity);
                }
                scenesVector.push_back(scene);
            }
            return scenesVector;
        }
    };
} // namespace racer
