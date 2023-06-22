#pragma once

#include "DescriptorHeap.h"

namespace Engine::Graphics {
class GraphicsContext;

class RenderTargetView : public DescriptorHeap {
public:
    RenderTargetView(GraphicsContext& gfx, UINT num_of_view);
    virtual ~RenderTargetView() = default;
};
}

