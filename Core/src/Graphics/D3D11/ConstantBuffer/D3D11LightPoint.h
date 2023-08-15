#pragma once

namespace Engine::Graphics {
    class D3D11LightPoint {
        struct LightInfo {
            DirectX::XMFLOAT3 _pos;
            DirectX::XMFLOAT3 _ambientColor;
            DirectX::XMFLOAT3 _diffuseColor;
        };
    };
}

