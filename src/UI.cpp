#include "UI.h"

void UI::ImGuiRender()
{
    Scene* scene = m_ActiveRenderer->GetScnPtr();
    
    ImGuiIO& io = ImGui::GetIO();
    io.FontDefault->Scale = 0.25f * ImGui::GetWindowDpiScale();
    
    ImGui::DockSpaceOverViewport();
    ImGui::GetStyle().WindowPadding = ImVec2(0, 0);
    bool is_open;
    ImGui::Begin("ImageWindow", &is_open, ImGuiWindowFlags_NoTitleBar);
    ImTextureID tex = ImTextureID(1);
    m_ActiveRenderer->Resize(glm::ivec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y));
    ImVec2 imageSize = ImVec2(m_ActiveRenderer->GetImgPtr()->size.x, m_ActiveRenderer->GetImgPtr()->size.y);
    ImGui::Image(tex, imageSize, ImVec2(1, 1), ImVec2(0, 0));
    ImGui::End();
    ImGui::GetStyle().WindowPadding = ImVec2(8, 8);

    ImGui::Begin("Render");
    ImGui::Text("%.3fms", m_sceneDiff * 1000.0f);
    ImGui::Text("%.3fFPS", m_sceneFPS);
    ImGui::Checkbox("Accumulation", &m_ActiveRenderer->GetSettings().Accumulate);

    ImageSave();
    CameraEditor();
    ShapesEditor();
    ContentBrowser();

    ImGui::Begin("Scene");
    ImGui::InputText("Name", fileName, sizeof(fileName));
    if (ImGui::Button("Save"))
    {
        std::string name = fileName;
        name += ".scene";
        scene->Save(name);
    }
    ImGui::End();
}

UI::UI(glm::ivec2 dimensions)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.WantCaptureMouse = true;
    io.Fonts->AddFontFromFileTTF("fonts\\Sansita-Regular.ttf", 80.0f);
    io.FontDefault = io.Fonts->Fonts[0];

    ImGuiPlatformIO& p_io = ImGui::GetPlatformIO(); (void)p_io;
    ImGui::RenderPlatformWindowsDefault();

    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;
    io.BackendFlags |= ImGuiBackendFlags_PlatformHasViewports;

    ImGuiWindowClass wclass;
    io.ConfigViewportsNoDefaultParent = true;
    wclass.ViewportFlagsOverrideSet = ImGuiWindowFlags_NoBringToFrontOnFocus;
    wclass.ViewportFlagsOverrideSet = ImGuiViewportFlags_CanHostOtherWindows;

    loader::GLFW();
    m_window = new Window(dimensions.x, dimensions.y, "RT.png");
    loader::GladGL();
    ImGui_ImplGlfw_InitForOpenGL(m_window->GetID(), true);
    ImGui_ImplOpenGL3_Init("#version 460");

    OpenGL::Get().CreateTexture();
    SetImGuiDarkTheme();
}

void UI::UpdateImGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGuiRender();

    ImGui::Render();
    ImGui::EndFrame();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui::UpdatePlatformWindows();
    ImGui::RenderPlatformWindowsDefault();
}

void UI::Update(Renderer& renderer, float FPS, float diff)
{
    m_ActiveRenderer = &renderer;
    m_sceneFPS = FPS;
    m_sceneDiff = diff;

    m_ActiveRenderer->GetCamPtr()->CalsculateMovement(m_window->GetID(), diff);
    glfwGetFramebufferSize(m_window->GetID(), &m_width, &m_height);
    OpenGL::Get().UpdateViewport(m_width, m_height);
    OpenGL::Get().UpdateTexture(m_ActiveRenderer->GetImgPtr());
    UpdateImGui();

    m_window->SwapBuffers();
    m_window->PollEvents();
    glfwMakeContextCurrent(m_window->GetID());
}

bool UI::IsRunning()
{
    return !glfwWindowShouldClose(m_window->GetID());
}

void UI::SetImGuiDarkTheme()
{
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
    colors[ImGuiCol_Border] = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
    colors[ImGuiCol_FrameBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_CheckMark] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_SliderGrab] = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
    colors[ImGuiCol_Button] = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_SeparatorHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLines] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);
}

void UI::ImageSave()
{
    const Image* image = m_ActiveRenderer->GetImgPtr();

    ImGui::Begin("Image");
    ImGui::InputText("Name", imageName, 200);

    if (ImGui::Button("Save"))
    {
        std::string name = imageName;
        std::string folder = "images/" + name + ".png";

        if (!std::filesystem::is_directory("images"))
            std::filesystem::create_directory("images");
        stbi_flip_vertically_on_write(true);
        stbi_write_png(folder.c_str(), image->size.x, image->size.y, 4, image->data, image->size.x * 4);
    }
    ImGui::End();
}

void UI::CameraEditor()
{
    Camera* camera = m_ActiveRenderer->GetCamPtr();

    ImGui::End();;
    ImGui::Begin("Camera");
    ImGui::SliderAngle("FOV", &camera->GetSettings().vfov, 10.f, 120.f);
    ImGui::End();
}

void UI::ContentBrowser()
{
    ImGui::Begin("Content Browser");

    if (m_currentDir != std::filesystem::path(m_assetsDir))
    {
        if (ImGui::Button("<-"))
        {
            m_currentDir = m_currentDir.parent_path();
        }
    }

    if (std::filesystem::exists(m_currentDir))
    {
        for (auto& p : std::filesystem::directory_iterator(m_currentDir))
        {
            std::string path = p.path().string();
            if (p.is_directory())
            {
                if (ImGui::Button(path.c_str()))
                {
                    m_currentDir /= p.path().filename();
                }
            }

            if (!p.is_directory())
            {
                if (ImGui::Button(p.path().filename().string().c_str()))
                {
                    if (p.path().extension() == ".scene")
                    {
                        m_ActiveRenderer->GetScnPtr()->Load(path);
                    }

                    else if (p.path().extension() == ".hdr")
                    {
                        m_ActiveRenderer->GetScnPtr()->LoadSkybox(path);
                    }
                }
            }
        }
    }
    ImGui::End();
}

