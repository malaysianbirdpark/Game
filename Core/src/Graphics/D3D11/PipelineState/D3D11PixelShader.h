#pragma once

#include "D3D11Bindable.h"

namespace Glowing::Bind {
    class GLOWING_API PixelShader final : public Bindable {
    public:
        PixelShader(std::string&& path);
        PixelShader(char const* path);

        void Bind() noexcept override;

        [[nodiscard]] static std::shared_ptr<PixelShader> Resolve(std::string const& path);
        [[nodiscard]] static std::string GenerateUID(std::string const& path);
        [[nodiscard]] std::string GetUID() const noexcept override;
    protected:
        std::string path;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;
    };
}

