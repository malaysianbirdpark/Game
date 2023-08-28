#pragma once

namespace Engine::Graphics {
    class D3D11VertexShaderConstants {
        friend class D3D11Material;

        struct MaterialParameters {
            float height_scale {0.5f};
            float padding[3] {};
        };
    public:
        D3D11VertexShaderConstants(ID3D11Device& device);

        MaterialParameters& GetParams();

        void Upload(ID3D11DeviceContext& context);
        void Bind(ID3D11DeviceContext& context);
    private:
        MaterialParameters                    _params {};
        Microsoft::WRL::ComPtr<ID3D11Buffer>  _resource;
    };
}
