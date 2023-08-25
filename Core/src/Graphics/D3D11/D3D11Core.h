#pragma once

#include "Graphics/D3D11/ConstantBuffer/D3D11ConstantBuffer.h"

namespace Engine::Graphics {
    class D3D11PipelineStateObject;
    class D3D11RootSignature;
    class D3D11RenderObject;

    class D3D11Core {
        enum {
            NUM_DEF_CONTEXTS = 2,
            IMGUI_CONTEXT = NUM_DEF_CONTEXTS - 1
        };
    public:
        D3D11Core(int width, int height, HWND native_wnd, bool windowed);
        ~D3D11Core();

        void Update(float const dt, DirectX::XMMATRIX const& view);

        void BeginFrame();
        void EndFrame();

        void AddScene();

        static void* GetFinalSRV();

        [[nodiscard]] static DirectX::XMMATRIX GetProj();

        static ID3D11Device& Device() { return *_device.Get(); }
        static int32_t&      RasterizerState() { return _selectedRS; }
    private:
        WindowInfo                        _windowInfo;
        inline static DirectX::XMFLOAT4X4 _proj;
    private:
        inline static Microsoft::WRL::ComPtr<ID3D11Device>     _device;
        Microsoft::WRL::ComPtr<ID3D11DeviceContext>            _immContext;
        x_array<Microsoft::WRL::ComPtr<ID3D11DeviceContext>, NUM_DEF_CONTEXTS>
                                                               _defContexts;

        Microsoft::WRL::ComPtr<IDXGISwapChain>                         _swapChain;
        inline static Microsoft::WRL::ComPtr<ID3D11Resource>           _backBuffers;
        inline static Microsoft::WRL::ComPtr<ID3D11RenderTargetView>   _backBufferView;

        Microsoft::WRL::ComPtr<ID3D11Texture2D>                _renderBuffer;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>       _renderSRV;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>         _renderRTV;

        Microsoft::WRL::ComPtr<ID3D11Texture2D>                _imguiBuffer;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>       _imguiSRV;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>         _imguiRTV;

        Microsoft::WRL::ComPtr<ID3D11Texture2D>           _resolvedBuffer;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  _resolvedSRV;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>    _resolvedRTV;

        inline static Microsoft::WRL::ComPtr<ID3D11Texture2D>           _finalBuffer;
        inline static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  _finalSRV;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>                  _finalRTV;

        inline static int32_t                                   _selectedRS {};
        x_vector<Microsoft::WRL::ComPtr<ID3D11RasterizerState>> _rasterizerState;
        Microsoft::WRL::ComPtr<ID3D11DepthStencilView>          _depthStencilView;

        D3D11_VIEWPORT                                         _viewPort {};

        x_vector<std::shared_ptr<D3D11RenderObject>>           _obj; 

        std::unique_ptr<class D3D11Sampler>                    _sampler;

        x_vector<D3D11ConstantBuffer>                          _lights;
        x_vector<D3D11ConstantBuffer>                          _globalCB;

        std::unique_ptr<class D3D11Cubemap>                    _cubemap;
        std::unique_ptr<class D3D11FullScreenFilter>           _fullscreenFilter;
    };
}

