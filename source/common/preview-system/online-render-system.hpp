#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>

#include <tiny_obj_loader.h>

#include <ecs/scene.hpp>
#include <preview-system/preview-structs.hpp>
#include <preview-system/primitive-shapes/cube-preview-data.hpp>
#include <preview-system/primitive-shapes/sphere-preview-data.hpp>
#include <preview-system/shader/shader.hpp>
#include <preview-system/shader/shader-files.hpp>
#include <preview-system/primitive-model.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace racer
{
    namespace previewSystem
    {
        class OnlineRenderSystem
        {
            GLFWwindow *window = nullptr;

            Scene *activeScene;

            PrimitiveModel *sphereModel;
            PrimitiveModel *cubeModel;

            void configureOpenGL()
            {
                // Request that OpenGL is 3.3
                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
                // Only enable core functionalities (disable features from older OpenGL versions that were removed in 3.3)
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
                // Enable forward compatibility with newer OpenGL versions by removing deprecated functionalities
                // This is necessary for some platforms
                glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

                // Make window size fixed (User can't resize it)
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

                // Set Number of sample used in MSAA (0 = Disabled)
                glfwWindowHint(GLFW_SAMPLES, 0);

                // Enable Double Buffering
                glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

                // Set the bit-depths of the frame buffer
                glfwWindowHint(GLFW_RED_BITS, 8);
                glfwWindowHint(GLFW_GREEN_BITS, 8);
                glfwWindowHint(GLFW_BLUE_BITS, 8);
                glfwWindowHint(GLFW_ALPHA_BITS, 8);

                // Set Bits for Depth Buffer
                glfwWindowHint(GLFW_DEPTH_BITS, 24);

                // Set Bits for Stencil Buffer
                glfwWindowHint(GLFW_STENCIL_BITS, 8);

                // Set the refresh rate of the window (GLFW_DONT_CARE = Run as fast as possible)
                glfwWindowHint(GLFW_REFRESH_RATE, GLFW_DONT_CARE);
            }

            void setWindowIcon()
            {
                GLFWimage images[1];
                images[0].pixels = stbi_load("racer-icon.png", &images[0].width, &images[0].height, 0, 4); // rgba channels
                glfwSetWindowIcon(window, 1, images);
                stbi_image_free(images[0].pixels);
            }

            void showContextInfo()
            {
                // Print information about the OpenGL context
                std::cout << "VENDOR          : " << glGetString(GL_VENDOR) << std::endl;
                std::cout << "RENDERER        : " << glGetString(GL_RENDERER) << std::endl;
                std::cout << "VERSION         : " << glGetString(GL_VERSION) << std::endl;
                std::cout << "GLSL VERSION    : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
            }

            void setPrimitiveModels()
            {
                sphereModel = new PrimitiveModel(sphereObjDataFile);
                cubeModel = new PrimitiveModel(cubeObjDataFile);
            }

        public:
            OnlineRenderSystem(Scene *scene)
            {
                activeScene = scene;

                // Initialize GLFW and exit if it failed
                if (!glfwInit())
                {
                    std::cerr << "Failed to Initialize GLFW" << std::endl;
                    return;
                }

                configureOpenGL();

                // The last parameter "share" can be used to share the resources (OpenGL objects) between multiple windows.
                window = glfwCreateWindow(activeScene->screen.width, activeScene->screen.height, "RACER", nullptr, nullptr);
                if (!window)
                {
                    std::cerr << "Failed to Create Window" << std::endl;
                    glfwTerminate();
                    return;
                }
                glfwMakeContextCurrent(window); // Tell GLFW to make the context of our window the main context on the current thread.

                gladLoadGL(glfwGetProcAddress); // Load the OpenGL functions from the driver

                setWindowIcon();

                setPrimitiveModels();

                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);

                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);

                Shader shader(MVP_VertexShader, diffuseFragmentShader);

                while (!glfwWindowShouldClose(window))
                {
                    glfwPollEvents();

                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                    glClearColor(activeScene->environmentColor.r, activeScene->environmentColor.g, activeScene->environmentColor.b,
                                 1.0f);

                    for (int i = 0; i < scene->shapesToRender.size(); i++)
                    {
                        shader.use();

                        glm::mat4 projection = glm::perspective((scene->activeCamera->GetVFov(scene->screen.aspectRatio)), scene->screen.aspectRatio, 0.1f, 100.0f);

                        // camera/view transformation
                        glm::mat4 view = glm::lookAt(scene->activeCamera->holdingEntity->transform->position,
                                                     scene->activeCamera->holdingEntity->transform->position + glm::vec3(0, 0, -1),
                                                     glm::vec3(0, 1, 0));

                        // calculate the model matrix for each object and pass it to shader before drawing
                        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
                        model = glm::translate(model, scene->shapesToRender[i]->holdingEntity->transform->position);
                        model = glm::scale(model, scene->shapesToRender[i]->holdingEntity->transform->scale);

                        shader.setVec3("viewDir", glm::vec3(0, 0, -1));

                        shader.setVec3("color", scene->shapesToRender[i]->renderingMaterial.color);

                        shader.setMat4("MVP", projection * view * model);

                        sphereModel->Render();
                    }

                    // Swap the frame buffers
                    glfwSwapBuffers(window);
                }

                // Destroy the window
                glfwDestroyWindow(window);
                // And finally terminate GLFW
                glfwTerminate();
            }
        };
    }
}