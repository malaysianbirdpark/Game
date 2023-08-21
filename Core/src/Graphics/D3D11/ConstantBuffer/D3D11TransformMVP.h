#pragma once

namespace Engine::Graphics {
    // MV + MVP
    class D3D11TransformMVP {
        struct MVP {
            DirectX::XMFLOAT4X4 _m;
            DirectX::XMFLOAT4X4 _mit;
            DirectX::XMFLOAT4X4 _mvp;     
        };
    public:
        D3D11TransformMVP(
            ID3D11Device& device,
            DirectX::XMMATRIX const& model,
            DirectX::XMMATRIX const& view, 
            DirectX::XMMATRIX const& proj
        );

        void Accumulate(DirectX::XMMATRIX const& transform);

        DirectX::XMMATRIX GetModel() const;
        DirectX::XMMATRIX GetView() const;
        DirectX::XMMATRIX GetProj() const;

        void SetModel(DirectX::XMMATRIX const& model);
        void SetView(DirectX::XMMATRIX const& view);
        void SetProj(DirectX::XMMATRIX const& proj);

        void Update(ID3D11DeviceContext& context);

        void Bind(ID3D11DeviceContext& context) const;
    private:
        Microsoft::WRL::ComPtr<ID3D11Buffer> _resource;
        DirectX::XMFLOAT4X4 _model;
        DirectX::XMFLOAT4X4 _view;
        DirectX::XMFLOAT4X4 _proj;
        MVP _mvp;
    };
}

