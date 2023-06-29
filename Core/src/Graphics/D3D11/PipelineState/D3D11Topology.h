#pragma once

#include "D3D11Bindable.h"

namespace Glowing::Bind {
    class GLOWING_API Topology final : public Bindable {
    public:
        Topology(D3D11_PRIMITIVE_TOPOLOGY type);

        void Bind() noexcept override;

        [[nodiscard]] static std::shared_ptr<Topology> Resolve(D3D11_PRIMITIVE_TOPOLOGY type);
        [[nodiscard]] static std::string GenerateUID(D3D11_PRIMITIVE_TOPOLOGY type);
        [[nodiscard]] std::string GetUID() const noexcept override;
    private:
        D3D11_PRIMITIVE_TOPOLOGY type;
    };
}

