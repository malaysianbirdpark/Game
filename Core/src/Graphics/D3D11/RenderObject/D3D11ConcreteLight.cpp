#include "pch.h"
#include "D3D11ConcreteLight.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"
#include "Graphics/D3D11/SceneGraph/D3D11SceneHolder.h"

#include <variant>

#include "Graphics/D3D11/D3D11RenderCommand.h"

Engine::Graphics::D3D11ConcreteLight::D3D11ConcreteLight(ID3D11Device& device, ID3D11DeviceContext& context, ConcreteLightType type)
    : _transform{device, context, DirectX::XMMatrixIdentity()}
{
    switch (type) {
    case ConcreteLightType::PointLight:
        _lightCB = MakeUnique<D3D11ConstantBuffer>(std::move(D3D11PointLight{device}));
        _scene = std::make_unique<D3D11SceneGraph>(D3D11SceneHolder::ResolveScene("Sphere"));
        _stack.reserve(_scene->_tree.size());
        _visited.resize(_scene->_tree.size());
        break;
    default: 
        CORE_ASSERT(false, "Invalid Concrete Light Type!");
    }
}

Engine::Graphics::D3D11SceneGraph* Engine::Graphics::D3D11ConcreteLight::GetScene() {
    return _scene.get();
}

void Engine::Graphics::D3D11ConcreteLight::Update(float const dt) {
    _scene->Update();
}

void Engine::Graphics::D3D11ConcreteLight::UpdatePos(DirectX::XMFLOAT3&& pos) {
    std::visit(UpdateObjectPos{std::forward<DirectX::XMFLOAT3>(pos)}, *_lightCB);
}

void Engine::Graphics::D3D11ConcreteLight::Render(ID3D11DeviceContext& context) {
    using namespace DirectX;

    auto const& tree {_scene->_tree};
    auto const& node_to_mesh {_scene->_nodeId_to_meshId};
    auto const& node_to_material {_scene->_nodeId_to_materialId};
    auto const& global_transforms {_scene->_globalTransforms};

    _stack.clear();
    for (auto i {1}; i != _visited.size(); ++i)
        _visited[i] = false;

    _stack.push_back(0);
    while (!_stack.empty()) {
        auto const node {_stack.back()};
        _stack.pop_back();

        if (node_to_mesh.contains(node)) {
            _transform.SetModel(XMLoadFloat4x4(&global_transforms[node]));
            _transform.Update(context);

            _transform.Bind(context);

            auto const material_idx {node_to_material.contains(node) ? node_to_material.at(node) : 0};
            renderStrategyTable[0](context, _scene->_mesh[node_to_mesh.at(node)], _scene->_material[material_idx]);
        }

        if (tree[node]._firstChild != -1)
            _stack.push_back(tree[node]._firstChild);
        for (auto s {tree[node]._nextSibling}; s != -1; s = tree[s]._nextSibling)
            _stack.push_back(s);
    }
}

Engine::Graphics::D3D11ConstantBuffer& Engine::Graphics::D3D11ConcreteLight::GetCB() {
    return *_lightCB.get();
}
