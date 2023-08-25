#pragma once

#include "D3D11ImGuiFileBrowser.h"

namespace Engine::Graphics {
    class D3D11RenderObject;
    class D3D11SceneGraph;

    class D3D11ImGuiRenderer {
    public:
        static void Init(ID3D11Device* device, ID3D11DeviceContext* context);
        static void Shutdown();

        static void BeginFrame();
        static void EndFrame();

        inline static bool imguiEnabled {false};

        static void     ImGuiShowDockSpace();
        static void     ImGuiShowViewport();

        static void     ImGuiShowRSEditWindow();
        static void     ImGuiShowCubemapEditWindow();

        static void     ImGuiShowSceneEditWindow(x_vector<std::shared_ptr<D3D11RenderObject>>& objs);
        static void     ImGuiShowNodeEditWindow();
        static void     ImGuiShowRenderConfigureWindow();
        static void     ImGuiShowMaterialEditWindow();
        static void     ImGuiShowPostProcessEditWindow();
        static int32_t  ImGuiRenderSceneTree(D3D11SceneGraph& scene, int32_t node);
    private:
        static void     ImGuiShowSceneInfoWindow();
        static void     ImGuiShowTextureInfoWindow();
        static void     ImGuiShowSolidConfigureWindow();
        static void     ImGuiShowPhongConfigureWindow();
        static void     ImGuiShowPBRConfigureWindow();
        static void     ImGuiShowEMConfigureWindow();
        static void     ImGuiShowBasicIBLConfigureWindow();

        static void     ImGuiShowSystemInfo();

        static void     ImGuiShowVSConstantEditWindow(int32_t node);
        static void     ImGuiShowPhongConstantEditWindow(int32_t node);

        inline static   x_vector<std::function<void(void)>> _renderConfigureTable {
           &ImGuiShowSolidConfigureWindow,
           &ImGuiShowPhongConfigureWindow,
           &ImGuiShowPBRConfigureWindow,
        };
    private:
        inline static std::pair<D3D11SceneGraph*, int32_t> _selected {};
        inline static int32_t                              _selectedMaterial {};
        inline static D3D11ImGuiFileBrowser                _assetBrowser {"C:\\Github\\Game\\Game\\Assets"};
    };
}

