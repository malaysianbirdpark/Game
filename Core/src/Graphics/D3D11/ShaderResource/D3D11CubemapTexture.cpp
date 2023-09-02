#include "pch.h"
#include "D3D11CubemapTexture.h"

#include <directxtk/DDSTextureLoader.h>

Engine::Graphics::D3D11SDRCubemapTexture::D3D11SDRCubemapTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* env_path, char const* specular_path, char const* diffuse_path)
    : _pathEnv{env_path}, _srvEnv{nullptr}, _srvSpecular{nullptr}, _srvDiffuse{nullptr}
{
    using namespace DirectX;

    {
        std::wstring p (_pathEnv.length(), L' ');
        std::ranges::copy(_pathEnv, p.begin());

        auto alphaMode {DDS_ALPHA_MODE_UNKNOWN};

        CreateDDSTextureFromFileEx(
            &device,
            p.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
            0u,
            D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE,
            DDS_LOADER_DEFAULT,
            nullptr,
            _srvEnv.ReleaseAndGetAddressOf(),
            &alphaMode
        );
        context.GenerateMips(_srvEnv.Get());
    }

    if (specular_path) {
        _pathSpecular = {specular_path};

        std::wstring p (_pathSpecular.length(), L' ');
        std::ranges::copy(_pathSpecular, p.begin());

        auto alphaMode {DDS_ALPHA_MODE_UNKNOWN};

        CreateDDSTextureFromFileEx(
            &device,
            p.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
            0u,
            D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE,
            DDS_LOADER_DEFAULT,
            nullptr,
            _srvSpecular.ReleaseAndGetAddressOf(),
            &alphaMode
        );
        context.GenerateMips(_srvSpecular.Get());
    }

    if (diffuse_path) {
        _pathDiffuse = {diffuse_path};

        std::wstring p (_pathDiffuse.length(), L' ');
        std::ranges::copy(_pathDiffuse, p.begin());

        auto alphaMode {DDS_ALPHA_MODE_UNKNOWN};

        CreateDDSTextureFromFileEx(
            &device,
            p.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
            0u,
            D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE,
            DDS_LOADER_DEFAULT,
            nullptr,
            _srvDiffuse.ReleaseAndGetAddressOf(),
            &alphaMode
        );
        context.GenerateMips(_srvDiffuse.Get());
    }
}

void Engine::Graphics::D3D11SDRCubemapTexture::Bind(ID3D11DeviceContext& context) const {
    context.PSSetShaderResources(20u, 1u, _srvEnv.GetAddressOf());
    context.PSSetShaderResources(21u, 1u, _srvSpecular.GetAddressOf());
    context.PSSetShaderResources(22u, 1u, _srvDiffuse.GetAddressOf());
}

Engine::Graphics::D3D11HDRCubemapTexture::D3D11HDRCubemapTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* env_path, char const* specular_path, char const* diffuse_path)
    : _pathEnv{env_path}, _pathSpecular{specular_path}, _pathDiffuse{diffuse_path}
{
    using namespace DirectX;

    {
        std::wstring p (_pathEnv.length(), L' ');
        std::ranges::copy(_pathEnv, p.begin());

        auto alphaMode {DDS_ALPHA_MODE_UNKNOWN};

        CreateDDSTextureFromFileEx(
            &device,
            p.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
            0u,
            D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE,
            DDS_LOADER_DEFAULT,
            nullptr,
            _srvEnv.ReleaseAndGetAddressOf(),
            &alphaMode
        );
    }

    {
        std::wstring p (_pathSpecular.length(), L' ');
        std::ranges::copy(_pathSpecular, p.begin());

        auto alphaMode {DDS_ALPHA_MODE_UNKNOWN};

        CreateDDSTextureFromFileEx(
            &device,
            p.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
            0u,
            D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE,
            DDS_LOADER_DEFAULT,
            nullptr,
            _srvSpecular.ReleaseAndGetAddressOf(),
            &alphaMode
        );
    }

    {
        std::wstring p (_pathDiffuse.length(), L' ');
        std::ranges::copy(_pathDiffuse, p.begin());

        auto alphaMode {DDS_ALPHA_MODE_UNKNOWN};

        CreateDDSTextureFromFileEx(
            &device,
            p.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
            0u,
            D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE,
            DDS_LOADER_DEFAULT,
            nullptr,
            _srvDiffuse.ReleaseAndGetAddressOf(),
            &alphaMode
        );
    }

    context.GenerateMips(_srvEnv.Get());
    context.GenerateMips(_srvSpecular.Get());
    context.GenerateMips(_srvDiffuse.Get());
}

