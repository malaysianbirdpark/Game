#pragma once

#include "ConstantBuffer/D3D11TransformMVP.h"

namespace Engine::Graphics {
    class D3D11TransformMVP;
    class D3D11SceneGraph;

    class D3D11RenderObject {
    public:
        D3D11RenderObject(
            ID3D11Device& device, 
            DirectX::XMMATRIX const& view,
            DirectX::XMMATRIX const& proj,
            std::shared_ptr<D3D11SceneGraph>&& scene
        );

        void Render(ID3D11DeviceContext& context, DirectX::FXMMATRIX const& view, DirectX::FXMMATRIX const& proj);
    private:
        std::unique_ptr<D3D11TransformMVP>   _transform;
        std::shared_ptr<D3D11SceneGraph>     _scene;
    };
}

