#include "pch.h"
#include "D3D11ImGuiRenderer.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include "Graphics/D3D11/D3D11RenderObject.h"
#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"
#include "Graphics/D3D11/Effect/D3D11Effect.h"
#include "Graphics/D3D11/D3D11CubeMap.h"
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

    bool showDemoWindow {true};
    if (showDemoWindow) [[likely]] {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    ShowMenu();
    ImGuiShowCubemapEditWindow();
    ImGuiShowPostProcessEditWindow();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowCubemapEditWindow() {
    auto& index {D3D11CubeMap::TextureIndex()};

    if (ImGui::Begin("Cubemap Edit Window")) {
        ImGui::RadioButton("Santa Maria (No IBL)", &index, 0);
        ImGui::RadioButton("Fort Point (No IBL)", &index, 1);
        ImGui::RadioButton("Atrium", &index, 2);
        ImGui::RadioButton("Garage", &index, 3);
    }
    ImGui::End();
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowSceneEditWindow(x_vector<std::shared_ptr<D3D11RenderObject>>& objs) {
    if (ImGui::Begin("Scene Edit Window")) {
        ImGui::Columns(3, nullptr, true);
        for (auto& obj : objs)
            ImGuiRenderSceneTree(*obj->GetScene(), 0);

        ImGui::NextColumn();
        if (_selected.first != nullptr) {
            ImGuiShowNodeEditWindow();

            ImGui::NextColumn();
            ImGuiShowRenderConfigureWindow();
            ImGuiShowMaterialEditWindow();
        }
    }
    ImGui::End();
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowNodeEditWindow() {
    auto& [scene, node] {_selected};
    int interacted {};

    auto& transform {scene->GetTransformParamAt(node)};

    ImGui::Text(scene->GetNameAt(node));
    ImGui::Text("Position");
    interacted += ImGui::SliderFloat("X", &transform.x, -20.0f, 20.0f);
    interacted += ImGui::SliderFloat("Y", &transform.y, -20.0f, 20.0f);
    interacted += ImGui::SliderFloat("Z", &transform.z, -20.0f, 20.0f);

    ImGui::Text("Rotation");
    interacted += ImGui::SliderAngle("Roll", &transform.roll, -180.0f, 180.0f);
    interacted += ImGui::SliderAngle("Pitch", &transform.pitch, -180.0f, 180.0f);
    interacted += ImGui::SliderAngle("Yaw", &transform.yaw, -180.0f, 180.0f);

    ImGui::Text("Scaling");
    interacted += ImGui::SliderFloat("Scale X", &transform.scale_x, 0.0f, 10.0f);
    interacted += ImGui::SliderFloat("Scale Y", &transform.scale_y, 0.0f, 10.0f);
    interacted += ImGui::SliderFloat("Scale Z", &transform.scale_z, 0.0f, 10.0f);

    if (interacted > 0)
        scene->MarkAsUpdated(node);
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowRenderConfigureWindow() {
    static char const* methods[] {"Solid", "Phong", "PBR"};
    static int selected_method {};
    ImGui::Text("Rendering Configuration");
    ImGui::Combo("Shading", &selected_method, methods, std::size(methods));
    _renderConfigureTable[selected_method]();
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowMaterialEditWindow() {
    ImGui::Text("Material Constants Configuration");
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowPostProcessEditWindow() {
    ImGui::Text("Post Process");
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

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowSolidConfigureWindow() {
    auto& [scene, node] {_selected};
    auto& render_strategy {scene->GetRenderStrategyAt(node)};

    int temp {};
    bool use_diffusemap {(render_strategy & (0b1 << static_cast<int>(ShaderResourceTypes::DiffuseMap))) != false};
    bool use_heightmap {(render_strategy & (0b1 << static_cast<int>(ShaderResourceTypes::HeightMap))) != false};

    ImGui::Checkbox("Diffuse Map", &use_diffusemap);
    ImGui::Checkbox("Height Map", &use_heightmap);
    ImGui::RadioButton("Environment Mapping", &temp, 0);
    ImGui::RadioButton("Basic IBL", &temp, 1);

    uint64_t current_value {0b0};
    current_value |= (0b1 << static_cast<int>(ShaderResourceTypes::DiffuseMap)) * use_diffusemap;
    current_value |= (0b1 << static_cast<int>(ShaderResourceTypes::HeightMap)) * use_heightmap;
    if (render_strategy != current_value) {
        render_strategy = current_value; 
        scene->SetRenderStrategies(node, render_strategy);
    }
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowPhongConfigureWindow() {
    auto& [scene, node] {_selected};
    auto& render_strategy {scene->GetRenderStrategyAt(node)};

    bool use_diffusemap {(render_strategy & (0b1 << static_cast<int>(ShaderResourceTypes::DiffuseMap))) != false};
    bool use_specularmap {(render_strategy & (0b1 << static_cast<int>(ShaderResourceTypes::SpecularMap))) != false};
    bool use_normalmap {(render_strategy & (0b1 << static_cast<int>(ShaderResourceTypes::NormalMap))) != false};
    bool use_heightmap {(render_strategy & (0b1 << static_cast<int>(ShaderResourceTypes::HeightMap))) != false};
    bool use_rim_effect {(render_strategy & (0b1 << static_cast<int>(EffectTypes::RimEffect))) != false};
    bool use_fresnel_effect {(render_strategy & (0b1 << static_cast<int>(EffectTypes::FresnelEffect))) != false};

    ImGui::Text("Textures");
    ImGui::Checkbox("Diffuse Map", &use_diffusemap);
    ImGui::Checkbox("Specular Map", &use_specularmap);
    ImGui::Checkbox("Normal Map", &use_normalmap);
    ImGui::Checkbox("Height Map", &use_heightmap);

    ImGui::Text("Effects");
    ImGui::Checkbox("Rim Effect", &use_rim_effect);
    ImGui::Checkbox("Fresnel Effect", &use_fresnel_effect);

    uint64_t current_value {0b1};
    current_value |= (0b1 << static_cast<int>(ShaderResourceTypes::DiffuseMap))  * use_diffusemap;
    current_value |= (0b1 << static_cast<int>(ShaderResourceTypes::SpecularMap)) * use_specularmap;
    current_value |= (0b1 << static_cast<int>(ShaderResourceTypes::NormalMap))   * use_normalmap;
    current_value |= (0b1 << static_cast<int>(ShaderResourceTypes::HeightMap))   * use_heightmap;
    current_value |= (0b1 << static_cast<int>(EffectTypes::RimEffect))           * use_rim_effect;
    current_value |= (0b1 << static_cast<int>(EffectTypes::FresnelEffect))       * use_fresnel_effect;
    if (render_strategy != current_value) {
        render_strategy = current_value; 
        scene->SetRenderStrategies(node, render_strategy);
    }

    //static bool use_cubemap {false};
    //ImGui::Checkbox("use Cubemap Texture", &use_cubemap);
    //if (use_cubemap) {
    //    if (ImGui::RadioButton("Environment Mapping", &render_strategy, 0))
    //        ImGuiShowEMConfigureWindow();        
    //    else if (ImGui::RadioButton("Basic IBL", &render_strategy, 1))
    //        ImGuiShowBasicIBLConfigureWindow(); 
    //}
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowPBRConfigureWindow() {
    auto& [scene, node] {_selected};
    auto& render_strategy {scene->GetRenderStrategyAt(node)};
    ImGui::Text("To be implemented");
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowEMConfigureWindow() {
    auto& [scene, node] {_selected};
    auto& render_strategy {scene->GetRenderStrategyAt(node)};
    auto const previous_value {render_strategy};

    //ImGui::RadioButton("Use Normal Direction", &render_strategy, 4);
    //ImGui::RadioButton("Use Total Reflection", &render_strategy, 5);

    if (previous_value != render_strategy)
        scene->SetRenderStrategies(node, render_strategy);
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowBasicIBLConfigureWindow() {
    auto& [scene, node] {_selected};
    auto& render_strategy {scene->GetRenderStrategyAt(node)};
    auto const previous_value {render_strategy};

    //ImGui::RadioButton("Default", &render_strategy, 6);
    //ImGui::RadioButton("Textured", &render_strategy, 7);

    if (previous_value != render_strategy)
        scene->SetRenderStrategies(node, render_strategy);
}

void Engine::Graphics::D3D11ImGuiRenderer::ShowMenu() {
    if (ImGui::Begin("Menu")) {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();
}
