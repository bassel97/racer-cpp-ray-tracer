#pragma once

#include <iostream>
#include <set>

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
    };
} // namespace racer
