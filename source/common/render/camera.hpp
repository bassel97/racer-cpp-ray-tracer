#pragma once

#include <ecs/component.hpp>

namespace racer
{

    class Camera : public Component {
        private:

        public:
        float nearPlane;
        float farPlane;    
        float left;
        float right;
        float bottom;
        float top;
    };

} // namespace racer
