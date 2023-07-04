#pragma once

namespace Engine::Graphics {
    class D3DCamera {
    public:
        D3DCamera(DirectX::XMFLOAT3 init_pos = DirectX::XMFLOAT3());

        DirectX::XMMATRIX GetView() const;

        void Rotate(float const dx, float const dy);
        void Translate(DirectX::XMFLOAT3 mat);
    private:
        DirectX::XMFLOAT3 _pos;
        float _pitch {0.0f};
        float _yaw {0.0f};
        float _travelSpeed {12.0f};
        float _rotationSpeed {0.4f};
    };
}

