#include "VanaheimPCH.h"
#include "UIManager.h"

// ImGui
#include "backends\imgui_impl_dx11.h"
#include "backends\imgui_impl_win32.h"

// Vanaheim Engine
#include "Window.h"
#include "Graphics.h"
#include "SceneSerializer.h"
#include "Scene.h"
#include "UI.h"

UIManager::UIManager(Window* pWindow)
		  : m_pWindow(pWindow)
          , m_pMainViewport(nullptr)
          , m_pUIs(std::vector<UI*>())
{}
UIManager::~UIManager()
{
	DELETE_POINTERS(m_pUIs, m_pUIs.size());
	ShutdownImGui();
}

void UIManager::Initialize()
{
    Window* pWindow{ Locator::GetWindowService() };
    Graphics* pGraphics{ Locator::GetGraphicsService() };

    InitializeImGui(pWindow, pGraphics);
}
void UIManager::PostInitialize()
{
    InitializeUIs();
}

void UIManager::BeginFrame()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();    
    ImGui::NewFrame();

    OpenDockSpace();
}
void UIManager::Render()
{
	for (UI* pUI : m_pUIs)
		pUI->ShowWindow();
}
void UIManager::EndFrame()
{
    CloseDockSpace();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}
}

void UIManager::InitializeImGui(Window* pWindow, Graphics* pGraphics)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

    io.ConfigViewportsNoDecoration = true;

    if (!ImGui_ImplWin32_Init(pWindow->GetWindowHandle()))
        LOG(ErrorLevel::LOG_ERROR, "UIManager::UIManager >> ImGui_ImplWin32_Init << failed");

    if (!ImGui_ImplDX11_Init(pGraphics->GetDevice(), pGraphics->GetDeviceContext()))
        LOG(ErrorLevel::LOG_ERROR, "UIManager::UIManager >> ImGui_ImplDX11_Init << failed");

    // Setup Dear ImGui style
    io.Fonts->AddFontFromFileTTF("../VanaheimEngine/Resources/Fonts/Roboto/Roboto-Bold.ttf", 15.f);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("../VanaheimEngine/Resources/Fonts/Roboto/Roboto-Regular.ttf", 15.f);
    ImGui::StyleColorsDark();
    SetThemeColors();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    m_pMainViewport = ImGui::GetMainViewport();
}
void UIManager::InitializeUIs()
{
    for (UI* pUI : m_pUIs)
        pUI->Initialize();
}
void UIManager::ShutdownImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void UIManager::OpenDockSpace()
{
    /** Code from ImGui::ShowDemoWIndow() */
    static bool dockSpaceOpen = false;
    static bool opt_fullscreen = true;
    static bool opt_padding = false;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGui::SetNextWindowPos(m_pMainViewport->WorkPos);
    ImGui::SetNextWindowSize(m_pMainViewport->WorkSize);
    ImGui::SetNextWindowViewport(m_pMainViewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace", &opt_fullscreen, window_flags);
    if (!opt_padding)
        ImGui::PopStyleVar();

    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Submit the DockSpace
    ImGuiStyle& style = ImGui::GetStyle();
    const float minWinSizeX{ style.WindowMinSize.x };
    style.WindowMinSize.x = 370.f;

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }
    style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            Scene* pScene = Locator::GetSceneManagerService()->GetActiveGameScene();
            if (ImGui::MenuItem("Save"))
            {
                SceneSerializer serializer{};
                serializer.Serialize("Scenes/Scene.Vanaheim", pScene);
            }
            if (ImGui::MenuItem("DeSave"))
            {
                SceneSerializer serializer{};
                serializer.Deserialize("Scenes/Scene.Vanaheim", pScene);
            }
            if (ImGui::MenuItem("Exit"))
            {
                Locator::GetInputManagerService()->QuitGame();
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::BeginMenu("Window size"))
            {
                //Graphics* pGraphics{ Locator::GetGraphicsService() };
                if (ImGui::MenuItem("1920x1080"))
                {
                    //pGraphics->SetWindowDimensions(1920, 1080);
                    //pGraphics->ResizeWindow({ 1920, 1080 });
                }
                if (ImGui::MenuItem("2560x1080"))
                {
                    //pGraphics->SetWindowDimensions(2560, 1080);
                    //pGraphics->ResizeWindow({ 2560, 1080 });
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Fullscreen"))
            {
                //Graphics* pGraphics{ Locator::GetGraphicsService() };
                //pGraphics->SetFullScreen(true);
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Window"))
        {
            // Open or Close UI windows
            // Viewport
            ViewportUI* pViewport{ GetUI<ViewportUI>() };
            ImGui::Checkbox("Viewport", pViewport->CanRenderUI());

            // Inspector
            InspectorUI* pInspector{ GetUI<InspectorUI>() };
            ImGui::Checkbox("Inspector", pInspector->CanRenderUI());

            // Console
            ConsoleUI* pConsole{ GetUI<ConsoleUI>() };
            ImGui::Checkbox("Console", pConsole->CanRenderUI());

            // Scene hierarchy
            HierarchyUI* pHierarchy{ GetUI<HierarchyUI>() };
            ImGui::Checkbox("Hierarchy", pHierarchy->CanRenderUI());

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Tool"))
        {
            if (ImGui::MenuItem("Close scene"))
            {
                Scene* pScene = Locator::GetSceneManagerService()->GetActiveGameScene();

                std::vector<GameObject*> pGameObjects{ pScene->GetObjects() };

                const size_t objectCount{ pGameObjects.size() };
                for (size_t i{}; i < objectCount; ++i)
                    pGameObjects[i]->SetRemoveFlag();
            }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}
void UIManager::CloseDockSpace()
{
    ImGui::End();
}

void UIManager::SetThemeColors()
{
    const float alpha{ 1.f };

    ImGuiStyle& style{ ImGui::GetStyle() };
    ImVec4 (&colors)[55]{ style.Colors };
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, alpha };

    // Headers
    colors[ImGuiCol_Header]         = ImVec4{ 0.20f, 0.2050f, 0.210f, alpha };
    colors[ImGuiCol_HeaderHovered]  = ImVec4{ 0.30f, 0.3050f, 0.310f, alpha };
    colors[ImGuiCol_HeaderActive]   = ImVec4{ 0.15f, 0.1505f, 0.151f, alpha };

    // Buttons
    colors[ImGuiCol_Button]         = ImVec4{ 0.20f, 0.2050f, 0.210f, alpha };
    colors[ImGuiCol_ButtonHovered]  = ImVec4{ 0.30f, 0.3050f, 0.310f, alpha };
    colors[ImGuiCol_ButtonActive]   = ImVec4{ 0.15f, 0.1505f, 0.151f, alpha };

    // Frame bg
    colors[ImGuiCol_FrameBg]        = ImVec4{ 0.20f, 0.2050f, 0.210f, alpha };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.30f, 0.3050f, 0.310f, alpha };
    colors[ImGuiCol_FrameBgActive]  = ImVec4{ 0.15f, 0.1505f, 0.151f, alpha };

    // Tabs
    colors[ImGuiCol_Tab]                = ImVec4{ 0.15f, 0.1505f, 0.151f, alpha };
    colors[ImGuiCol_TabHovered]         = ImVec4{ 0.38f, 0.3805f, 0.381f, alpha };
    colors[ImGuiCol_TabActive]          = ImVec4{ 0.28f, 0.2805f, 0.281f, alpha };
    colors[ImGuiCol_TabUnfocused]       = ImVec4{ 0.15f, 0.1505f, 0.151f, alpha };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.20f, 0.2050f, 0.210f, alpha };

    colors[ImGuiCol_TitleBg]            = ImVec4{ 0.15f, 0.1505f, 0.151f, alpha };
    colors[ImGuiCol_TitleBgActive]      = ImVec4{ 0.15f, 0.1505f, 0.151f, alpha };
    colors[ImGuiCol_TitleBgCollapsed]   = ImVec4{ 0.15f, 0.1501f, 0.151f, alpha };
}