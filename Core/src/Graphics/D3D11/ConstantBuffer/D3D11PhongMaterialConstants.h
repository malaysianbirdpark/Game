#pragma once

namespace Engine::Graphics {
    class D3D11PhongMaterialConstants {
        friend class D3D11Material;

        struct MaterialParameters {
            float ambient_constant {0.01f};
            float diffuse_constant {0.5f};
            float specular_constant {0.8f};
            float shininess {100.0f};
        };
    public:
        D3D11PhongMaterialConstants(ID3D11Device& device);

        MaterialParameters& GetParams();

        void Upload(ID3D11DeviceContext& context);
        void Bind(ID3D11DeviceContext& context);
    private:
        MaterialParameters                    _params {};
        Microsoft::WRL::ComPtr<ID3D11Buffer>  _resource;
    };
}

