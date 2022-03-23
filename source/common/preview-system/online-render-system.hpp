#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>

namespace racer
{
    class OnlineRenderSystem
    {
        GLFWwindow* window = nullptr;

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

    public:
        OnlineRenderSystem()
        {
            // Initialize GLFW and exit if it failed
            if (!glfwInit())
            {
                std::cerr << "Failed to Initialize GLFW" << std::endl;
                // return -1;
            }

            configureOpenGL();

            // The last parameter "share" can be used to share the resources (OpenGL objects) between multiple windows.
            window = glfwCreateWindow(512, 512, "RACER", nullptr, nullptr);
            if (!window)
            {
                std::cerr << "Failed to Create Window" << std::endl;
                glfwTerminate();
                // return -1;
            }
            glfwMakeContextCurrent(window); // Tell GLFW to make the context of our window the main context on the current thread.

            gladLoadGL(glfwGetProcAddress); // Load the OpenGL functions from the driver

            // Print information about the OpenGL context
            std::cout << "VENDOR          : " << glGetString(GL_VENDOR) << std::endl;
            std::cout << "RENDERER        : " << glGetString(GL_RENDERER) << std::endl;
            std::cout << "VERSION         : " << glGetString(GL_VERSION) << std::endl;
            std::cout << "GLSL VERSION    : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

            while (!glfwWindowShouldClose(window))
            {
                glfwPollEvents();

                glClear(GL_COLOR_BUFFER_BIT);

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