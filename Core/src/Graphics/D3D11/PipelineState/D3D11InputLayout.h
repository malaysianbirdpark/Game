#pragma once

#include "D3D11Bindable.h"
#include "../../D3DCommon/D3DVertex.h"

namespace Glowing::Bind {
    class GLOWING_API InputLayout final : public Bindable {
    public:
        InputLayout(Vertex::Layout const& layout, ID3DBlob* pVertexShaderByteCode);

        void Bind() noexcept override;

        [[nodiscard]] static std::shared_ptr<InputLayout> Resolve(Vertex::Layout const& layout, ID3DBlob* pVertexShaderByteCode);
        [[nodiscard]] static std::string GenerateUID(Vertex::Layout const& layout, ID3DBlob* pVertexShaderByteCode = nullptr);
        [[nodiscard]] std::string GetUID() const noexcept override;
    private:
        Vertex::Layout layout;
        Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout;
    };
}

