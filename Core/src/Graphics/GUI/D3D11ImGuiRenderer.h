#pragma once

#include "D3D11ImGuiFileBrowser.h"
#include "D3D11ImGuiSceneBrowser.h"

namespace Engine::Graphics {
    class D3D11DefaultObject;
    class D3D11ConcreteLight;
    class D3D11SceneGraph;

    class D3D11ImGuiRenderer {
    public:
        static void Init(ID3D11DeviceContext* context);
        static void Shutdown();

        static void BeginFrame();
        static void EndFrame();

        inline static bool imguiEnabled {false};

        static void     ImGuiShowDockSpace();
        static void     ImGuiShowViewport();

        static void     ImGuiShowRSEditWindow();
        static void     ImGuiShowCubemapEditWindow();

        static void     ImGuiShowSceneEditWindow(x_vector<std::shared_ptr<D3D11DefaultObject>>& objs);
        static void     ImGuiShowNodeEditWindow();
        static void     ImGuiShowRenderConfigureWindow();
        static void     ImGuiShowMaterialEditWindow();
        static void     ImGuiShowPostProcessEditWindow();
        static int32_t  ImGuiRenderSceneTree(D3D11SceneGraph& scene, int32_t node);
        static void     ImGuiRenderLightList(D3D11SceneGraph& scene, int32_t id);
        static void     ImGuiRenderMirrorList(D3D11SceneGraph& scene, int32_t id);
        static void     ImGuiShowConcreteLightEditWindow(x_vector<std::shared_ptr<D3D11ConcreteLight>>& light_obj);
        static void     ImGuiShowMirrorEditWindow(x_vector<std::shared_ptr<class D3D11MirrorObject>>& mirror_obj);
    private:
        static void     ImGuiShowSceneInfoWindow();
        static void     ImGuiShowTextureInfoWindow();
        static void     ImGuiShowSolidConfigureWindow(int32_t& render_strategy);
        static void     ImGuiShowPhongConfigureWindow(int32_t& render_strategy);
        static void     ImGuiShowUnrealPBRConfigureWindow(int32_t& render_strategy);
        static void     ImGuiShowEMConfigureWindow();
        static void     ImGuiShowBasicIBLConfigureWindow();

        static void     ImGuiShowSystemInfo();

        static void     ImGuiShowVSConstantEditWindow(int32_t node);
        static void     ImGuiShowPhongConstantEditWindow(int32_t node);
        static void     ImGuiShowUnrealPBRConstantEditWindow(int32_t node);
        static void     ImGuiShowSolidConstantEditWindow(int32_t node);

        inline static   x_vector<std::function<void(int32_t&)>> _renderConfigureTable {
           &ImGuiShowSolidConfigureWindow,
           &ImGuiShowPhongConfigureWindow,
           &ImGuiShowUnrealPBRConfigureWindow,
        };
    private:
        inline static std::pair<D3D11SceneGraph*, int32_t> _selected {};
        inline static std::pair<D3D11SceneGraph*, int32_t> _selectedLight {};
        inline static int32_t                              _selectedMaterial {};
        inline static D3D11ImGuiFileBrowser                _assetBrowser {"C:\\Github\\Game\\Game\\Assets"};
        inline static D3D11ImGuiSceneBrowser               _sceneBrowser {};
    };
}

