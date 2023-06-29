#pragma once

#include "D3D11Bindable.h"

/*** Forward Decl ***/
namespace Glowing::Vertex {
    class VertexBuffer;
}
/********************/

namespace Glowing::Bind {
    class VertexBuffer final : public Bindable {
    public:
        VertexBuffer(std::string const& tag, Vertex::VertexBuffer&& vbuf);
        VertexBuffer(Vertex::VertexBuffer&& vbuf);

        void Bind() noexcept override;

        [[nodiscard]] static std::shared_ptr<VertexBuffer> Resolve(std::string const& tag, Vertex::VertexBuffer const& vbuf);
        template <typename... Ignore>
        [[nodiscard]] static std::string GenerateUID(std::string const& tag, Ignore&&... ignore);
        [[nodiscard]] std::string GetUID() const noexcept override;
    private:
        std::string tag;
        UINT stride;
        Microsoft::WRL::ComPtr<ID3D11Buffer> p_VertexBuffer;
    };
}

