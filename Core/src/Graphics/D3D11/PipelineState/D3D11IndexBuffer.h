#pragma once

#include "D3D11Bindable.h"

namespace Glowing::Bind {
    class IndexBuffer final : public Bindable {
    public:
        IndexBuffer(std::vector<unsigned short> const& indices);
        IndexBuffer(std::string const& tag, std::vector<unsigned short> const& indices);

        void Bind() noexcept override;
        [[nodiscard]] UINT GetCount() const noexcept;

        [[nodiscard]] static std::shared_ptr<IndexBuffer> Resolve(std::string const& tag, std::vector<unsigned short> const& indices);

        template <typename... Ignore>
        [[nodiscard]] static std::string GenerateUID(std::string const& tag, Ignore&&... ignore) { return GenerateUID_(tag); }

        [[nodiscard]] std::string GetUID() const noexcept override;
    private:
        [[nodiscard]] static std::string GenerateUID_(std::string const& tag);
    protected:
        std::string tag;
        UINT m_Count;
        Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;
    };
}

