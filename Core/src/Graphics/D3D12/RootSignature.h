#pragma once

namespace Engine::Graphics {
class GraphicsContext;
    
class RootSignature {
public:
    RootSignature(GraphicsContext& gfx);
    
    Microsoft::WRL::ComPtr<ID3D12RootSignature> GetSignature() const { return _signature; }
private:
    Microsoft::WRL::ComPtr<ID3D12RootSignature> _signature;
};
}

