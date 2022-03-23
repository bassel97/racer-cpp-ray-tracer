#pragma once

#include <iostream>
#include <set>
#include <string>

#include <json.hpp>

#include <ecs/component.hpp>
#include <ecs/components/transform.hpp>
#include <render/components/camera.hpp>
#include <render/components/light.hpp>

#include <shapes/sphere/sphere-shape.hpp>

namespace racer
{
    class Entity{
        
        std::string name;
        std::set<Component*> components;

        public:

        Transform* transform;

        Entity(std::string entityName, nlohmann::json entityData):
        name(entityName)
        {
            auto componentsData = entityData["components"];
            for (auto& componentData : componentsData.items()) {                    
                if(componentData.key()._Equal("transform"))
                {
                    Transform* transformComponent = new Transform (componentData.value());
                    AddComponent(transformComponent);
                    transform = transformComponent;
                    continue;
                }
                if(componentData.key()._Equal("camera"))
                {
                    Camera* cameraComponent = new Camera (componentData.value());
                    AddComponent(cameraComponent);
                    continue;
                }
                if(componentData.key()._Equal("light"))
                {
                    Light* lightComponent = new Light (componentData.value());
                    AddComponent(lightComponent);
                    continue;
                }
                if(componentData.key()._Equal("sphere-renderer"))
                {
                    Sphere* sphereComponent = new Sphere (componentData.value());
                    AddComponent(sphereComponent);
                    continue;
                }
            }
        }

        void AddComponent(Component* component)
        {
            components.insert(component);
            component->holdingEntity = this;
        }

        void RemoveComponent(Component* component)
        {
            components.erase(component);
        }

        std::string GetName() const
        {
            return name;
        }

        template <class T>
        T* GetComponent()
        {
            for (Component* component : components) {
                T* childComponent = dynamic_cast<T*>(component);
                if ( childComponent != NULL )
                {
                    return childComponent;
                }
            }
            return NULL;
        }

        static std::vector<Entity*> PopulateEntities (nlohmann::json entities)
        {
            std::vector<Entity*> entitiesVector;
            for (auto& entityData : entities.items()) {
                Entity* entity = new Entity(entityData.key(), entityData.value());
                entitiesVector.push_back(entity);
            }
            return entitiesVector;
        }
    };
} // namespace racer