void UI::ShapesEditor()
{
    std::vector<Shape*>& Shapes = m_ActiveRenderer->GetScnPtr()->shapes;

    ImGui::Begin("Spheres");
    for (int i = 0; i < Shape::SpherePos.size(); i++)
    {
        std::string currentSphere = "Sphere " + std::to_string(i);
        if (ImGui::CollapsingHeader(currentSphere.c_str()))
        {
            std::string number = "##" + std::to_string(i);
            std::string radius = "Radius" + number;
            std::string smoothness = "Smoothness" + number;
            std::string albido = "Albido" + number;
            std::string center = "Center" + number;
            std::string specularColor = "SpecularColor" + number;
            std::string specularProb = "SpecularProb" + number;
            std::string emissionColor = "Em_Color" + number;
            std::string emissionStrength = "Em_Strength" + number;

            Shape* shape = Shapes[Shape::SpherePos[i]];
            Sphere* sphere = static_cast<Sphere*>(shape);
            ImGui::DragFloat3(center.c_str(), &sphere->centerPtr()->x, 0.01f);
            ImGui::DragFloat(radius.c_str(), sphere->radiusPtr(), 0.01f);
            ImGui::Checkbox("Metalic", &sphere->materialPtr()->isMetalic);
            ImGui::ColorEdit4(albido.c_str(), &sphere->materialPtr()->albido.x);
            ImGui::ColorEdit4(specularColor.c_str(), &sphere->materialPtr()->specularColor.x);
            ImGui::SliderFloat(smoothness.c_str(), &sphere->materialPtr()->smoothness, 0.0f, 1.0f);
            ImGui::SliderFloat(specularProb.c_str(), &sphere->materialPtr()->specularProbability, 0.0f, 1.0f);

            ImGui::Checkbox("Light Source", &sphere->materialPtr()->isLightSource);

            if (sphere->material().isLightSource)
            {
                ImGui::ColorEdit4(emissionColor.c_str(), &sphere->materialPtr()->emissionColor.x);
                ImGui::DragFloat(emissionStrength.c_str(), &sphere->materialPtr()->emissionStrength, 0.05);
            }

            if (sphere->material().isMetalic)
            {
                sphere->materialPtr()->specularColor = sphere->materialPtr()->albido;
            }
        }
    }
    if (ImGui::Button("-", ImVec2(100.0f, 100.0f)))
    {
        if (Shape::SpherePos.size() > 1)
        {
            int lastSphere = Shape::SpherePos[Shape::SpherePos.size() - 1];
            delete Shapes[lastSphere];
            Shapes.erase(Shapes.begin() + lastSphere);
            Shape::SpherePos.pop_back();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("+", ImVec2(100.0f, 100.0f)))
    {
        Shape::SpherePos.push_back(Shapes.size());
        Sphere* newSphere = new Sphere();
        newSphere->material() = Shapes[Shape::SpherePos[0]]->material();
        newSphere->center() = static_cast<Sphere*>(Shapes[Shape::SpherePos[0]])->center();
        newSphere->radius() = static_cast<Sphere*>(Shapes[Shape::SpherePos[0]])->radius();
        Shapes.push_back(newSphere);
    }
    ImGui::End();

    ImGui::Begin("Planes");
    for (int i = 0; i < Shape::PlanePos.size(); i++)
    {
        std::string currentPlane = "Plane " + std::to_string(i);
        if (ImGui::CollapsingHeader(currentPlane.c_str()))
        {
            std::string number = "##" + std::to_string(i);
            std::string smoothness = "Smoothness" + number;
            std::string albido = "Albido" + number;
            std::string center = "Point" + number;
            std::string specularColor = "SpecularColor" + number;
            std::string specularProb = "SpecularProb" + number;

            Shape* shape = Shapes[Shape::PlanePos[i]];
            Plane* plane = static_cast<Plane*>(shape);
            ImGui::DragFloat(center.c_str(), &plane->pointPtr()->y, 0.01f);
            ImGui::ColorEdit4(albido.c_str(), &plane->materialPtr()->albido.x);
            ImGui::ColorEdit4(specularColor.c_str(), &plane->materialPtr()->specularColor.x);
            ImGui::SliderFloat(smoothness.c_str(), &plane->materialPtr()->smoothness, 0.0f, 1.0f);
            ImGui::SliderFloat(specularProb.c_str(), &plane->materialPtr()->specularProbability, 0.0f, 1.0f);
        }
    }
    if (ImGui::Button("-", ImVec2(100.0f, 100.0f)))
    {
        if (Shape::PlanePos.size() > 0)
        {
            int lastPlane = Shape::PlanePos[Shape::PlanePos.size() - 1];
            delete Shapes[lastPlane];
            Shapes.erase(Shapes.begin() + lastPlane);
            Shape::PlanePos.pop_back();
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("+", ImVec2(100.0f, 100.0f)))
    {
        Plane* newPlane = new Plane();
        newPlane->material() = Shapes[Shape::PlanePos[0]]->material();
        newPlane->point() = static_cast<Plane*>(Shapes[Shape::PlanePos[0]])->point();
        newPlane->normal() = static_cast<Plane*>(Shapes[Shape::PlanePos[0]])->normal();
        Shape::PlanePos.push_back(Shapes.size());
        Shapes.push_back(newPlane);

    }
    ImGui::End();
}
