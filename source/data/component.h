#ifndef RACER_DATA_COMPONENT_H_
#define RACER_DATA_COMPONENT_H_

#include <iostream>
#include <string>

namespace racer
{
    class Entity;
    class Component
    {
    public:
        Component()
        {
        }
        virtual std::string GetName() = 0;

        Entity *holdingEntity;

    protected:
        std::string name_;
    };
} // namespace racer

#endif // RACER_DATA_COMPONENT_H_