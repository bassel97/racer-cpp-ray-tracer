#include <glm/glm.hpp>

namespace racer
{    
    class Material 
    {
        public:
        glm::vec3 color;
        float Ka, Kd, Ks, Kr;
        int n;
    };
}