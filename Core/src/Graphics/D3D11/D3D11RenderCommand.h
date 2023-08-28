#pragma once

namespace Engine::Graphics {
    class D3D11RenderCommand {
        enum {
            NUM_DEF_CONTEXTS = 3,
            IMGUI_CONTEXT = NUM_DEF_CONTEXTS - 1
        };
    public:
        static void Init(ID3D11Device& device, ID3D11DeviceContext& imm_context, IDXGISwapChain& swap_chain, int width, int height);

        static void BeginForward(struct D3D11RenderData* data);
        static void EndForward(x_array<ID3D11CommandList*, NUM_DEF_CONTEXTS>& cmd_lists);

        static void* GetFinalSRV();
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
    private:
        inline static D3D11_VIEWPORT                                          _viewPort {};

        inline static Microsoft::WRL::ComPtr<ID3D11SamplerState>              _samplerWrap;
        inline static Microsoft::WRL::ComPtr<ID3D11SamplerState>              _samplerClamp;
    };
}

