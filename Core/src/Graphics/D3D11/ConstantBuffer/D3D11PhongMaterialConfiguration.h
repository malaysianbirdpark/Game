#pragma once

namespace Engine::Graphics {
    class D3D11PhongMaterialConfiguration {
        struct MaterialParameters {
            bool use_texture;
        };
    public:
        D3D11PhongMaterialConfiguration(ID3D11Device& device, DirectX::XMFLOAT3&& dir, DirectX::XMFLOAT3&& color);

        void Update();

        void Upload(ID3D11DeviceContext& context);

        void Bind(ID3D11DeviceContext& context);
    private:
        MaterialParameters                             _info {};
        Microsoft::WRL::ComPtr<ID3D11Buffer>  _resource;
    };
}

