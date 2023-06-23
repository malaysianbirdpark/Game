#pragma once

#include "Bindable.h"

namespace Engine::Graphics {
class PipelineState : public Bindable {
public:
    PipelineState(GraphicsContext& gfx);
    virtual ~PipelineState() override = default;

    void Bind(GraphicsContext& gfx) noexcept override;
    [[nodiscard]] std::string GetUID() const noexcept override;
private:
    Microsoft::WRL::ComPtr<ID3D12PipelineState> _pipelineState;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC          _desc {};
};
}

