#pragma once

namespace Engine::Graphics {
    class D3D11RenderTarget {
    public:
        D3D11RenderTarget(ID3D11Device& device, D3D11_TEXTURE2D_DESC const& texture_desc);

        ID3D11ShaderResourceView* GetSRV();
        void SetRTV(ID3D11DeviceContext& context, ID3D11DepthStencilView* dsv = nullptr);
        void ClearRTV(ID3D11DeviceContext& context);
        void Copy(D3D11RenderTarget const& src);
    private:
        Microsoft::WRL::ComPtr<ID3D11Texture2D>            _buffer;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>   _srv;
        Microsoft::WRL::ComPtr<ID3D11RenderTargetView>     _rtv;
    };
}