Engine::Graphics::D3D11HDRCubemapTexture::D3D11HDRCubemapTexture(ID3D11Device& device, ID3D11DeviceContext& context, char const* env_path, char const* specular_path, char const* diffuse_path, char const* brdf_path)
    : _pathEnv{env_path}, _pathSpecular{specular_path}, _pathDiffuse{diffuse_path}, _pathBRDF{brdf_path}
{
    using namespace DirectX;

    {
        std::wstring p (_pathEnv.length(), L' ');
        std::ranges::copy(_pathEnv, p.begin());

        auto alphaMode {DDS_ALPHA_MODE_UNKNOWN};

        CreateDDSTextureFromFileEx(
            &device,
            p.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
            0u,
            D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE,
            DDS_LOADER_DEFAULT,
            nullptr,
            _srvEnv.ReleaseAndGetAddressOf(),
            &alphaMode
        );

    }

    {
        std::wstring p (_pathSpecular.length(), L' ');
        std::ranges::copy(_pathSpecular, p.begin());

        auto alphaMode {DDS_ALPHA_MODE_UNKNOWN};

        CreateDDSTextureFromFileEx(
            &device,
            p.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
            0u,
            D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE,
            DDS_LOADER_DEFAULT,
            nullptr,
            _srvSpecular.ReleaseAndGetAddressOf(),
            &alphaMode
        );
    }

    {
        std::wstring p (_pathDiffuse.length(), L' ');
        std::ranges::copy(_pathDiffuse, p.begin());

        auto alphaMode {DDS_ALPHA_MODE_UNKNOWN};

        CreateDDSTextureFromFileEx(
            &device,
            p.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
            0u,
            D3D11_RESOURCE_MISC_GENERATE_MIPS | D3D11_RESOURCE_MISC_TEXTURECUBE,
            DDS_LOADER_DEFAULT,
            nullptr,
            _srvDiffuse.ReleaseAndGetAddressOf(),
            &alphaMode
        );
    }

    {
        std::wstring p (_pathBRDF.length(), L' ');
        std::ranges::copy(_pathBRDF, p.begin());

        auto alphaMode {DDS_ALPHA_MODE_UNKNOWN};

        CreateDDSTextureFromFileEx(
            &device,
            p.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE, 
            D3D11_CPU_ACCESS_READ,
            0,
            DDS_LOADER_DEFAULT,
            nullptr,
            _srvBRDF.ReleaseAndGetAddressOf(),
            &alphaMode
        );
    }

    context.GenerateMips(_srvEnv.Get());
    context.GenerateMips(_srvSpecular.Get());
    context.GenerateMips(_srvDiffuse.Get());
}

void Engine::Graphics::D3D11HDRCubemapTexture::Bind(ID3D11DeviceContext& context) const {
    context.PSSetShaderResources(20u, 1u, _srvEnv.GetAddressOf());
    context.PSSetShaderResources(21u, 1u, _srvSpecular.GetAddressOf());
    context.PSSetShaderResources(22u, 1u, _srvDiffuse.GetAddressOf());
    context.PSSetShaderResources(23u, 1u, _srvBRDF.GetAddressOf());
}

