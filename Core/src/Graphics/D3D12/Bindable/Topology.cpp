#include "pch.h"
#include "Topology.h"

Engine::Graphics::Topology::Topology(GraphicsContext& gfx, D3D12_PRIMITIVE_TOPOLOGY type)
    : _type{type}
{
}

void Engine::Graphics::Topology::Bind(GraphicsContext& gfx) noexcept {
}

std::shared_ptr<Engine::Graphics::Topology> Engine::Graphics::Topology::Resolve(GraphicsContext& gfx, D3D12_PRIMITIVE_TOPOLOGY type) {
    return nullptr;
}

std::string Engine::Graphics::Topology::GenerateUID(D3D12_PRIMITIVE_TOPOLOGY type) {
    using namespace std::string_literals;
    return typeid(Topology).name() + "#"s + std::to_string(type);
}

std::string Engine::Graphics::Topology::GetUID() const noexcept {
    return GenerateUID(_type);
}
