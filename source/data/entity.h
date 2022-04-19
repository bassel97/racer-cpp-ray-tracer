#ifndef RACER_DATA_ENTITY_H_
#define RACER_DATA_ENTITY_H_

#include <iostream>
#include <set>
#include <string>

#include <component.h>
#include <components/transform.h>
#include <components/renderer/sphere-renderer.h>
#include <components/renderer/triangle-renderer.h>
#include <components/camera.h>
#include <components/light.h>

namespace racer
{
    class Entity
    {
    public:
        Entity(std::string = "");
        Entity(const Entity &) = delete;
        Entity &operator=(const Entity &) = delete;
        ~Entity();

        Transform *transform;

        void AddComponent(Component *);
        void RemoveComponent(Component *);

        std::string GetName() const;

        bool IsActive() const
        {
            return is_active_;
        }

        void SetActive(bool is_active)
        {
            is_active_ = is_active;
        }

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

        std::set<Component *> components_;

    private:
        std::string name_;
        bool is_active_;
    };
} // namespace racer

#endif // RACER_DATA_ENTITY_H_