#pragma once

#include "Bindable.h"

namespace Engine::Graphics {
class GraphicsContext;
class CPUBuffer;
class Vertex;

class VertexBuffer : public Bindable {
public:
    VertexBuffer(GraphicsContext& gfx, x_vector<Vertex> const& vertices);

    uint32_t GetCount() const { return _count; }

    void Bind(GraphicsContext& gfx) noexcept override;
    [[nodiscard]] std::string GetUID() const noexcept override;
private:
    std::unique_ptr<CPUBuffer>   _buffer;
    D3D12_VERTEX_BUFFER_VIEW  _view {};
    uint32_t                  _count {};
};
}

