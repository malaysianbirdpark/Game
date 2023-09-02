#include "pch.h"
#include "D3D11MirrorObject.h"

#include "Graphics/D3D11/D3D11RenderCommand.h"
#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"

Engine::Graphics::D3D11MirrorObject::D3D11MirrorObject(ID3D11Device& device, ID3D11DeviceContext& context, D3D11SceneGraph&& scene)
    : _transform{device, context, DirectX::XMMatrixIdentity()}
{
    _scene = std::make_unique<D3D11SceneGraph>(scene);
    _stack.reserve(scene._tree.size());
    _visited.resize(scene._tree.size());
}

Engine::Graphics::D3D11SceneGraph* Engine::Graphics::D3D11MirrorObject::GetScene() {
    return _scene.get();
}

DirectX::XMVECTOR Engine::Graphics::D3D11MirrorObject::GetMirrorPlane() {
    auto const& global_transforms {_scene->_globalTransforms};

    return DirectX::XMPlaneTransform(
        DirectX::XMPlaneFromPointNormal(
            DirectX::XMVECTOR{0.0f, 0.0f, 0.0f, 1.0f},
            DirectX::XMVECTOR{0.0f, 0.0f, 1.0f, 0.0f}
        ),
        DirectX::XMLoadFloat4x4(&global_transforms[0])
    );
}

void Engine::Graphics::D3D11MirrorObject::Render(ID3D11DeviceContext& context) {
    using namespace DirectX;

    auto const& tree {_scene->_tree};
    auto const& node_to_mesh {_scene->_nodeId_to_meshId};
    auto const& node_to_material {_scene->_nodeId_to_materialId};
    auto const& global_transforms {_scene->_globalTransforms};

    _stack.clear();
    for (auto i {1}; i != _visited.size(); ++i)
        _visited[i] = false;

    _stack.push_back(0);
    _visited[0] = true;
    while (!_stack.empty()) {
        auto const node {_stack.back()};
        _stack.pop_back();

        if (node_to_mesh.contains(node)) {
            _transform.SetModel(XMLoadFloat4x4(&global_transforms[node]));
            _transform.Update(context);

            _transform.Bind(context);

            auto const material_idx {node_to_material.contains(node) ? node_to_material.at(node) : 0};
            renderStrategyTable[_scene->_renderStrategy[node]](context, _scene->_mesh[node_to_mesh.at(node)], _scene->_material[material_idx]);
        }

        if ((tree[node]._firstChild != -1) && !_visited[tree[node]._firstChild]) {
            _visited[tree[node]._firstChild] = true;
            _stack.push_back(tree[node]._firstChild);
        }
        for (auto s {tree[node]._nextSibling}; s != -1; s = tree[s]._nextSibling)
            if (!_visited[s]) {
                _visited[s] = true; 
                _stack.push_back(s);
            }
    }
}

void Engine::Graphics::D3D11MirrorObject::Update(float const dt) {
    _scene->Update();
}

