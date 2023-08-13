#include "pch.h"
#include "D3D11RenderObject.h"

#include "ConstantBuffer/D3D11TransformMVP.h"
#include "SceneGraph/D3D11SceneGraph.h"

Engine::Graphics::D3D11RenderObject::D3D11RenderObject(ID3D11Device& device, DirectX::XMMATRIX const& view, DirectX::XMMATRIX const& proj, std::shared_ptr<D3D11SceneGraph>&& scene)
    : _scene{std::move(scene)}
{
    _transform = MakeUnique<D3D11TransformMVP>(
        device, 
        DirectX::XMMatrixIdentity(),
        view,
        proj
    );
}

void Engine::Graphics::D3D11RenderObject::Render(ID3D11DeviceContext& context, DirectX::FXMMATRIX const& view, DirectX::FXMMATRIX const& proj) {
    _transform->SetView(view);
    _transform->SetProj(proj);

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
            //std::cout << _scene->_nodeNames[_scene->_nodeId_to_namesId[node]] << std::endl;

            _transform->SetModel(DirectX::XMLoadFloat4x4(&global_transforms[node_to_mesh.at(node)]));
            _transform->Update(context);

            _transform->Bind(context);

            auto const material_idx {node_to_material.contains(node) ? node_to_material.at(node) : 0};
            _scene->_mesh[node_to_mesh.at(node)].Render(context, &_scene->_material[material_idx]);
        }

        if (tree[node]._firstChild != -1)
            stack.push(tree[node]._firstChild);
        for (auto s {tree[node]._nextSibling}; s != -1; s = tree[s]._nextSibling)
            stack.push(s);
    }

}
