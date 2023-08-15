#pragma once

namespace Engine::Graphics {
    class D3DCamera {
    public:
        static void Init();

        static DirectX::XMVECTOR GetPos();
        static DirectX::XMMATRIX GetView();

        static void Update();
        static void Reset();

        static void Rotate(float const dx, float const dy);
        static void Translate(DirectX::XMFLOAT3 mat);
    private:
        inline static DirectX::XMFLOAT3 _pos;
        inline static float _pitch {0.0f};
        inline static float _yaw {0.0f};
        inline static float _travelSpeed {12.0f};
        inline static float _rotationSpeed {0.4f};
    };
}

