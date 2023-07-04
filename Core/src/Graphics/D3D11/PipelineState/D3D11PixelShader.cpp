#include "pch.h"
#include "D3D11PixelShader.h"

#include <d3dcompiler.h>

namespace Engine::Graphics {
    D3D11PixelShader::D3D11PixelShader(ID3D11Device& device, char const* path)
        : _path{path}
    {
        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;

        std::wstring p (_path.length(), L' ');
        std::ranges::copy(_path, p.begin());

        D3DReadFileToBlob(p.c_str(), pBlob.ReleaseAndGetAddressOf());
        device.CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf());
    }

    void D3D11PixelShader::Bind(ID3D11DeviceContext& context) noexcept {
        context.PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
    }

    x_string D3D11PixelShader::GenUID(x_string const& path) {
        return x_string{typeid(D3D11PixelShader).name()} + "#" + path;
    }

    x_string D3D11PixelShader::GetUID() const {
        return GenUID(_path);
    }
}
