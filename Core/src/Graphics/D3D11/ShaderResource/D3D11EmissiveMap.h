#pragma once

namespace Engine::Graphics {
    class D3D11EmissiveMap {
    public:
        D3D11EmissiveMap(ID3D11Device& device, char const* path);

        static D3D11EmissiveMap CreateEmissiveMap(ID3D11Device& device, char const* path);

        void Bind(ID3D11DeviceContext& context) const;

        [[nodiscard]] static x_string GenUID(char const* path);
        [[nodiscard]] x_string GetUID() const;
    protected:
        x_string _path;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srv;
    };
}


