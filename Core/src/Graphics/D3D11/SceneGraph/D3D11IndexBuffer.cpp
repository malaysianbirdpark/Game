#include "pch.h"
#include "D3D11IndexBuffer.h"

namespace Engine::Graphics {
    D3D11IndexBuffer::D3D11IndexBuffer(ID3D11Device& device, void const* data, UINT count, char const* tag)
        : _tag{tag}, _count{count}
    {
        D3D11_BUFFER_DESC bd {};
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.Usage = D3D11_USAGE_IMMUTABLE;
        bd.CPUAccessFlags = 0u;
        bd.MiscFlags = 0u;
        bd.ByteWidth = static_cast<UINT>(_count * sizeof(unsigned short));
        bd.StructureByteStride = sizeof(unsigned short);

        D3D11_SUBRESOURCE_DATA sd {};
        sd.pSysMem = data;

        device.CreateBuffer(&bd, &sd, _indexBuffer.ReleaseAndGetAddressOf());
    }

    void D3D11IndexBuffer::Bind(ID3D11DeviceContext& context) noexcept {
        context.IASetIndexBuffer(_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
    }

    UINT D3D11IndexBuffer::GetCount() const noexcept {
        return _count;
    }

    x_string D3D11IndexBuffer::GenUID(char const* tag) {
        return x_string{typeid(D3D11IndexBuffer).name()} + "#" + tag;
    }

    x_string D3D11IndexBuffer::GetUID() const {
        return GenUID(_tag.c_str());
    }
}
