#include "gwpch.h"
#include "D3D11VertexShader.h"
#include "BindableCodex.h"
#include "../../Utility/UTFConv.h"

#include "Renderer/RenderDevice.h"

#include <d3dcompiler.h>

namespace Glowing::Bind {
    VertexShader::VertexShader(std::string&& path)
        : path{std::forward<std::string>(path)}
    {
        D3DReadFileToBlob(Glowing::Util::UTFConv::ConvertStoW(this->path).data(), m_pByteCode.ReleaseAndGetAddressOf());
        GET_DEVICE->CreateVertexShader(
            m_pByteCode->GetBufferPointer(),
            m_pByteCode->GetBufferSize(),
            nullptr,
            m_pVertexShader.ReleaseAndGetAddressOf()
        );
    }

    VertexShader::VertexShader(char const* path)
        : VertexShader{std::string{path}}
    {
    }

    void VertexShader::Bind() noexcept {
        GET_CONTEXT->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
    }

    ID3DBlob* VertexShader::GetByteCode() const noexcept {
        return m_pByteCode.Get();
    }

    std::shared_ptr<VertexShader> VertexShader::Resolve(std::string const& path) {
        return Codex::Resolve<VertexShader>(path);
    }

    std::string VertexShader::GenerateUID(std::string const& path) {
        using namespace std::string_literals;
        return typeid(VertexShader).name() + "#"s + path;
    }

    std::string VertexShader::GetUID() const noexcept {
        return GenerateUID(path);
    }
}
