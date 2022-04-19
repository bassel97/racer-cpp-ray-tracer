#include "application-ui.h"

#include <glm/gtc/type_ptr.hpp>

racer::ApplicationUI::ApplicationUI(GLFWwindow *glfw_window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io->Fonts->AddFontFromFileTTF("BAHNSCHRIFT 1.TTF", 16.0f);

    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(glfw_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

racer::ApplicationUI::~ApplicationUI()
{
    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void racer::ApplicationUI::StartIMGUIFrame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void racer::ApplicationUI::SetIMGUILayout()
{
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

    const ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", NULL, window_flags);
    ImGui::PopStyleVar();

    ImGui::PopStyleVar(2);

    // Submit the DockSpace
    io = &ImGui::GetIO();
    if (io->ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Scene", "CTRL+N"))
            {
                throw "New Scene";
            }

            if (ImGui::MenuItem("Save Scene", "CTRL+S"))
            {
            }

            if (ImGui::MenuItem("Open Scene", "CTRL+O"))
            {
            }

            ImGui::Separator();

            if (ImGui::MenuItem("Close", "ALT+F4"))
            {
                close_window_ = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Render"))
        {
            if (ImGui::MenuItem("Render Image"))
            {
                throw "Render Image";
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End();
}

void racer::ApplicationUI::GetPreviewWindowSize(int &width, int &height)
{
    width = preview_window_width_;
    height = preview_window_height_;
}

void racer::ApplicationUI::RenderPreviewWindow(ImTextureID im_texture_id)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (ImGui::Begin("Preview Window", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        preview_window_width_ = static_cast<int>(ImGui::GetContentRegionAvail().x);
        preview_window_height_ = static_cast<int>(ImGui::GetContentRegionAvail().y);

        ImGui::Image(im_texture_id, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void racer::ApplicationUI::RenderRenderResultWindow(ImTextureID im_texture_id)
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (ImGui::Begin("Render Result", NULL, ImGuiWindowFlags_HorizontalScrollbar))
    {
        ImGuiIO &io = ImGui::GetIO();
        ImGui::Image(im_texture_id, ImVec2(static_cast<float>(render_properties_.width), static_cast<float>(render_properties_.height)));
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void racer::ApplicationUI::RenderRenderOptionsWindow()
{
    if (ImGui::Begin("Render Options"))
    {
        ImGuiIO &io = ImGui::GetIO();
        ImGui::InputInt("width", &render_properties_.width);
        ImGui::InputInt("height", &render_properties_.height);
        if (ImGui::SmallButton("Render"))
            start_render_ = true;
    }
    ImGui::End();
}

void racer::ApplicationUI::RenderSceneComponentHirerchy(Scene *scene)
{
    if (ImGui::Begin("Scene Hierarchy"))
    {
        if (ImGui::TreeNode(("Entities (" + scene->name_ + ")").c_str()))
        {
            for (auto entity : scene->entities_)
            {
                ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
                ImGuiTreeNodeFlags node_flags = base_flags;
                const bool is_selected = entity->IsActive();
                if (is_selected)
                    node_flags |= ImGuiTreeNodeFlags_Selected;
                node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen; // ImGuiTreeNodeFlags_Bullet
                ImGui::TreeNodeEx((void *)entity, node_flags, entity->GetName().c_str());
                if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
                {
                    for (auto entity_not_selected : scene->entities_)
                    {
                        entity_not_selected->SetActive(false);
                    }
                    entity->SetActive(true);
                }
            }
            ImGui::TreePop();
        }
    }
    ImGui::End();

    if (ImGui::Begin("Components Inspector"))
    {
        for (auto entity : scene->entities_)
        {
            if (entity->IsActive())
            {
                for (auto component : entity->components_)
                {
                    ImGui::Text(component->GetName().c_str());

                    Transform *transformComponent = dynamic_cast<Transform *>(component);
                    if (transformComponent)
                    {
                        ImGui::DragFloat3("Position", glm::value_ptr(transformComponent->position_), 0.01f);
                        ImGui::DragFloat3("Rotation", glm::value_ptr(transformComponent->rotation_), 0.01f);
                        ImGui::DragFloat3("Scale", glm::value_ptr(transformComponent->scale_), 0.01f);
                    }

                    Light *lightComponent = dynamic_cast<Light *>(component);
                    if (lightComponent)
                    {
                        ImGui::ColorEdit3("Color", glm::value_ptr(lightComponent->color_));
                    }

                    Camera *cameraComponent = dynamic_cast<Camera *>(component);
                    if (cameraComponent)
                    {
                        ImGui::DragFloat("Horizontal FOV", &cameraComponent->fov_y_, 0.01f);
                    }

                    RendererComponent *rendererComponentComponent = dynamic_cast<RendererComponent *>(component);
                    if (rendererComponentComponent)
                    {
                        ImGui::ColorEdit3("Color", glm::value_ptr(rendererComponentComponent->rendering_material_.color));

                        ImGui::DragFloat("Ka", &rendererComponentComponent->rendering_material_.Ka, 0.01f);
                        ImGui::DragFloat("Kd", &rendererComponentComponent->rendering_material_.Kd, 0.01f);
                        ImGui::DragFloat("Kr", &rendererComponentComponent->rendering_material_.Kr, 0.01f);
                        ImGui::DragFloat("Ks", &rendererComponentComponent->rendering_material_.Ks, 0.01f);
                    }

                    ImGui::Separator();
                }
                break;
            }
        }
    }
    ImGui::End();
}

void racer::ApplicationUI::RenderIMGUIFrame()
{
    // ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool racer::ApplicationUI::IsWindowClosed() const
{
    return close_window_;
}

bool racer::ApplicationUI::RayTraceRender(RenderProperties &render_properties)
{
    render_properties = render_properties_;
    return start_render_;
}

void racer::ApplicationUI::FinishedRayTracing()
{
    start_render_ = false;
}