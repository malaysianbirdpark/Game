#pragma once

#include "D3D11RenderObject.h"
#include "Graphics/D3D11/ConstantBuffer/D3D11ConstantBuffer.h"

namespace Engine::Graphics {
    enum class ConcreteLightType {
        PointLight = 0,
    };

    class D3D11ConcreteLight : public D3D11RenderObject {
    public:
        D3D11ConcreteLight(
            ID3D11Device& device, 
            DirectX::XMMATRIX const& view,
            DirectX::XMMATRIX const& proj,
            ConcreteLightType type
        );
        virtual ~D3D11ConcreteLight() = default;

        void UpdatePos(DirectX::XMFLOAT3&& pos);
        void Render(ID3D11DeviceContext& context) override;

        D3D11ConstantBuffer& GetCB();
    private:
        std::unique_ptr<D3D11ConstantBuffer> _lightCB;
    };
}

