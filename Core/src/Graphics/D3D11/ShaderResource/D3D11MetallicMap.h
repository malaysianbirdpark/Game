#pragma once

namespace Engine::Graphics {
    class D3D11MetallicMap {
    public:
        D3D11MetallicMap(ID3D11Device& device, ID3D11DeviceContext& context, char const* path);

        static std::shared_ptr<D3D11MetallicMap> Create(ID3D11Device& device, ID3D11DeviceContext& context, char const* path);

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
