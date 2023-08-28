#pragma once

namespace Engine::Graphics {
    class D3D11PointLight {
        friend class D3D11ImGuiRenderer;

        struct LightInfo {
            DirectX::XMFLOAT3 _pos {};
            float pad0;
            DirectX::XMFLOAT3 _color {};
            float pad1;
            float             _reciprocalRange {1.0f};
            float pad2[3];
        };
    public:
        D3D11PointLight(ID3D11Device& device);

        void Update(float const dt);
        void SetPosition(DirectX::XMFLOAT3 pos);
        void Upload(ID3D11DeviceContext& context);
        void Bind(ID3D11DeviceContext& context);
    private:
        LightInfo                                 _info {};
        float                                     _range {1.0f};
        Microsoft::WRL::ComPtr<ID3D11Buffer>      _resource;
        inline static uint32_t                    _interacted {};
    };
}

