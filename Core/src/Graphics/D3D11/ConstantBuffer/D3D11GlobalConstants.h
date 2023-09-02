#pragma once

namespace Engine::Graphics {
    enum class LightType {
        Amb_Dir = 0,
        Point = 1,
    };

    class D3D11GlobalConstants {
        struct Constants {
            DirectX::XMFLOAT4   _camPos {};
            int32_t             _light_type {};
            int32_t pad0[3];
        };
    public:
        D3D11GlobalConstants(ID3D11Device& device);

        void Reflect(DirectX::XMVECTOR plane);

        void SetLightType(LightType type);
        void Update(float const dt);
        void Upload(ID3D11DeviceContext& context);

        void Bind(ID3D11DeviceContext& context);
    private:
        Constants                            _data;
        Microsoft::WRL::ComPtr<ID3D11Buffer> _resource;
    };
}

