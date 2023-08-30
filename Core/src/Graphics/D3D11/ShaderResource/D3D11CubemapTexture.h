#pragma once

#include <variant>

namespace Engine::Graphics {
    class D3D11SDRCubemapTexture {
    public:
        D3D11SDRCubemapTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* env_path, char const* specular_path = nullptr, char const* diffuse_path = nullptr);

        void Bind(ID3D11DeviceContext& context) const;
    protected:
        x_string _pathEnv;
        x_string _pathSpecular;
        x_string _pathDiffuse;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srvEnv;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srvSpecular;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srvDiffuse;
    };

    class D3D11HDRCubemapTexture {
    public:
        D3D11HDRCubemapTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* env_path, char const* specular_path, char const* diffuse_path);
        D3D11HDRCubemapTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* env_path, char const* specular_path, char const* diffuse_path, char const* brdf_path);

        void Bind(ID3D11DeviceContext& context) const;
    protected:
        x_string _pathEnv;
        x_string _pathSpecular;
        x_string _pathDiffuse;
        x_string _pathBRDF;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srvEnv;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srvSpecular;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srvDiffuse;
        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> _srvBRDF;
    };

    using D3D11CubemapTexture = std::variant<D3D11SDRCubemapTexture, D3D11HDRCubemapTexture>;

    struct BindCubemapTexture {
    public:
        BindCubemapTexture(ID3D11DeviceContext& context) : _context{context} {}
        void operator() (D3D11SDRCubemapTexture const& target) const { target.Bind(_context); }
        void operator() (D3D11HDRCubemapTexture const& target) const { target.Bind(_context); }
    private:
        ID3D11DeviceContext& _context;
    };
}

