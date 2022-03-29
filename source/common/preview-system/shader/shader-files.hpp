#pragma once

#include <string>

namespace racer
{
    namespace previewSystem
    {
        const std::string MVP_VertexShader =
            "#version 330 core\n"
            "layout (location = 0) in vec3 aPos;\n"
            "layout (location = 1) in vec3 aNormal;\n"
            "layout (location = 2) in vec2 aTexCoord;\n"
            "out vec2 TexCoord;\n"
            "out vec3 Normal;\n"
            "uniform mat4 MVP;\n"
            "void main()\n"
            "{\n"
            "	gl_Position = MVP * vec4(aPos, 1.0f);\n"
            "	TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
            "   Normal = aNormal;\n"
            "}";

        const std::string diffuseFragmentShader =
            "#version 330 core\n"
            "out vec4 FragColor;\n"
            "in vec3 Normal;\n"
            "uniform vec3 viewDir;\n"
            "uniform vec3 color;\n"
            "void main()\n"
            "{\n"
            "   float diff = max( dot(-viewDir, Normal), 0.0);"
            "	FragColor = vec4(color, 1.0) * vec4(diff, diff, diff, 1.0);\n"
            "}";
    }
}