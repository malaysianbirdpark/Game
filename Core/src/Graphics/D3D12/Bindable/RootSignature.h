#pragma once

#include "Bindable.h"

namespace Engine::Graphics {
class RootSignature : public Bindable {
public:
    RootSignature(GraphicsContext& gfx);
    
    Microsoft::WRL::ComPtr<ID3D12RootSignature> GetSignature() const { return _signature; }

    void Bind(GraphicsContext& gfx) noexcept override;
    [[nodiscard]] std::string GetUID() const noexcept override;
private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> _signature;
};
}

