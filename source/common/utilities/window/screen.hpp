#pragma once

#include <json.hpp>

namespace racer
{
    class Screen{

        public:
            int width;
            int height;

            Screen (){}

            Screen (nlohmann::json screenData)
            {
                SetScreen (screenData);
            }

            void SetScreen (nlohmann::json screenData)
            {
                width = screenData["width"];
                height = screenData["height"];
            }
    };
} // namespace racer
