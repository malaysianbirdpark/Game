#pragma once

#include "Graphics/D3D11/ConstantBuffer/D3D11ConstantBuffer.h"
#include "Graphics/D3D11/ConstantBuffer/D3D11TransformCB.h"
#include "Graphics/D3D11/RenderStrategy/D3D11RenderStrategy.h"

namespace Engine::Graphics {
    class D3D11TransformCB;
    class D3D11SceneGraph;

    enum class ConcreteLightType {
        PointLight = 0,
    };

    class D3D11ConcreteLight {
    public:
        D3D11ConcreteLight(
            ID3D11Device& device, 
            ID3D11DeviceContext& context,
            ConcreteLightType type
        );
        virtual ~D3D11ConcreteLight() = default;

        D3D11SceneGraph* GetScene();

        void Update(float const dt);
        void UpdatePos(DirectX::XMFLOAT3&& pos);

        void Render(ID3D11DeviceContext& context);

        D3D11ConstantBuffer& GetCB();
    private:
        std::unique_ptr<D3D11ConstantBuffer> _lightCB;
        D3D11TransformCB                     _transform;
        std::unique_ptr<D3D11SceneGraph>     _scene;
        x_vector<int32_t>                    _stack;
        x_vector<bool>                       _visited;
    };
}