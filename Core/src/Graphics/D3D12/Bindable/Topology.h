#pragma once

#include "Bindable.h"

namespace Engine::Graphics {
    class Topology : public Bindable {
    public:
        Topology(GraphicsContext& gfx, D3D12_PRIMITIVE_TOPOLOGY type);
        virtual ~Topology() override = default;

        void Bind(GraphicsContext& gfx) noexcept override;

        [[nodiscard]] static std::shared_ptr<Topology> Resolve(GraphicsContext& gfx, D3D12_PRIMITIVE_TOPOLOGY type);
        [[nodiscard]] static std::string GenerateUID(D3D12_PRIMITIVE_TOPOLOGY type);
        [[nodiscard]] std::string GetUID() const noexcept override;
    private:
        D3D12_PRIMITIVE_TOPOLOGY _type;
    };
}

