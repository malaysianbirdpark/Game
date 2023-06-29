#include "gwpch.h"
#include "D3D11Topology.h"
#include "BindableCodex.h"

#include "Renderer/RenderDevice.h"

namespace Glowing::Bind {
    Topology::Topology(D3D11_PRIMITIVE_TOPOLOGY type)
        :type{type}
    {
    }

    void Topology::Bind() noexcept {
        GET_CONTEXT->IASetPrimitiveTopology(type);
    }

    std::shared_ptr<Topology> Topology::Resolve(D3D11_PRIMITIVE_TOPOLOGY type) {
        return Codex::Resolve<Topology>(type);
    }

    std::string Topology::GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type) {
        using namespace std::string_literals;
        return typeid(Topology).name() + "#"s + std::to_string(type);
    }

    std::string Topology::GetUID() const noexcept {
        return GenerateUID(type);
    }
}
