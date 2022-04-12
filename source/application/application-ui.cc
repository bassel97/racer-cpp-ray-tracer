#include "application-ui.h"

racer::ApplicationUI::ApplicationUI(GLFWwindow *glfw_window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = &ImGui::GetIO();
    (void)io;
    io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

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

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    if (ImGui::Begin("Render Result", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove))
    {
        ImGuiIO &io = ImGui::GetIO();
        ImTextureID my_tex_id = io.Fonts->TexID;
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
        ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);   // No tint
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white
        ImGui::Image(my_tex_id, (ImGui::GetContentRegionAvail()), uv_min, uv_max, tint_col, border_col);
    }
    ImGui::End();
    ImGui::PopStyleVar();

    if (ImGui::Begin("Scene Hierarchy", NULL, ImGuiWindowFlags_NoMove))
    {
    }
    ImGui::End();

    if (ImGui::Begin("Component Inspector", NULL, ImGuiWindowFlags_NoMove))
    {
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
    if (ImGui::Begin("Preview Window", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoMove))
    {
        preview_window_width_ = ImGui::GetContentRegionAvail().x;
        preview_window_height_ = ImGui::GetContentRegionAvail().y;

        ImGui::Image(im_texture_id, ImGui::GetContentRegionAvail());
    }
    ImGui::End();
    ImGui::PopStyleVar();
}

void racer::ApplicationUI::RenderIMGUIFrame()
{
    ImGui::ShowDemoWindow();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool racer::ApplicationUI::IsWindowClosed() const
{
    return close_window_;
}