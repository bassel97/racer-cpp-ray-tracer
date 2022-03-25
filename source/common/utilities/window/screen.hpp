#pragma once

#include <json.hpp>

namespace racer
{
    class Screen{

        public:
            int width;
            int height;

            float aspectRatio;

            Screen (){}

            Screen (nlohmann::json screenData)
            {
                SetScreen (screenData);
            }

            void SetScreen (nlohmann::json screenData)
            {
                width = screenData["width"];
                height = screenData["height"];

                aspectRatio = (float)width / height;
            }
    };
} // namespace racer
