#pragma once

#include <ecs/component.hpp>

namespace racer
{

    class Camera : public Component {
        private:

        public:
        float nearPLane;
        float farPLane;    
        float left;
        float right;
        float bottom;
        float top;
    };

} // namespace racer
