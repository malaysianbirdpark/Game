#pragma once

namespace Engine::Graphics {
    class D3D11PipelineStateObject;
    class D3D11RootSignature;
    class D3D11DefaultObject;

    class D3D11Core {
    public:
        D3D11Core(int width, int height, HWND native_wnd, bool windowed);
        ~D3D11Core();

        void Update(float const dt);

        void BeginFrame();
        void EndFrame();

        void AddScene();

        [[nodiscard]] static DirectX::XMMATRIX GetProj();

        static ID3D11Device& Device() { return *_device.Get(); }
        static ID3D11DeviceContext& Context() { return *_immContext.Get(); }
        //static int32_t&      RasterizerState() { return _selectedRS; }
    private:
        void InitData();
    private:
        WindowInfo                        _windowInfo;
        inline static DirectX::XMFLOAT4X4 _proj;
    private:
        inline static Microsoft::WRL::ComPtr<ID3D11Device>         _device;
        inline static Microsoft::WRL::ComPtr<ID3D11DeviceContext>  _immContext;

        Microsoft::WRL::ComPtr<IDXGISwapChain>                 _swapChain;
    private:
        std::unique_ptr<struct D3D11RenderData>                _data;
    };
}

