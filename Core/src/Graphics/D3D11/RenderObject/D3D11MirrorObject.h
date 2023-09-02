#pragma once

#include "Graphics/D3D11/ConstantBuffer/D3D11TransformCB.h"
#include "Graphics/D3D11/RenderStrategy/D3D11RenderStrategy.h"

namespace Engine::Graphics {
    class D3D11TransformCB;
    class D3D11SceneGraph;

    class D3D11MirrorObject {
    public:
        D3D11MirrorObject(
            ID3D11Device& device, 
            ID3D11DeviceContext& context,
            D3D11SceneGraph&& scene
        );

        D3D11SceneGraph* GetScene();

        DirectX::XMVECTOR GetMirrorPlane();

        void Render(ID3D11DeviceContext& context);
        void Update(float const dt);
        void UpdatePos(DirectX::XMFLOAT3&& pos);
    private:
        D3D11TransformCB                    _transform;
        std::unique_ptr<D3D11SceneGraph>    _scene;
        x_vector<int32_t>                   _stack;
        x_vector<bool>                      _visited;
    };
}

