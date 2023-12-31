#pragma once

namespace Engine::Graphics {
    class D3D11BRDFLut {
    public:
        D3D11BRDFLut(ID3D11Device& device, char const* path);

        static std::shared_ptr<D3D11BRDFLut> Create(ID3D11Device& device, char const* path);

        void Bind(ID3D11DeviceContext& context) const;

        [[nodiscard]] x_string GetDescription() const;
        [[nodiscard]] int32_t  GetTypeID() const;

        [[nodiscard]] static x_string GenUID(char const* path);
        [[nodiscard]] x_string GetUID() const;
    protected:
        x_string _path;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srv;
    };
}

