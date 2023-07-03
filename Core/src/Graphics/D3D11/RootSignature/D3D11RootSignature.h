#pragma once

namespace Engine::Graphics {
    class D3D11ConstantBuffer;
    class D3D11Sampler;
    class D3D11Texture;

    class D3D11RootSignature {
    public:
        D3D11RootSignature() = default;
        ~D3D11RootSignature() = default;

        void AddConstantBuffer(ID3D11Device& device, UINT slot, void* data, size_t data_size, bool vsVisible, bool psVisible, bool dynamic, char const* tag);
        void AddSampler(ID3D11Device& device, UINT slot, char const* tag);
        void AddTexture(ID3D11Device& device, UINT slot, char const* path);

        std::shared_ptr<D3D11ConstantBuffer>& GetConstantBufferAt(size_t index) { return _cb[index].second; };

        void Bind(ID3D11DeviceContext& context);
    private:
        x_vector<std::pair<UINT, std::shared_ptr<D3D11ConstantBuffer>>>  _cb;
        x_vector<std::pair<UINT, std::shared_ptr<D3D11Sampler>>>         _sampler;
        x_vector<std::pair<UINT, std::shared_ptr<D3D11Texture>>>         _texture;
    };
}

