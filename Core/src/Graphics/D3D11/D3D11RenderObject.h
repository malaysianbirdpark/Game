#pragma once

namespace Engine::Graphics {
    class D3D11TransformMVP;
    class D3D11SceneGraph;

    class D3D11RenderObject {
    public:
        D3D11RenderObject(
            ID3D11Device& device, 
            DirectX::XMMATRIX const& view,
            DirectX::XMMATRIX const& proj,
            DirectX::XMMATRIX const& root_transform, 
            std::shared_ptr<D3D11SceneGraph>&& scene
        );

        void Update(ID3D11DeviceContext& context, DirectX::FXMMATRIX const& transform, float const dt);
        void Render(ID3D11DeviceContext& context);
    private:
        DirectX::XMFLOAT4X4                           _rootTransform {};
        x_vector<std::unique_ptr<D3D11TransformMVP>>  _transformOfMeshes;
        std::shared_ptr<D3D11SceneGraph>              _scene;
    };
}

