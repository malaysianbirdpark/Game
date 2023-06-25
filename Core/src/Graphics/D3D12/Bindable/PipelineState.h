#pragma once

#include "Bindable.h"

namespace Engine::Graphics {
class PipelineState : public Bindable {
public:
    PipelineState(
        GraphicsContext& gfx, 
        std::unique_ptr<class RootSignature>& p_root_signature,
        std::unique_ptr<class VertexShader>& p_vertex_shader,
        std::unique_ptr<class PixelShader>& p_pixel_shader);
    virtual ~PipelineState() override = default;

    void Bind(GraphicsContext& gfx) noexcept override;
    [[nodiscard]] std::string GetUID() const noexcept override;
private:
    Microsoft::WRL::ComPtr<ID3D12PipelineState>  _pipelineState;
    D3D12_GRAPHICS_PIPELINE_STATE_DESC           _desc {};
};
}

