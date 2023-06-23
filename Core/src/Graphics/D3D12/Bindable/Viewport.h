#pragma once

#include "Bindable.h"

namespace Engine::Graphics {
    class Viewport : public Bindable {
    public:
        Viewport(float width, float height, float top_left_x = 0.0f, float top_left_y = 0.0f, float min_depth = 0.0f, float max_depth = 1.0f);
        virtual ~Viewport() override;

        void Bind(GraphicsContext& gfx) noexcept override;
        [[nodiscard]] std::string GetUID() const noexcept override;
    private:
        D3D12_VIEWPORT _vp;
    };
}

