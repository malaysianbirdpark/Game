#pragma once

#include "D3D11Bindable.h"

namespace Glowing::Bind {
    class GLOWING_API VertexShader final : public Bindable {
    public:
        VertexShader(std::string&& path);
        VertexShader(char const* path);

        void Bind() noexcept override;
        [[nodiscard]] ID3DBlob* GetByteCode() const noexcept;

        [[nodiscard]] static std::shared_ptr<VertexShader> Resolve( std::string const& path);
        [[nodiscard]] static std::string GenerateUID(std::string const& path);
        [[nodiscard]] std::string GetUID() const noexcept override;
    protected:
        std::string path;
        Microsoft::WRL::ComPtr<ID3DBlob> m_pByteCode;
        Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
    };
}

