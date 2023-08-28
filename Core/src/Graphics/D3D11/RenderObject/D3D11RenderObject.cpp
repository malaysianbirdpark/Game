#include "pch.h"
#include "D3D11RenderObject.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"

Engine::Graphics::D3D11RenderObject::D3D11RenderObject(ID3D11Device& device, DirectX::XMMATRIX const& view, DirectX::XMMATRIX const& proj, D3D11SceneGraph&& scene)
    : _transform{device, DirectX::XMMatrixIdentity(), view, proj}
{
    _scene = std::make_unique<D3D11SceneGraph>(scene);
    DirectX::XMStoreFloat4x4(&_scale, DirectX::XMMatrixIdentity());
}

Engine::Graphics::D3D11SceneGraph* Engine::Graphics::D3D11RenderObject::GetScene() {
    return _scene.get();
}

void Engine::Graphics::D3D11RenderObject::SetModelScale(float const x, float const y, float const z) {
    DirectX::XMStoreFloat4x4(&_scale, DirectX::XMMatrixScaling(x, y, z));
}

void Engine::Graphics::D3D11RenderObject::Render(ID3D11DeviceContext& context) {
    using namespace DirectX;

    auto const& tree {_scene->_tree};
    auto const& node_to_mesh {_scene->_nodeId_to_meshId};
    auto const& node_to_material {_scene->_nodeId_to_materialId};
    auto const& global_transforms {_scene->_globalTransforms};

    std::stack<int32_t> stack {};
    stack.push(0);
    while (!stack.empty()) {
        auto const node {stack.top()};
        stack.pop();

        if (node_to_mesh.contains(node)) {
            _transform.SetModel(XMMatrixMultiply(XMLoadFloat4x4(&global_transforms[node]), XMLoadFloat4x4(&_scale)));
            _transform.Update(context);

            _transform.Bind(context);

            auto const material_idx {node_to_material.contains(node) ? node_to_material.at(node) : 0};
            renderStrategyTable[_scene->_renderStrategy[node]](context, _scene->_mesh[node_to_mesh.at(node)], _scene->_material[material_idx]);
        }

        if (tree[node]._firstChild != -1)
            stack.push(tree[node]._firstChild);
        for (auto s {tree[node]._nextSibling}; s != -1; s = tree[s]._nextSibling)
            stack.push(s);
    }
}

void Engine::Graphics::D3D11RenderObject::DesignatedRender(ID3D11DeviceContext& context, RenderStrategyType strategy) {
    using namespace DirectX;

    auto const& tree {_scene->_tree};
    auto const& node_to_mesh {_scene->_nodeId_to_meshId};
    auto const& node_to_material {_scene->_nodeId_to_materialId};
    auto const& global_transforms {_scene->_globalTransforms};

    std::stack<int32_t> stack {};
    stack.push(0);
    while (!stack.empty()) {
        auto const node {stack.top()};
        stack.pop();

        if (node_to_mesh.contains(node)) {
            _transform.SetModel(XMMatrixMultiply(XMLoadFloat4x4(&global_transforms[node]), XMLoadFloat4x4(&_scale)));
            _transform.Update(context);

            _transform.Bind(context);

            auto const material_idx {node_to_material.contains(node) ? node_to_material.at(node) : 0};
            renderStrategyTable[static_cast<int32_t>(strategy)](context, _scene->_mesh[node_to_mesh.at(node)], _scene->_material[material_idx]);
        }

        if (tree[node]._firstChild != -1)
            stack.push(tree[node]._firstChild);
        for (auto s {tree[node]._nextSibling}; s != -1; s = tree[s]._nextSibling)
            stack.push(s);
    }
}

void Engine::Graphics::D3D11RenderObject::Update(float const dt, DirectX::FXMMATRIX& view, DirectX::FXMMATRIX& proj) {
    _transform.SetView(view);
    _transform.SetProj(proj);

    _scene->Update();
}
