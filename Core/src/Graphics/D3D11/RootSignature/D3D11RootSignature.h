#pragma once

namespace Engine::Graphics {
    class D3D11PipelineStateObject;
    class D3D11ConstantBuffer;
    class D3D11Sampler;
    class D3D11Texture;

    class D3D11RootSignature {
    public:
        D3D11RootSignature(std::shared_ptr<D3D11PipelineStateObject> const& target_pso, uint16_t priority)
            : _targetPSO{target_pso}, _priority{priority} {}
        ~D3D11RootSignature() = default;

        void AddConstantBuffer(ID3D11Device& device, UINT slot, void* data, size_t data_size, bool vsVisible, bool psVisible, bool dynamic, char const* tag);
        void AddSampler(ID3D11Device& device, UINT slot, char const* tag);
        void AddTexture(ID3D11Device& device, UINT slot, char const* path);

        std::shared_ptr<D3D11ConstantBuffer>& GetConstantBufferAt(UINT slot) { return _cb[slot]; };
        void UpdateMVP(ID3D11DeviceContext& context, void const* data, size_t data_size);

        void Bind(ID3D11DeviceContext& context);
    private:
        USE_LOCK;

        std::shared_ptr<D3D11PipelineStateObject>                        _targetPSO {};
        uint16_t                                                         _priority {};

        // TEMP: Always use slot 0 for MVP!!!!!!!!
        x_unordered_map<UINT, std::shared_ptr<D3D11ConstantBuffer>>  _cb;
        x_unordered_map<UINT, std::shared_ptr<D3D11Sampler>>         _sampler;
        x_unordered_map<UINT, std::shared_ptr<D3D11Texture>>         _texture;
    };
}

