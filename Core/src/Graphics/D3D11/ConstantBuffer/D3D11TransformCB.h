#pragma once

namespace Engine::Graphics {
    // MV + Transform
    class D3D11TransformCB {
        struct Transform {
            DirectX::XMFLOAT4X4 _m;
            DirectX::XMFLOAT4X4 _mit;
            DirectX::XMFLOAT4X4 _mvp;
        };
    public:
        D3D11TransformCB(
            ID3D11Device& device,
            ID3D11DeviceContext& context,
            DirectX::CXMMATRIX model
        );

        void Accumulate(DirectX::CXMMATRIX transform);

        DirectX::XMMATRIX GetModel() const;

        void SetModel(DirectX::CXMMATRIX model);

        void Update(ID3D11DeviceContext& context);

        void Bind(ID3D11DeviceContext& context) const;
    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> _resource;
        DirectX::XMFLOAT4X4 _model;
        Transform _transform;
    };
}

