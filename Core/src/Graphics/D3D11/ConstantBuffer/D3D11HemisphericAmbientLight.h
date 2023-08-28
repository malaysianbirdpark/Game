#pragma once

namespace Engine::Graphics {
    class D3D11HemisphericAmbientLight {
        struct LightInfo {
            DirectX::XMFLOAT3 _ambientDownColor {1.0f, 1.0f, 1.0f};
            float             _padding0;
            DirectX::XMFLOAT3 _ambientUpColor;
            float             _padding1;
        };
    public:
        D3D11HemisphericAmbientLight(ID3D11Device& device);

        void Update(float const dt);
        void Upload(ID3D11DeviceContext& context);
        void Bind(ID3D11DeviceContext& context);
    private:
        LightInfo                             _info {};
        Microsoft::WRL::ComPtr<ID3D11Buffer>  _resource;
        inline static uint32_t                _interacted {};
    };

}

