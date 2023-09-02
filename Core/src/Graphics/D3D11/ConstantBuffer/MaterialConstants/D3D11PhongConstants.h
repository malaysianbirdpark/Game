#pragma once

namespace Engine::Graphics {
    class D3D11PhongConstants {
        friend class D3D11Material;

        struct PhongParameters {
            // material constants
            float ambient_constant {0.01f};
            float diffuse_constant {0.5f};
            float specular_constant {0.8f};
            float shininess {100.0f};

            // texture options
            bool use_diffuse_map {false};
            bool pad0[3];
            bool use_specular_map {false};
            bool pad1[3];
            bool use_normal_map {false};
            bool pad2[3];
            bool use_height_map {false};
            bool pad3[3];
        };
    public:
        D3D11PhongConstants(ID3D11Device& device);

        PhongParameters& GetParams();

        void Upload(ID3D11DeviceContext& context);
        void Bind(ID3D11DeviceContext& context);
    private:
        PhongParameters                       _params {};
        Microsoft::WRL::ComPtr<ID3D11Buffer>  _resource;
    };
}

