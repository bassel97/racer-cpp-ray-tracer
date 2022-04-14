#ifndef RACER_DATA_ENTITY_H_
#define RACER_DATA_ENTITY_H_

#include <iostream>
#include <set>
#include <string>

#include <json.hpp>

#include <component.h>
#include <components/transform.h>
#include <components/renderer/sphere-renderer.h>
#include <render/components/camera.hpp>
#include <render/components/light.hpp>

namespace racer
{
    class Entity
    {
    public:
        Entity(std::string, nlohmann::json);

        Transform *transform;

        void AddComponent(Component *);
        void RemoveComponent(Component *);

        std::string GetName() const;

        static std::vector<Entity *> PopulateEntities(nlohmann::json entities);

        template <class T>
        T *GetComponent()
        {
            for (Component *component : components)
            {
                T *childComponent = dynamic_cast<T *>(component);
                if (childComponent != NULL)
                {
                    return childComponent;
                }
            }
            return NULL;
        }

    private:
        std::string name;
        std::set<Component *> components;
    };
} // namespace racer

#endif // RACER_DATA_ENTITY_H_