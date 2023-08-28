#pragma once

namespace Engine::Graphics {
    class D3D11UnrealPBRConstants {
        friend class D3D11Material;

        struct UnrealPBRParameters {
            DirectX::XMFLOAT3 albedo_color;
            float  roughness;
            float  metallicFactor;
            bool   use_emissive_map {false};
            bool   pad0[3];
            bool   use_diffuse_map {false};
            bool   pad1[3];
            bool   use_specular_map {false};
            bool   pad2[3];
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
            bool   use_brdf_lut {false};
            bool   pad8[3];
            float  pad9[2];
        };
    public:
        D3D11UnrealPBRConstants(ID3D11Device& device);

        UnrealPBRParameters& GetParams();

        void Upload(ID3D11DeviceContext& context);
        void Bind(ID3D11DeviceContext& context);
    private:
        UnrealPBRParameters                    _params {};
        Microsoft::WRL::ComPtr<ID3D11Buffer>  _resource;
    };
}

