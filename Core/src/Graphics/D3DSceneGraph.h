#pragma once

namespace Engine::Graphics {
    class D3D11PipelineStateObject;
    class D3D11RootSignature;

    class D3DMesh {
    public:
        D3DMesh(x_string const& pso_tag, x_string const& rs_tag);

        void Draw(ID3D11DeviceContext& context, DirectX::FXMMATRIX acc_transform);

        DirectX::XMMATRIX GetModelMatrix() const { return DirectX::XMLoadFloat4x4(&_transform); }
    private:
        x_string _psoTag;
        x_string _rsTag;
        mutable DirectX::XMFLOAT4X4 _transform;
    };

    class D3DSceneNode {
    public:
        D3DSceneNode(x_string const& file_name, DirectX::XMMATRIX const& parent_transform);

        void Draw(ID3D11DeviceContext& context, DirectX::FXMMATRIX acc_transform);

        void AddNode(x_string const& file_name);
    private:
        std::shared_ptr<D3DMesh>                 _mesh;
        x_vector<std::shared_ptr<D3DSceneNode>>  _child;
        DirectX::XMFLOAT4X4                      _currentTransform;
        DirectX::XMFLOAT4X4                      _parentTransform;
    };

    class D3DSceneGraph {
    public:
        D3DSceneGraph(x_string const& root_model_file_name);

        void Update(float const dt);
        void Draw();

        void AddNode(x_string const& file_name);
    private:
        [[nodiscard]] std::shared_ptr<D3DMesh> ParseMeshFromFile(x_string const& file_name);
    private:
        std::shared_ptr<D3DSceneNode> _root;
    };
}

