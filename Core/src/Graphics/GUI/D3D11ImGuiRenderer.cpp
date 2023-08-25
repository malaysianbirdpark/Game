#include "pch.h"
#include "D3D11ImGuiRenderer.h"

#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

#include "Graphics/D3D11/D3D11Core.h"
#include "Graphics/D3D11/D3D11RenderObject.h"
#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"
#include "Graphics/D3D11/Effect/D3D11Effect.h"
#include "Graphics/D3D11/D3D11Cubemap.h"
#include "Graphics/D3D11/ConstantBuffer/D3D11PhongMaterialConstants.h"
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

    ImGuiShowDockSpace();
    ImGuiShowViewport();
    ImGuiShowSystemInfo();
    ImGuiShowRSEditWindow();
    ImGuiShowCubemapEditWindow();
    ImGuiShowPostProcessEditWindow();
}

void Engine::Graphics::D3D11ImGuiRenderer::EndFrame() {
    auto& io {ImGui::GetIO()};
    io.DisplaySize = ImVec2(static_cast<float>(Platform::GetWidth()), 
                            static_cast<float>(Platform::GetHeight()));

    bool showDemoWindow {true};
    if (showDemoWindow) [[likely]] {
        ImGui::ShowDemoWindow(&showDemoWindow);
    }

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowDockSpace() {
    static bool p_open {true};

    // Variables to configure the Dockspace example.
    static bool opt_fullscreen = true; // Is the Dockspace full-screen?
    static bool opt_padding = false; // Is there padding (a blank space) between the window edge and the Dockspace?
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None; // Config flags for the Dockspace

    // In this example, we're embedding the Dockspace into an invisible parent window to make it more configurable.
    // We set ImGuiWindowFlags_NoDocking to make sure the parent isn't dockable into because this is handled by the Dockspace.
    //
    // ImGuiWindowFlags_MenuBar is to show a menu bar with config options. This isn't necessary to the functionality of a
    // Dockspace, but it is here to provide a way to change the configuration flags interactively.
    // You can remove the MenuBar flag if you don't want it in your app, but also remember to remove the code which actually
    // renders the menu bar, found at the end of this function.
    //ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

    // Is the example in Fullscreen mode?
    if (opt_fullscreen)
    {
        // If so, get the main viewport:
        const ImGuiViewport* viewport = ImGui::GetMainViewport();

        // Set the parent window's position, size, and viewport to match that of the main viewport. This is so the parent window
        // completely covers the main viewport, giving it a "full-screen" feel.
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        // Set the parent window's styles to match that of the main viewport:
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f); // No corner rounding on the window
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f); // No border around the window

        // Manipulate the window flags to make it inaccessible to the user (no titlebar, resize/move, or navigation)
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }
    else
    {
        // The example is not in Fullscreen mode (the parent window can be dragged around and resized), disable the
        // ImGuiDockNodeFlags_PassthruCentralNode flag.
        dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
    // and handle the pass-thru hole, so the parent window should not have its own background:
    if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
        window_flags |= ImGuiWindowFlags_NoBackground;

    // If the padding option is disabled, set the parent window's padding size to 0 to effectively hide said padding.
    if (!opt_padding)
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::Begin("DockSpace Demo", &p_open, window_flags);

    // Remove the padding configuration - we pushed it, now we pop it:
    if (!opt_padding)
        ImGui::PopStyleVar();

    // Pop the two style rules set in Fullscreen mode - the corner rounding and the border size.
    if (opt_fullscreen)
        ImGui::PopStyleVar(2);

    // Check if Docking is enabled:
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        // If it is, draw the Dockspace with the DockSpace() function.
        // The GetID() function is to give a unique identifier to the Dockspace - here, it's "MyDockSpace".
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    // This is to show the menu bar that will change the config settings at runtime.
    // If you copied this demo function into your own code and removed ImGuiWindowFlags_MenuBar at the top of the function,
    // you should remove the below if-statement as well.
    //if (ImGui::BeginMenuBar())
    //{
    //    if (ImGui::BeginMenu("Options"))
    //    {
    //        // Disabling fullscreen would allow the window to be moved to the front of other windows,
    //        // which we can't undo at the moment without finer window depth/z control.
    //        ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
    //        ImGui::MenuItem("Padding", NULL, &opt_padding);
    //        ImGui::Separator();

    //        // Display a menu item for each Dockspace flag, clicking on one will toggle its assigned flag.
    //        if (ImGui::MenuItem("Flag: NoSplit",                "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 { dockspace_flags ^= ImGuiDockNodeFlags_NoSplit; }
    //        if (ImGui::MenuItem("Flag: NoResize",               "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                { dockspace_flags ^= ImGuiDockNodeFlags_NoResize; }
    //        if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  { dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode; }
    //        if (ImGui::MenuItem("Flag: AutoHideTabBar",         "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          { dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar; }
    //        if (ImGui::MenuItem("Flag: PassthruCentralNode",    "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0, opt_fullscreen)) { dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode; }
    //        ImGui::Separator();

    //        // Display a menu item to close this example.
    //        if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
    //            if (p_open != NULL) // Remove MSVC warning C6011 (NULL dereference) - the `p_open != NULL` in MenuItem() does prevent NULL derefs, but IntelliSense doesn't analyze that deep so we need to add this in ourselves.
    //                p_open = false; // Changing this variable to false will close the parent window, therefore closing the Dockspace as well.
    //        ImGui::EndMenu();
    //    }

    //    ImGui::EndMenuBar();
    //}

    // End the parent window that contains the Dockspace:
    ImGui::End();
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowViewport() {
    if (ImGui::Begin("Viewport")) {
        auto constexpr w {800};
        auto constexpr h {400};

        ImGui::Image(
            D3D11Core::GetFinalSRV(),
            ImVec2{w, h}
        );
    }
    ImGui::End();
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowRSEditWindow() {
    auto& select {D3D11Core::RasterizerState()};
    if (ImGui::Begin("Rasterizer State")) {
        ImGui::RadioButton("Solid", &select, 0);
        ImGui::RadioButton("Wireframe", &select, 1);
    }
    ImGui::End();
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowCubemapEditWindow() {
    auto& index {D3D11Cubemap::TextureIndex()};

    if (ImGui::Begin("Cubemap Edit Window")) {
        ImGui::RadioButton("Santa Maria (No IBL)", &index, 0);
        ImGui::RadioButton("Fort Point (No IBL)", &index, 1);
        ImGui::RadioButton("Atrium", &index, 2);
        ImGui::RadioButton("Garage", &index, 3);
        ImGui::RadioButton("Day Environment (HDR)", &index, 4);
    }
    ImGui::End();
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowSceneEditWindow(x_vector<std::shared_ptr<D3D11RenderObject>>& objs) {
    if (ImGui::Begin("Scene Edit Window")) {
        ImGui::Columns(4, nullptr, true);
        for (auto& obj : objs)
            ImGuiRenderSceneTree(*obj->GetScene(), 0);

        ImGui::NextColumn();
        if (_selected.first != nullptr) {
            ImGuiShowSceneInfoWindow();

            ImGui::NextColumn();
            ImGuiShowNodeEditWindow();

            ImGui::NextColumn();
            ImGuiShowRenderConfigureWindow();
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
        scene->MarkAsTransformed(node);
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowRenderConfigureWindow() {
    auto& [scene, node] {_selected};
    static char const* methods[] {"Solid", "Phong", "PBR"};
    static int selected_method {};
    ImGui::Text("Rendering Configuration");
    ImGui::Text("Target Material ID: %d", scene->_nodeId_to_materialId.contains(node) ? scene->_nodeId_to_materialId.at(node) : -1);
    ImGui::Combo("Shading", &selected_method, methods, static_cast<int>(std::size(methods)));
    _renderConfigureTable[selected_method]();
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowMaterialEditWindow() {
    auto& [scene, node] {_selected};
    ImGui::Text("Material Constants Configuration");
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowPostProcessEditWindow() {
    //ImGui::Text("Post Process");
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

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowSceneInfoWindow() {
    auto& [scene, node] {_selected};
    ImGui::Text("Number of Nodes: %d", scene->_tree.size());
    ImGui::Text("Number of Meshes: %d", scene->_mesh.size());
    ImGui::Text("Number of Materials: %d", scene->_material.size());
    if (ImGui::Button("Add Material"))
        scene->AddMaterial(D3D11Core::Device());

    ImGui::SameLine();
    std::optional<std::pair<int32_t, x_string>> submitted_file_info {};
    if (ImGui::Button("Add Texture") || _assetBrowser.Opened()) {
        _assetBrowser.SetOpened();
        submitted_file_info = _assetBrowser.Browse();
    }

    x_vector<x_string> materials {};
    for (auto i {0}; i != scene->_material.size(); ++i) {
        materials.push_back("Material");
        materials.back() += std::to_string(i);
    }
    char const* preview_value {materials[_selectedMaterial].c_str()};
    if (ImGui::BeginCombo("Materials", preview_value))
    {
        for (int n = 0; n != materials.size(); n++)
        {
            const bool is_selected = (_selectedMaterial == n);
            if (ImGui::Selectable(materials[n].c_str(), is_selected))
                _selectedMaterial = n;

            // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    if (_selectedMaterial >= 0 && _selectedMaterial < scene->_material.size()) {
        ImGuiShowTextureInfoWindow();

        if (submitted_file_info.has_value() && submitted_file_info.value().first != -1 && submitted_file_info.value().second.ends_with(".dds")) {
            std::cout << submitted_file_info.value().first << ' ' << submitted_file_info.value().second << std::endl;
            scene->_material[_selectedMaterial].AddOrRelplaceTexture(
                D3D11Core::Device(), 
                static_cast<ShaderResourceTypes>(submitted_file_info.value().first + static_cast<int>(ShaderResourceTypes::EmissiveMap)), 
                submitted_file_info.value().second.c_str());
        }
    }
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowTextureInfoWindow() {
    auto& [scene, node] {_selected};
    auto const& material {scene->_material[_selectedMaterial]};
    auto const description {material.GetTextureInfo()};

    ImGui::Text("Texture Information");
    ImGui::Text("%s", description.c_str());
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowSolidConfigureWindow() {
    auto& [scene, node] {_selected};
    auto& render_strategy {scene->GetRenderStrategyAt(node)};

    int temp {};
    bool use_diffusemap {(render_strategy & (0b1 << static_cast<int>(ShaderResourceTypes::DiffuseMap))) != false};
    bool use_heightmap {(render_strategy & (0b1 << static_cast<int>(ShaderResourceTypes::HeightMap))) != false};

    ImGui::Checkbox("Diffuse Map", &use_diffusemap);
    ImGui::Checkbox("Height Map", &use_heightmap);

    ImGui::Text("Environment Mapping");
    ImGui::RadioButton("None", &temp, 0);
    ImGui::RadioButton("Basic Environment Mapping", &temp, 1);
    ImGui::RadioButton("Basic IBL", &temp, 2);

    auto const closest_node_that_has_material {scene->GetClosestMaterialConstant(node)};
    ImGuiShowVSConstantEditWindow(closest_node_that_has_material);

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

    int temp {};
    ImGui::Text("Environment Mapping");
    ImGui::RadioButton("None", &temp, 0);
    ImGui::RadioButton("Basic Environment Mapping", &temp, 1);
    ImGui::RadioButton("Basic IBL", &temp, 2);

    auto const closest_node_that_has_material {scene->GetClosestMaterialConstant(node)};
    ImGuiShowPhongConstantEditWindow(closest_node_that_has_material);
    ImGuiShowVSConstantEditWindow(closest_node_that_has_material);

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

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowPhongConstantEditWindow(int32_t node) {
    int interacted {};
    if (node != -1) {
        ImGui::Text("Phong Constant Editor");
        auto& params {_selected.first->GetMaterialAt(_selected.first->_nodeId_to_materialId[node]).GetPhongConstants()->GetParams()};
        interacted += ImGui::SliderFloat("Ambient Strength", &params.ambient_constant, 0.0f, 2.0f, "%.3f");
        interacted += ImGui::SliderFloat("Diffuse Strength", &params.diffuse_constant, 0.0f, 2.0f, "%.3f");
        interacted += ImGui::SliderFloat("Specular Strength", &params.specular_constant, 0.0f, 2.0f, "%.3f");
        interacted += ImGui::InputFloat("Shininess", &params.shininess, 0.0f, 10000.0f, "%.3f");
    }
    if (interacted > 0) {
        _selected.first->_recentlyUpdatedPhongMaterial = node;
        _selected.first->MarkAsPhongMaterialEdited(_selected.second);
    }
    else {
        _selected.first->_recentlyUpdatedPhongMaterial = -1;
    }
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowSystemInfo() {
    if (ImGui::Begin("System Info")) {
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    }
    ImGui::End();
}

void Engine::Graphics::D3D11ImGuiRenderer::ImGuiShowVSConstantEditWindow(int32_t node) {
    int interacted {};
    if (node != -1) {
        ImGui::Text("VS Constant Editor");
        auto& params {_selected.first->GetMaterialAt(_selected.first->_nodeId_to_materialId[node]).GetVSConstants()->GetParams()};
        interacted += ImGui::SliderFloat("Height Map Scale", &params.height_scale, 0.0f, 2.0f, "%.5f");
    }

    if (interacted > 0) {
        _selected.first->_recentlyUpdatedVSConstants = node;
        _selected.first->MarkAsVSConstantEdited(_selected.second);
    }
    else {
        _selected.first->_recentlyUpdatedVSConstants = -1;
    }
}
