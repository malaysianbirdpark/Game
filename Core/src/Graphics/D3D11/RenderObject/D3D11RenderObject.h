#pragma once

#include "Graphics/D3D11/ConstantBuffer/D3D11TransformMVP.h"
#include "Graphics/D3D11/RenderStrategy/D3D11RenderStrategy.h"

namespace Engine::Graphics {
    class D3D11TransformMVP;
    class D3D11SceneGraph;

    class D3D11RenderObject {
    public:
        D3D11RenderObject(
            ID3D11Device& device, 
            DirectX::XMMATRIX const& view,
            DirectX::XMMATRIX const& proj,
            D3D11SceneGraph&& scene
        );

        D3D11SceneGraph* GetScene();

        void SetModelScale(float const x, float const y, float const z);

        virtual void Render(ID3D11DeviceContext& context);
        void Update(float const dt, DirectX::FXMMATRIX const& view, DirectX::FXMMATRIX const& proj);
    protected:
        void DesignatedRender(ID3D11DeviceContext& context, RenderStrategyType strategy);
    private:
        DirectX::XMFLOAT4X4                  _scale;
        D3D11TransformMVP                    _transform;
        std::unique_ptr<D3D11SceneGraph>     _scene;
    };
}

