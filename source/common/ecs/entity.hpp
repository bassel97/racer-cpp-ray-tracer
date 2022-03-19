#pragma once

#include <iostream>
#include <set>
#include <string>

#include <json.hpp>

#include <ecs/component.hpp>
#include <ecs/components/transform.hpp>
#include <render/camera.hpp>

namespace racer
{
    class Entity{
        
        std::string name;
        std::set<Component*> components;

        public:

        Transform* transform;

        Entity(std::string entityName):
        name(entityName)
        {

        }

        void AddComponent(Component* component)
        {
            components.insert(component);
        }

        void RemoveComponent(Component* component)
        {
            components.erase(component);
        }

        std::string GetName() const
        {
            return name;
        }

        static std::vector<Entity*> PopulateEntities (nlohmann::json entities)
        {
            std::vector<Entity*> entitiesVector;
            for (auto& entityData : entities.items()) {

                Entity* entity = new Entity(entityData.key());

                //std::cout << entityData.key() << " : " << entityData.value() << "\n";
                std::cout << entityData.key() << " :\n";

                auto componentsData = entityData.value()["components"];

                for (auto& componentData : componentsData.items()) {                    
                    if(componentData.key()._Equal("transform"))
                    {
                        Transform* transformComponent = new Transform (componentData.value());
                        entity->AddComponent(transformComponent);
                        entity->transform = transformComponent;
                        continue;
                    }
                    if(componentData.key()._Equal("camera"))
                    {
                        Camera* transformComponent = new Camera (componentData.value());
                        entity->AddComponent(transformComponent);
                        continue;
                    }
                    if(componentData.key()._Equal("light"))
                    {
                        Camera* transformComponent = new Camera (componentData.value());
                        entity->AddComponent(transformComponent);
                        continue;
                    }

                    std::cout << componentData.key() << " : " << componentData.value() << std::endl;
                }


                entitiesVector.push_back(entity);
            }
            return entitiesVector;
        }
    };
} // namespace racer
