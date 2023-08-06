#pragma once

namespace Engine::Graphics {
    class D3D11LightDirectional {
        struct LightInfo {
            DirectX::XMFLOAT3 _dir;
            DirectX::XMFLOAT4 _color;
            float             _att;
        };
    public:
        D3D11LightDirectional(ID3D11Device& device, DirectX::XMFLOAT3 const& dir, DirectX::XMFLOAT4 const& color, float att);

        void UpdateDir(ID3D11DeviceContext& context, DirectX::XMFLOAT3 const& dir);
        void UpdateColor(ID3D11DeviceContext& context, DirectX::XMFLOAT4 const& color);
        void UpdateAttenuation(ID3D11DeviceContext& context, float const att);
        void Update(ID3D11DeviceContext& context);

        void Bind(ID3D11DeviceContext& context) const;
    private:
        LightInfo _info {};
        Microsoft::WRL::ComPtr<ID3D11Buffer> _resource;
    };
}

