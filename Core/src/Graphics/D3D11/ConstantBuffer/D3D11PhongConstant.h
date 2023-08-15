#pragma once

namespace Engine::Graphics {
    class D3D11PhongConstant {
        struct data {
            DirectX::XMFLOAT3 _ambient;
            DirectX::XMFLOAT3 _diffuse;
            DirectX::XMFLOAT3 _specular;
        };
    };
}

