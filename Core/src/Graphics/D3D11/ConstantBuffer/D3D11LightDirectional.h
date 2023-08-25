#pragma once

namespace Engine::Graphics {
    class D3D11LightDirectional {
        struct LightInfo {
            DirectX::XMFLOAT3 _diffuseColor;
            float             _padding0;
            DirectX::XMFLOAT3 _dir;
            float             _padding1;
        };
    public:
        D3D11LightDirectional(ID3D11Device& device, DirectX::XMFLOAT3&& dir, DirectX::XMFLOAT3&& color);

        void Update();
        void Upload(ID3D11DeviceContext& context);
        void Bind(ID3D11DeviceContext& context);
    private:
        LightInfo                             _info {};
        Microsoft::WRL::ComPtr<ID3D11Buffer>  _resource;
    };
}

