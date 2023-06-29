#include "gwpch.h"
#include "D3D11InputLayout.h"

#include "BindableCodex.h"

#include "Renderer/RenderDevice.h"

namespace Glowing {
    namespace Vertex {
        class Layout;
    }
}

namespace Glowing::Bind {
    InputLayout::InputLayout(Vertex::Layout const& layout, ID3DBlob* pVertexShaderByteCode)
        : layout{layout}
    {
        auto const D3DLayout {layout.GenerateD3D11Layout()};
        GET_DEVICE->CreateInputLayout(
            D3DLayout.data(),
            static_cast<UINT>(D3DLayout.size()),
            pVertexShaderByteCode->GetBufferPointer(),
            pVertexShaderByteCode->GetBufferSize(),
            m_pInputLayout.ReleaseAndGetAddressOf()
        );
    }

    void InputLayout::Bind() noexcept {
        GET_CONTEXT->IASetInputLayout(m_pInputLayout.Get());
    }

    std::shared_ptr<InputLayout> InputLayout::Resolve(Vertex::Layout const& layout, ID3DBlob* pVertexShaderByteCode) {
        return Codex::Resolve<InputLayout>(layout, pVertexShaderByteCode);
    }

    std::string InputLayout::GenerateUID(Vertex::Layout const& layout, ID3DBlob* pVertexShaderByteCode) {
        using namespace std::string_literals;
        return typeid(InputLayout).name() + "#"s + layout.GetCode();
    }

    std::string InputLayout::GetUID() const noexcept {
        return Bindable::GetUID();
    }
}
