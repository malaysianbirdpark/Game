#include "gwpch.h"
#include "D3D11Texture.h"
#include "BindableCodex.h"
#include "../../Utility/UTFConv.h"
#include <directxtk/DDSTextureLoader.h>

#include "Renderer/RenderDevice.h"

namespace Glowing::Bind {
    Texture::Texture(std::string&& path, UINT slot)
        : path{std::forward<std::string>(path)}, slot{slot}
    {
        using namespace DirectX;
        auto const lPath {Util::UTFConv::ConvertStoW(this->path)};
        auto alphaMode {DDS_ALPHA_MODE_STRAIGHT};
        pvTextureView.emplace_back(Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>{});
        CreateDDSTextureFromFileEx(
            GET_DEVICE,
            lPath.c_str(),
            0,
            D3D11_USAGE_DEFAULT,
            D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,
            D3D11_CPU_ACCESS_READ,
            D3D11_RESOURCE_MISC_GENERATE_MIPS,
            DDS_LOADER_DEFAULT,
            nullptr,
            pvTextureView[0].ReleaseAndGetAddressOf(),
            &alphaMode
        );
    }

    Texture::Texture(char const* path, UINT slot)
        : Texture{std::string{path}, slot}
    {
    }

    void Texture::Bind() noexcept {
        GET_CONTEXT->PSSetShaderResources(slot, pvTextureView.size(), pvTextureView.data()->GetAddressOf());
    }

    std::shared_ptr<Texture> Texture::Resolve(std::string const& path, UINT slot) {
        return Codex::Resolve<Texture>(path, slot);
    }

    std::string Texture::GenerateUID(std::string const& path, UINT slot) {
        using namespace std::string_literals;
        return typeid(Texture).name() + "#"s + path + std::to_string(slot);
    }

    std::string Texture::GetUID() const noexcept {
        return GenerateUID(path, slot);
    }
}
