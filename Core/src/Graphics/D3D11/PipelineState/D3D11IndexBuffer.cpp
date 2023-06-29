#include "gwpch.h"
#include "D3D11IndexBuffer.h"
#include "BindableCodex.h"

#include "Renderer/RenderDevice.h"

namespace Glowing::Bind {
    IndexBuffer::IndexBuffer(std::vector<unsigned short> const& indices)
        : IndexBuffer{"?", indices}
    {
    }

    IndexBuffer::IndexBuffer(std::string const& tag, std::vector<unsigned short> const& indices)
        : tag{std::move(tag)}, m_Count{static_cast<UINT>(indices.size())}
    {
        D3D11_BUFFER_DESC bd {};
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.CPUAccessFlags = 0u;
        bd.MiscFlags = 0u;
        bd.ByteWidth = static_cast<UINT>(m_Count * sizeof(unsigned short));
        bd.StructureByteStride = sizeof(unsigned short);

        D3D11_SUBRESOURCE_DATA sd {};
        sd.pSysMem = indices.data();

        GET_DEVICE->CreateBuffer(&bd, &sd, m_pIndexBuffer.ReleaseAndGetAddressOf());
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::Resolve(std::string const& tag, std::vector<unsigned short> const& indices) {
        GW_CORE_ASSERT(tag != "?", "you screwed up with tag");
        return Codex::Resolve<IndexBuffer>(tag, indices);
    }

    std::string IndexBuffer::GetUID() const noexcept {
        return GenerateUID(tag);
    }

    std::string IndexBuffer::GenerateUID_(std::string const& tag) {
        using namespace std::string_literals;
        return typeid(IndexBuffer).name() + "#"s + tag;
    }

    void IndexBuffer::Bind() noexcept {
        GET_CONTEXT->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
    }

    UINT IndexBuffer::GetCount() const noexcept {
        return m_Count;
    }
}
