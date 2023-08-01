#include "pch.h"
#include "D3D11VertexBuffer.h"

namespace Engine::Graphics {
    D3D11VertexBuffer::D3D11VertexBuffer(ID3D11Device& device, UINT stride, UINT byte_width, void const* data, char const* tag)
        : _tag{tag}, _stride{stride}
    {
        D3D11_BUFFER_DESC bd {};
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.CPUAccessFlags = 0u;
        bd.MiscFlags = 0u;
        bd.ByteWidth = byte_width;
        bd.StructureByteStride = _stride;

        D3D11_SUBRESOURCE_DATA sd {};
        sd.pSysMem = data;

        device.CreateBuffer(&bd, &sd, _VertexBuffer.ReleaseAndGetAddressOf());
    }

    void D3D11VertexBuffer::Bind(ID3D11DeviceContext& context) noexcept {
        constexpr UINT offset {0u};
        context.IASetVertexBuffers(0u, 1u, _VertexBuffer.GetAddressOf(), &_stride, &offset);
    }

    x_string D3D11VertexBuffer::GenUID(char const* tag) {
        return x_string(typeid(D3D11VertexBuffer).name()) + "#" + tag;
    }

    x_string D3D11VertexBuffer::GetUID() const {
        return GenUID(_tag.c_str());
    }
}
