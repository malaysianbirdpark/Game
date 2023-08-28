#include "pch.h"
#include "D3D11ConcreteLight.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"
#include "Graphics/D3D11/SceneGraph/D3D11SceneHolder.h"

#include <variant>

Engine::Graphics::D3D11ConcreteLight::D3D11ConcreteLight(ID3D11Device& device, DirectX::XMMATRIX const& view, DirectX::XMMATRIX const& proj, ConcreteLightType type)
    : D3D11RenderObject{device, view, proj, std::move(D3D11SceneHolder::ResolveScene("Sphere"))}
{
    switch (type) {
    case ConcreteLightType::PointLight:
        _lightCB = MakeUnique<D3D11ConstantBuffer>(std::move(D3D11PointLight{device}));
        break;
    default: 
        CORE_ASSERT(false, "Invalid Concrete Light Type!");
    }

    D3D11RenderObject::SetModelScale(0.1f, 0.1f, 0.1f);
}

void Engine::Graphics::D3D11ConcreteLight::UpdatePos(DirectX::XMFLOAT3&& pos) {
    std::visit(UpdateLightPos{std::forward<DirectX::XMFLOAT3>(pos)}, *_lightCB.get());
}

void Engine::Graphics::D3D11ConcreteLight::Render(ID3D11DeviceContext& context) {
    D3D11RenderObject::DesignatedRender(context, RenderStrategyType::Solid);
}

Engine::Graphics::D3D11ConstantBuffer& Engine::Graphics::D3D11ConcreteLight::GetCB() {
    return *_lightCB.get();
}
