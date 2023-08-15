#include "pch.h"
#include "D3D11ImGuiRenderer.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include "Graphics/D3D11/D3D11RenderObject.h"
#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"
#include "Platform/Platform.h"

void Engine::Graphics::D3D11ImGuiRenderer::Init(ID3D11Device* device, ID3D11DeviceContext* context) {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    //io.ConfigViewportsNoAutoMerge = true;
    //io.ConfigViewportsNoTaskBarIcon = true;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(Platform::GetNativeWnd());
    ImGui_ImplDX11_Init(device, context);

    imguiEnabled = true;
}

void Engine::Graphics::D3D11ImGuiRenderer::Shutdown() {
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Engine::Graphics::D3D11ImGuiRenderer::BeginFrame() {
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void Engine::Graphics::D3D11ImGuiRenderer::EndFrame() {
    auto& io {ImGui::GetIO()};
    io.DisplaySize = ImVec2(static_cast<float>(Platform::GetWidth()), 
                            static_cast<float>(Platform::GetHeight()));

    bool showDemoWindow {false};
    if (showDemoWindow) [[unlikely]] {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    ShowMenu();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowSceneEditWindow(x_vector<std::shared_ptr<D3D11RenderObject>>& objs) {
    if (ImGui::Begin("Scene Edit Window")) {
        ImGui::Columns(2, nullptr, true);
        for (auto& obj : objs)
            ImGuiRenderSceneTree(*obj->GetScene(), 0);

        ImGui::NextColumn();
        if (_selected.first != nullptr)
            ImGuiShowNodeEditWindow();
    }
    ImGui::End();

}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowNodeEditWindow() {
    auto& [scene, node] {_selected};

    auto& transform {scene->GetTransformParamAt(node)};

    ImGui::Text(scene->GetNameAt(node));
    ImGui::Text("Position");
    ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f);
    ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f);
    ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f);

    ImGui::Text("Rotation");
    ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
    ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
    ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);

    scene->MarkAsUpdated(node);
}

int32_t Engine::Graphics::D3D11ImGuiRenderer::ImGuiRenderSceneTree(D3D11SceneGraph& scene, int32_t node) {
    std::string const name = {scene.GetNameAt(node)};
	std::string const label = name.empty() ? (std::string("Node") + std::to_string(node)) : name;

	const int flags = (scene.GetNodeAt(node)._firstChild < 0) ? ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_Bullet : 0;

	const bool opened = ImGui::TreeNodeEx(&scene.GetNodeAt(node), flags, "%s", label.c_str());

	ImGui::PushID(node);

	if (ImGui::IsItemClicked())
	{
		printf("Selected node: %d (%s)\n", node, label.c_str());
        _selected.first = &scene;
		_selected.second = node;
	}

	if (opened)
	{
		for (int ch = scene.GetNodeAt(node)._firstChild; ch != -1; ch = scene.GetNodeAt(ch)._nextSibling)
		{
			auto const subNode = ImGuiRenderSceneTree(scene, ch);
			if (subNode > -1)
				_selected.second = subNode;
		}
		ImGui::TreePop();
	}

	ImGui::PopID();

	return _selected.second;
}

void Engine::Graphics::D3D11ImGuiRenderer::ShowMenu() {
    static float f = 0.0f;
    static int counter = 0;

    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
    ImGui::SameLine();
    ImGui::Text("counter = %d", counter);

    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}
