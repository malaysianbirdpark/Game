#include "pch.h"
#include "D3D11Texture.h"

#include <directxtk/DDSTextureLoader.h>

namespace Engine::Graphics {
    D3D11Texture::D3D11Texture(ID3D11Device& device, char const* path)
        : _path{path}
    {
        using namespace DirectX;

        std::wstring p (_path.length(), L' ');
        std::ranges::copy(_path, p.begin());

        auto alphaMode {DDS_ALPHA_MODE_STRAIGHT};

        _textureView.emplace_back(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>{});
        CreateDDSTextureFromFileEx(
            &device,
            p.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
            D3D11_CPU_ACCESS_READ,
            D3D11_RESOURCE_MISC_GENERATE_MIPS,
            DDS_LOADER_DEFAULT,
            nullptr,
            _textureView[0].ReleaseAndGetAddressOf(),
            &alphaMode
        );

    }

    void D3D11Texture::Bind(ID3D11DeviceContext& context, UINT slot) noexcept {
        context.PSSetShaderResources(slot, _textureView.size(), _textureView.data()->GetAddressOf());
    }

    x_string D3D11Texture::GenUID(char const* path) {
        return x_string{typeid(D3D11Texture).name()} + "#" + path;
    }

    x_string D3D11Texture::GetUID() const {
        return GenUID(_path.c_str());
    }
}
