#pragma once

namespace Engine::Graphics {
    class D3D11UnrealPBRConstants {
        friend class D3D11Material;

        struct UnrealPBRParameters {
            DirectX::XMFLOAT3 albedo_color;
            float  metallicFactor;

            float  roughness;
            float  ambient_strength;
            bool   use_emissive_map {false};
            bool   pad0[3];
            bool   use_diffuse_map {false};
            bool   pad1[3];

            bool   use_normal_map {false};
            bool   pad3[3];
            bool   use_height_map {false};
            bool   pad4[3];
            bool   use_metallic_map {false};
            bool   pad5[3];
            bool   use_roughness_map {false};
            bool   pad6[3];

            bool   use_ambient_occlusion {false};
            bool   pad7[3];
            float  pad8[3];
        };
    public:
        D3D11UnrealPBRConstants(ID3D11Device& device);

        UnrealPBRParameters& GetParams();

        void Upload(ID3D11DeviceContext& context);
        void Bind(ID3D11DeviceContext& context);
    private:
        UnrealPBRParameters                   _params {};
        Microsoft::WRL::ComPtr<ID3D11Buffer>  _resource;
    };
}

