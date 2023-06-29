#include "gwpch.h"
#include "D3D11VertexBuffer.h"
#include "BindableCodex.h"

#include "../../D3DCommon/D3DVertex.h"

#include "Renderer/RenderDevice.h"

namespace Glowing::Bind {
    VertexBuffer::VertexBuffer(std::string const& tag, Vertex::VertexBuffer&& vbuf)
        : tag{std::move(tag)}, stride{static_cast<UINT>(vbuf.GetLayout().SizeBytes())}
    {
        D3D11_BUFFER_DESC bd {};
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.CPUAccessFlags = 0u;
        bd.MiscFlags = 0u;
        bd.ByteWidth = static_cast<UINT>(vbuf.GetSizeBytes());
        bd.StructureByteStride = stride;

        D3D11_SUBRESOURCE_DATA sd {};
        sd.pSysMem = vbuf.GetData();

        GET_DEVICE->CreateBuffer(&bd, &sd, p_VertexBuffer.ReleaseAndGetAddressOf());
        vbuf.Destroy();
    }

    VertexBuffer::VertexBuffer(Vertex::VertexBuffer&& vbuf)
        : VertexBuffer{"?", std::move(vbuf)}
    {
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::Resolve(std::string const& tag, Vertex::VertexBuffer const& vbuf) {
        GW_CORE_ASSERT(tag != "?", "you screwed up with tag");
        return Codex::Resolve<VertexBuffer>(tag, vbuf);
    }

    template <typename ... Ignore>
    std::string VertexBuffer::GenerateUID(std::string const& tag, Ignore&&... ignore) {
        using namespace std::string_literals;
        return typeid(VertexBuffer).name() + "#"s + tag;
    }

    std::string VertexBuffer::GetUID() const noexcept {
        return GenerateUID(tag);
    }

    void VertexBuffer::Bind() noexcept {
        constexpr UINT offset {0u};
        GET_CONTEXT->IASetVertexBuffers(0u, 1u, p_VertexBuffer.GetAddressOf(), &stride, &offset);
    }
}
