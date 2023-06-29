#pragma once

#include "D3D11Bindable.h"

namespace Glowing::Bind {
    class GLOWING_API Texture final : public Bindable {
    public:
        Texture(std::string&& path, UINT slot = 0u);
        Texture(char const* path, UINT slot = 0u);

        void Bind() noexcept override;

        [[nodiscard]] static std::shared_ptr<Texture> Resolve(std::string const& path, UINT slot = 0u);
        [[nodiscard]] static std::string GenerateUID(std::string const& path, UINT slot = 0u);
        [[nodiscard]] std::string GetUID() const noexcept override;
    private:
        std::string path;
        UINT slot;
    protected:
        std::vector<Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> pvTextureView;
    };
}

