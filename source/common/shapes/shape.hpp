#pragma once
#include <string>
#include <physics/ray.hpp>
#include <utilities/graphics/intersection-data.hpp>

#include <ecs/component.hpp>
#include <render/material.hpp>

#define SMALL_NUMBER    1.e-4f

namespace racer {

    // Act as a parent for every premitive shape in the project
    class Shape : public Component {

        public:
        //std::string name;

        // Transform data
        //glm::vec3 position;
        //glm::vec3 scale;
        //glm::vec3 rotation;

        // TODO(Bassel): move to a material class
        /*glm::vec3 color;
        float Ka;    float Kd;    float Ks;    float Kr;
        int n;*/

        Material renderingMaterial;

        virtual IntersectionData DidIntersectWithRay(Ray ray) = 0;

    };

}