#pragma once

namespace Engine::Graphics {
    enum class DefContext {
            Default,
            ConcreteLight,
            Others,
            Mirror,
            PostProcess,
            GUI
        };

    class D3D11RenderCommand {
    public:
        enum {
            NUM_DEF_CONTEXTS = 6,
            IMGUI_CONTEXT = NUM_DEF_CONTEXTS - 1
        };

        static void Init(ID3D11Device& device, IDXGISwapChain& swap_chain, int width, int height);

        static void BeginForward(struct D3D11RenderData* data);
        static void EndForward(x_array<ID3D11CommandList*, NUM_DEF_CONTEXTS>& cmd_lists);

        static void* GetFinalSRV();
        static int32_t& RasterizerState();

        static ID3D11DeviceContext& GetContext(DefContext context);
    private:
        static void SetContextsStates();
        static void InitRasterizerState(ID3D11Device& device);
        static void InitDepthStencil(ID3D11Device& device, int width, int height, UINT quality_level);
        static void InitSamplers(ID3D11Device& device);
        static void InitBlendState(ID3D11Device& device);
    private:
        inline static x_array<Microsoft::WRL::ComPtr<ID3D11DeviceContext>,   NUM_DEF_CONTEXTS>
                                                                        _defContexts;
    private:
        inline static Microsoft::WRL::ComPtr<ID3D11Resource>            _backBuffers;
        inline static Microsoft::WRL::ComPtr<ID3D11RenderTargetView>    _backBufferView;

        inline static Microsoft::WRL::ComPtr<ID3D11Texture2D>           _renderBuffer;
        inline static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  _renderSRV;
        inline static Microsoft::WRL::ComPtr<ID3D11RenderTargetView>    _renderRTV;

        inline static Microsoft::WRL::ComPtr<ID3D11Texture2D>           _intermediateBuffer;
        inline static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  _intermediateSRV;
        inline static Microsoft::WRL::ComPtr<ID3D11RenderTargetView>    _intermediateRTV;
        inline static int32_t                                           _idx {0};

        inline static Microsoft::WRL::ComPtr<ID3D11Texture2D>           _imguiBuffer;
        inline static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  _imguiSRV;
        inline static Microsoft::WRL::ComPtr<ID3D11RenderTargetView>    _imguiRTV;

        inline static Microsoft::WRL::ComPtr<ID3D11Texture2D>           _resolvedBuffer;
        inline static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  _resolvedSRV;
        inline static Microsoft::WRL::ComPtr<ID3D11RenderTargetView>    _resolvedRTV;

        inline static Microsoft::WRL::ComPtr<ID3D11Texture2D>           _finalBuffer;
        inline static Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>  _finalSRV;
        inline static Microsoft::WRL::ComPtr<ID3D11RenderTargetView>    _finalRTV;
    private:
        inline static int32_t                                                 _selectedRS {};
        inline static x_vector<Microsoft::WRL::ComPtr<ID3D11RasterizerState>> _rasterizerState;
        inline static Microsoft::WRL::ComPtr<ID3D11DepthStencilView>          _depthStencilView;

        inline static Microsoft::WRL::ComPtr<ID3D11DepthStencilState>         _dsDefault;
        inline static Microsoft::WRL::ComPtr<ID3D11DepthStencilState>         _dsWriteMask;
        inline static Microsoft::WRL::ComPtr<ID3D11DepthStencilState>         _dsReadMask;

        inline static Microsoft::WRL::ComPtr<ID3D11BlendState>                _bsDefault;
        inline static Microsoft::WRL::ComPtr<ID3D11BlendState>                _bsMirror;
    private:
        inline static D3D11_VIEWPORT                                          _viewPort {};

        inline static Microsoft::WRL::ComPtr<ID3D11SamplerState>              _samplerWrap;
        inline static Microsoft::WRL::ComPtr<ID3D11SamplerState>              _samplerClamp;
    };
}

