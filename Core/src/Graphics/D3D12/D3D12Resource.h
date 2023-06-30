#pragma once

namespace Engine::Graphics {
class D3D12Descriptor;

class D3D12ConstantBuffer {
public:
    D3D12ConstantBuffer(ID3D12Device* device, size_t element_size);
    ~D3D12ConstantBuffer();

    void Push_back(void* data);

    void Transition(ID3D12GraphicsCommandList& cmd_list, D3D12_RESOURCE_STATES to);
private:    
    D3D12_RESOURCE_STATES                    _state;
    Microsoft::WRL::ComPtr<ID3D12Resource>   _resource;
    //std::unique_ptr<D3D12Descriptor>         _descriptor;
    size_t                                   _capacity {};
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

