#pragma once

namespace Engine::Graphics {
    class D3D11PipelineStateObject;
    class D3D11RootSignature;
    class D3D11RenderObject;

    class D3D11Core {
    public:
        D3D11Core(int width, int height, HWND native_wnd, bool windowed);
        ~D3D11Core();

        void Update(float const dt, DirectX::XMMATRIX const& view);
        void Render();

        void AddScene();

        [[nodiscard]] DirectX::XMMATRIX GetProj();

        ID3D11Device& Device() { return *_device.Get(); }
    private:
        void BeginFrame();
        void EndFrame();
    private:
        WindowInfo          _windowInfo;
        DirectX::XMFLOAT4X4 _proj;
    private:
        Microsoft::WRL::ComPtr<ID3D11Device>             _device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>      _immContext;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>      _defContext0;

        Microsoft::WRL::ComPtr<IDXGISwapChain>           _swapChain;
        Microsoft::WRL::ComPtr<ID3D11Resource>           _backBuffers;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   _backBufferView;

        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>   _depthStencilView;

        D3D11_VIEWPORT                                   _viewPort {};

        x_vector<std::unique_ptr<D3D11RenderObject>>     _obj; 

        std::unique_ptr<class D3D11Sampler>              _sampler;
    };
}

