#pragma once

namespace Engine::Graphics {
    class D3D11PipelineStateObject;
    class D3D11ConstantBuffer;
    class D3D11Sampler;
    class D3D11Texture;

    class D3D11RootSignature {
        enum ShaderResourceType {
            DIFFUSE,
            AMBIENT,
            SPECULAR,
            NORMAL_MAP,
        };

        struct MVP {
            DirectX::XMFLOAT4X4 model;
            DirectX::XMFLOAT4X4 view;
            DirectX::XMFLOAT4X4 projection;
        };
    public:
        D3D11RootSignature(
            ID3D11Device& device, 
            DirectX::XMMATRIX const& proj,
            std::shared_ptr<D3D11PipelineStateObject> const& target_pso, 
            uint16_t priority
        );
        ~D3D11RootSignature() = default;

        void AddConstantBuffer(ID3D11Device& device, UINT slot, void* data, size_t data_size, bool vsVisible, bool psVisible, bool dynamic, char const* tag);
        void AddSamplerSlot(ID3D11Device& device, UINT slot, char const* tag);
        void AddShaderResourceSlot(ID3D11Device& device, UINT slot, char const* path);

        std::shared_ptr<D3D11ConstantBuffer>& GetConstantBufferAt(UINT slot) { return _cb[slot]; };

        void UpdateModel(DirectX::XMMATRIX const& model);
        void UpdateView(DirectX::XMMATRIX const& view);
        void UpdateProj(DirectX::XMMATRIX const& proj);
        void Update(ID3D11DeviceContext& context);

        void Bind(ID3D11DeviceContext& context);
    private:
        USE_LOCK;

        std::shared_ptr<D3D11PipelineStateObject>      _targetPSO {};
        uint16_t                                       _priority {};

        // Always use slot 0 for MVP!!!!!!!!
        x_unordered_map<UINT, std::shared_ptr<D3D11ConstantBuffer>>  _cb;
        x_unordered_map<UINT, std::shared_ptr<D3D11Sampler>>         _sampler;
        x_unordered_map<UINT, std::shared_ptr<D3D11Texture>>         _texture;
    private:
        MVP          _mvp {};
    };
}

