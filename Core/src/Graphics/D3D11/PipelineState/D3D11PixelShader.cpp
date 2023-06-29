#include "gwpch.h"
#include "D3D11PixelShader.h"
#include "BindableCodex.h"

#include "../../Utility/UTFConv.h"

#include "Renderer/RenderDevice.h"

#include <d3dcompiler.h>

namespace Glowing::Bind {
    PixelShader::PixelShader(std::string&& path)
        : path{std::forward<std::string>(path)}
    {
        Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
        D3DReadFileToBlob(Util::UTFConv::ConvertStoW(this->path).c_str(), pBlob.ReleaseAndGetAddressOf());
        GET_DEVICE->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, m_pPixelShader.ReleaseAndGetAddressOf());
    }

    PixelShader::PixelShader(char const* path)
        : PixelShader{std::string{path}}
    {
    }

    void PixelShader::Bind() noexcept {
        GET_CONTEXT->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
    }

    std::shared_ptr<PixelShader> PixelShader::Resolve(std::string const& path) {
        return Codex::Resolve<PixelShader>(path);
    }

    std::string PixelShader::GenerateUID(std::string const& path) {
        using namespace std::string_literals;
        return typeid(PixelShader).name() + "#"s + path;
    }

    std::string PixelShader::GetUID() const noexcept {
        return GenerateUID(path);
    }
}
