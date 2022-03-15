#pragma once

#include <iostream>
#include <set>
#include <string>

#include <ecs/component.hpp>

namespace racer
{
    class Entity{
        
        std::string name;
        std::set<Component*> components;

        public:

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
    };
} // namespace racer
