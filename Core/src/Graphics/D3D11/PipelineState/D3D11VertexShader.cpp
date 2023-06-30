#include "pch.h"
#include "D3D11VertexShader.h"

#include <d3dcompiler.h>

namespace Engine::Graphics {
    D3D11VertexShader::D3D11VertexShader(ID3D11Device& device, char const* path)
        : _path{path}
    {
        std::wstring p (_path.length(), L' ');
        std::ranges::copy(_path, p.begin());

        D3DReadFileToBlob(
            p.c_str(),
            m_pByteCode.ReleaseAndGetAddressOf()
        );

        device.CreateVertexShader(
            m_pByteCode->GetBufferPointer(),
            m_pByteCode->GetBufferSize(),
            nullptr,
            m_pVertexShader.ReleaseAndGetAddressOf()
        );
    }

    void D3D11VertexShader::Bind(ID3D11DeviceContext& context) noexcept {
        context.VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
    }

    ID3DBlob* D3D11VertexShader::GetByteCode() const noexcept {
        return m_pByteCode.Get();
    }

    x_string D3D11VertexShader::GenUID(x_string const& path) {
        return x_string{typeid(D3D11VertexShader).name()} + "#" + path;
    }

    x_string D3D11VertexShader::GetUID() const {
        return GenUID(_path);
    }
}
