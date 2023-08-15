#pragma once

namespace Engine::Graphics {
    class D3D11CamPosition {
    public:
        D3D11CamPosition(ID3D11Device& device, DirectX::XMVECTOR const& pos);

        void Update();
        void Upload(ID3D11DeviceContext& context);

        void Bind(ID3D11DeviceContext& context);
    private:
        DirectX::XMFLOAT4                    _pos;
        Microsoft::WRL::ComPtr<ID3D11Buffer> _resource;
    };
}

