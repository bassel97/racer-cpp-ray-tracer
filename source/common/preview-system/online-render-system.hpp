#pragma once
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <tiny_obj_loader.h>

#include <scene.h>
#include <preview-system/preview-structs.hpp>
#include <preview-system/primitive-shapes/cube-preview-data.hpp>
#include <preview-system/primitive-shapes/sphere-preview-data.hpp>
#include <preview-system/shader/shader.hpp>
#include <preview-system/shader/shader-files.hpp>
#include <preview-system/primitive-model.hpp>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl/imgui_impl_glfw.h>
#include <imgui_impl/imgui_impl_opengl3.h>

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
                glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

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

            /*void setWindowIcon()
            {
                GLFWimage images[1];
                images[0].pixels = stbi_load("racer-icon.png", &images[0].width, &images[0].height, 0, 4); // rgba channels
                glfwSetWindowIcon(window, 1, images);
                stbi_image_free(images[0].pixels);
            }*/

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

                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                {
                    std::cout << "Failed to initialize GLAD" << std::endl;
                    return;
                }

                // Setup Dear ImGui context
                IMGUI_CHECKVERSION();
                ImGui::CreateContext();
                ImGuiIO &io = ImGui::GetIO();
                (void)io;
                io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
                io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
                io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

                // Setup Dear ImGui style
                ImGui::StyleColorsDark();
                // ImGui::StyleColorsClassic();

                // Setup Platform/Renderer backends
                ImGui_ImplGlfw_InitForOpenGL(window, true);
                ImGui_ImplOpenGL3_Init("#version 330");

                //setWindowIcon();

                setPrimitiveModels();

                glEnable(GL_CULL_FACE);
                glCullFace(GL_BACK);

                glEnable(GL_DEPTH_TEST);
                glDepthFunc(GL_LESS);

                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

                Shader shader(MVP_VertexShader, diffuseFragmentShader);

                // Our state
                bool show_demo_window = true;
                bool show_another_window = false;
                ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

                while (!glfwWindowShouldClose(window))
                {
                    glfwPollEvents();

                    // Start the Dear ImGui frame
                    ImGui_ImplOpenGL3_NewFrame();
                    ImGui_ImplGlfw_NewFrame();
                    ImGui::NewFrame();

                    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
                    /*if (show_demo_window)
                        ImGui::ShowDemoWindow(&show_demo_window);

                    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
                    {
                        static float f = 0.0f;
                        static int counter = 0;

                        ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

                        ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
                        ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
                        ImGui::Checkbox("Another Window", &show_another_window);

                        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
                        ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

                        if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
                            counter++;
                        ImGui::SameLine();
                        ImGui::Text("counter = %d", counter);

                        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                        ImGui::End();
                    }

                    // 3. Show another simple window.
                    if (show_another_window)
                    {
                        ImGui::Begin("Another Window", &show_another_window); // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                        ImGui::Text("Hello from another window!");
                        if (ImGui::Button("Close Me"))
                            show_another_window = false;
                        ImGui::End();
                    }*/

                    static bool opt_fullscreen = true;
                    static bool opt_padding = false;
                    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

                    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
                    // because it would be confusing to have two docking targets within each others.
                    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
                    if (opt_fullscreen)
                    {
                        const ImGuiViewport *viewport = ImGui::GetMainViewport();
                        ImGui::SetNextWindowPos(viewport->WorkPos);
                        ImGui::SetNextWindowSize(viewport->WorkSize);
                        ImGui::SetNextWindowViewport(viewport->ID);
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
                    }
                    else
                    {
                        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
                    }

                    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
                    // and handle the pass-thru hole, so we ask Begin() to not render a background.
                    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                        window_flags |= ImGuiWindowFlags_NoBackground;

                    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
                    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
                    // all active windows docked into it will lose their parent and become undocked.
                    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
                    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
                    if (!opt_padding)
                        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
                    ImGui::Begin("DockSpace Demo", NULL, window_flags);
                    if (!opt_padding)
                        ImGui::PopStyleVar();

                    if (opt_fullscreen)
                        ImGui::PopStyleVar(2);

                    // Submit the DockSpace
                    ImGuiIO &io = ImGui::GetIO();
                    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
                    {
                        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
                    }
                    else
                    {
                        // ShowDockingDisabledMessage();
                    }

                    if (ImGui::BeginMenuBar())
                    {
                        if (ImGui::BeginMenu("Options"))
                        {
                            // Disabling fullscreen would allow the window to be moved to the front of other windows,
                            // which we can't undo at the moment without finer window depth/z control.
                            ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
                            ImGui::MenuItem("Padding", NULL, &opt_padding);
                            ImGui::Separator();

                            if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))
                            {
                                dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
                            }
                            if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))
                            {
                                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
                            }
                            if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))
                            {
                                dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
                            }
                            if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))
                            {
                                dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
                            }
                            if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen))
                            {
                                dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
                            }
                            ImGui::Separator();

                            /*if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
                             *p_open = false;*/
                            ImGui::EndMenu();
                        }
                        /*HelpMarker(
                            "When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!"
                            "\n"
                            "- Drag from window title bar or their tab to dock/undock."
                            "\n"
                            "- Drag from window menu button (upper-left button) to undock an entire node (all windows)."
                            "\n"
                            "- Hold SHIFT to disable docking (if io.ConfigDockingWithShift == false, default)"
                            "\n"
                            "- Hold SHIFT to enable docking (if io.ConfigDockingWithShift == true)"
                            "\n"
                            "This demo app has nothing to do with enabling docking!"
                            "\n\n"
                            "This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window."
                            "\n\n"
                            "Read comments in ShowExampleAppDockSpace() for more details.");*/

                        ImGui::EndMenuBar();
                    }

                    ImGui::ShowDemoWindow(&show_demo_window);
                    ImGui::End();

                    // Rendering
                    ImGui::Render();

                    glClearColor(
                        activeScene->environmentColor.r, activeScene->environmentColor.g, activeScene->environmentColor.b,
                        1.0f);
                    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

                    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

                    // Update and Render additional Platform Windows
                    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
                    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
                    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
                    {
                        GLFWwindow *backup_current_context = glfwGetCurrentContext();
                        ImGui::UpdatePlatformWindows();
                        ImGui::RenderPlatformWindowsDefault();
                        glfwMakeContextCurrent(backup_current_context);
                    }

                    // Swap the frame buffers
                    glfwSwapBuffers(window);
                }

                // Cleanup
                ImGui_ImplOpenGL3_Shutdown();
                ImGui_ImplGlfw_Shutdown();
                ImGui::DestroyContext();

                // Destroy the window
                glfwDestroyWindow(window);
                // And finally terminate GLFW
                glfwTerminate();
            }
        };
    }
}