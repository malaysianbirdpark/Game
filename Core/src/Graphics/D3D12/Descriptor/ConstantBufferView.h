#pragma once

#include "DescriptorHeap.h"

namespace Engine::Graphics {
class GraphicsContext;

class ConstantBufferView : public DescriptorHeap {
public:
    ConstantBufferView(GraphicsContext& gfx, UINT num_of_view);
    virtual ~ConstantBufferView() override = default;
};
}

