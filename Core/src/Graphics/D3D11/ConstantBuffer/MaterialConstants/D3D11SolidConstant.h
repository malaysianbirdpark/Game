#pragma once

namespace Engine::Graphics {
    class D3D11SolidConstants {
        friend class D3D11Material;

        struct SolidParameters {
            // texture options
            bool use_diffuse_map {false};
            bool pad0[3];
            bool use_height_map {false};
            bool pad1[3];
            float pad2[2];
        };
    public:
        D3D11SolidConstants(ID3D11Device& device);

        SolidParameters& GetParams();

        void Upload(ID3D11DeviceContext& context);
        void Bind(ID3D11DeviceContext& context);
    private:
        SolidParameters                       _params {};
        Microsoft::WRL::ComPtr<ID3D11Buffer>  _resource;
    };
}

