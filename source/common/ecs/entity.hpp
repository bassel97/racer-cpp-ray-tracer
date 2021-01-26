#pragma once

#include <iostream>
#include <vector>

#include <ecs/component.hpp>

namespace racer
{
    class Entity{
        
        public:
        std::vector<Component*> components;

    };
} // namespace racer
