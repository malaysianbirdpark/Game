#pragma once

namespace Engine::Graphics {
    class D3D11PipelineStateObject;
    class D3D11RootSignature;

    // TODO: TEMP
    class Triangle;

    class D3D11Core {
    public:
        D3D11Core(int width, int height, HWND native_wnd, bool windowed);
        ~D3D11Core();

        void Render();
    private:
        void BeginFrame();
        void EndFrame();
    private:
        WindowInfo _windowInfo;
    private:
        Microsoft::WRL::ComPtr<ID3D11Device>             _device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>      _immContext;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>      _defContext0;

        Microsoft::WRL::ComPtr<IDXGISwapChain>           _swapChain;
        Microsoft::WRL::ComPtr<ID3D11Resource>           _backBuffers;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   _backBufferView;

        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>   _depthStencilView;

        D3D11_VIEWPORT                                   _viewPort {};

        x_unordered_map<x_string, std::unique_ptr<D3D11PipelineStateObject>> _pso;
        x_unordered_map<x_string, std::unique_ptr<D3D11RootSignature>>       _rs;
    };
}

