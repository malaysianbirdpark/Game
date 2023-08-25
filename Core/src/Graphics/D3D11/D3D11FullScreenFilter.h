#pragma once
#include "ConstantBuffer/D3D11TransformMVP.h"
#include "SceneGraph/D3D11IndexBuffer.h"
#include "SceneGraph/D3D11VertexBuffer.h"

namespace Engine::Graphics {
    class D3D11FullScreenFilter {
    public:
        D3D11FullScreenFilter(ID3D11Device& device);

        void Render(ID3D11DeviceContext& context, ID3D11ShaderResourceView* const* input, ID3D11RenderTargetView* const* output);
    private:
        struct FullScreenVertex {
            DirectX::XMFLOAT2 pos;            
        };

        inline static x_array<FullScreenVertex, 4> _vertices {{
            {{-1, 1}}, {{1, 1}}, {{-1, -1}}, {{1, -1}}        
        }};
        inline static x_array<uint16_t, 6> _indices {0, 1, 2, 1, 3, 2};
    private:
        D3D11_PRIMITIVE_TOPOLOGY                          _topology {D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST};
        D3D11VertexBuffer                                 _vertex;
        D3D11IndexBuffer                                  _index;

        Microsoft::WRL::ComPtr<ID3D11SamplerState>        _sampler;
        std::shared_ptr<class D3D11PipelineStateObject>   _psoBlur;
    };
}

