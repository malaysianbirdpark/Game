#pragma once

namespace Engine::Graphics {
class D3D12ConstantBuffer {
public:
    D3D12ConstantBuffer();
    ~D3D12ConstantBuffer();

    void Transition(ID3D12GraphicsCommandList& cmd_list, D3D12_RESOURCE_STATES to);
private:    
    D3D12_RESOURCE_STATES                    _state;
    Microsoft::WRL::ComPtr<ID3D12Resource>   _resource;
    size_t                                   _size {};
    size_t                                   _elemSize {};
    size_t                                   _curIdx {};
};

class D3D12Texture2D {
};

class D3D12Texture3D {
};

class D3D12Buffer {
};

};

