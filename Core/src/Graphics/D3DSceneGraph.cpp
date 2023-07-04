#include "pch.h"
#include "D3DSceneGraph.h"

Engine::Graphics::D3DMesh::D3DMesh(x_string const& pso_tag, x_string const& rs_tag)
    : _psoTag{pso_tag}, _rsTag{rs_tag}
{
}

void Engine::Graphics::D3DMesh::Draw(ID3D11DeviceContext& context, DirectX::FXMMATRIX acc_transform) {
    DirectX::XMStoreFloat4x4(&_transform, acc_transform);

    // Bind
    context.DrawIndexed(, 0u, 0u);
}

Engine::Graphics::D3DSceneNode::D3DSceneNode(x_string const& file_name, DirectX::XMMATRIX const& parent_transform) {
    DirectX::XMStoreFloat4x4(&_parentTransform, parent_transform);
    DirectX::XMStoreFloat4x4(&_currentTransform, DirectX::XMMatrixIdentity());
}

void Engine::Graphics::D3DSceneNode::Draw(ID3D11DeviceContext& context, DirectX::FXMMATRIX acc_transform) {
    auto const transform {
        DirectX::XMLoadFloat4x4(&_currentTransform) *
        DirectX::XMLoadFloat4x4(&_parentTransform)  *
        acc_transform
    };

    _mesh->Draw(context, transform);
    for (auto& ch : _child)
        ch->Draw(context, transform);
}

Engine::Graphics::D3DSceneGraph::D3DSceneGraph(x_string const& root_model_file_name) {
}

void Engine::Graphics::D3DSceneGraph::Update(float const dt) {
}

void Engine::Graphics::D3DSceneGraph::Draw() {
}

void Engine::Graphics::D3DSceneGraph::AddNode(x_string const& file_name) {
    _root->AddNode(file_name);
}
