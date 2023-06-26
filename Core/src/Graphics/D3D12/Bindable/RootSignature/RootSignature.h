#pragma once

#include "../Bindable.h"

namespace Engine::Graphics {
class RootSignature : public Bindable {
public:
    RootSignature(GraphicsContext& gfx);
    RootSignature(
        GraphicsContext& gfx, 
        std::function<D3D12_ROOT_SIGNATURE_DESC()> const& get_desc, 
        UINT slot, 
        UINT num
    );
    virtual ~RootSignature() override = default;
    
    Microsoft::WRL::ComPtr<ID3D12RootSignature> GetSignature() const { return _signature; }

    virtual void Bind(GraphicsContext& gfx) noexcept override;
    [[nodiscard]] virtual std::string GetUID() const noexcept override;
protected:
    UINT                                        _slot;
    UINT                                        _num;
    Microsoft::WRL::ComPtr<ID3D12RootSignature> _signature;
};
}

