#pragma once
#include "D3D11ImGuiRenderer.h"

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

        static void     ImGuiShowCubemapEditWindow();

        static void     ImGuiShowSceneEditWindow(x_vector<std::shared_ptr<D3D11RenderObject>>& objs);
        static void     ImGuiShowNodeEditWindow();
        static void     ImGuiShowRenderConfigureWindow();
        static void     ImGuiShowMaterialEditWindow();
        static int32_t  ImGuiRenderSceneTree(D3D11SceneGraph& scene, int32_t node);
    private:
        static void     ImGuiShowSolidConfigureWindow();
        static void     ImGuiShowPhongConfigureWindow();
        static void     ImGuiShowEMConfigureWindow();
        static void     ImGuiShowBasicIBLConfigureWindow();

        inline static   x_vector<std::function<void(void)>> _renderConfigureTable {
           &ImGuiShowSolidConfigureWindow,
           &ImGuiShowPhongConfigureWindow,
           &ImGuiShowEMConfigureWindow,
           &ImGuiShowBasicIBLConfigureWindow
        };
    private:
        static void ShowMenu();
        inline static std::pair<D3D11SceneGraph*, int32_t> _selected {};
    };
}

