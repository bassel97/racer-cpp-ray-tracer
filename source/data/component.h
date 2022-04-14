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
        Entity *holdingEntity;

        virtual std::string GetName()
        {
            return name;
        }

    protected:
        std::string name;
    };
} // namespace racer

#endif // RACER_DATA_COMPONENT_H_