#pragma once

namespace Engine::Graphics {
    class D3D11DirectionalLight {
        struct LightInfo {
            DirectX::XMFLOAT3 _diffuseColor;
            float             _padding0;
            DirectX::XMFLOAT3 _dir;
            float             _padding1;
        };
    public:
        D3D11DirectionalLight(ID3D11Device& device, DirectX::XMFLOAT3&& dir, DirectX::XMFLOAT3&& color);

        void Update(float const dt);
        void Upload(ID3D11DeviceContext& context);
        void Bind(ID3D11DeviceContext& context);
    private:
        LightInfo                             _info {};
        Microsoft::WRL::ComPtr<ID3D11Buffer>  _resource;
        inline static uint32_t                _interacted;
    };
}

