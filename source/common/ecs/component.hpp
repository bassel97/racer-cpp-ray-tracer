#pragma once

#include <iostream>
#include <string>

namespace racer
{
    class Entity;
    class Component{
        
        protected:
        std::string name;
        
        public:
        Entity* holdingEntity;
        /*Component(std::string componentName = "New Component"):
        name(componentName)
        {

        }*/
    };
} // namespace racer
