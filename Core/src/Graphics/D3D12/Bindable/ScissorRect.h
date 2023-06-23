#pragma once

#include "Bindable.h"

namespace Engine::Graphics {
    class ScissorRect : public Bindable {
    public:
        ScissorRect(long left, long top, long right, long bottom);
        virtual ~ScissorRect() override;
         
        void Bind(GraphicsContext& gfx) noexcept override;

        [[nodiscard]] static std::shared_ptr<ScissorRect> Resolve();
        [[nodiscard]] static std::string GenerateUID();
        [[nodiscard]] std::string GetUID() const noexcept override;
    private:
        CD3DX12_RECT _rect;
    };
}

