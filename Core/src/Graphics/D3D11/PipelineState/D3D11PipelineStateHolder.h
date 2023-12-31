#pragma once

namespace Engine::Graphics {
    namespace Vertex {
        class Layout;        
        class Buffer;
    }

    class D3D11PipelineStateHolder {
        friend class D3D11PipelineStateObject;
    public:
        D3D11PipelineStateHolder() = default;
        ~D3D11PipelineStateHolder() = default;

        static std::shared_ptr<class D3D11PixelShader> ResolvePixelShader(ID3D11Device& device, char const* path); 
        static std::shared_ptr<class D3D11VertexShader> ResolveVertexShader(ID3D11Device& device, char const* path);
    private:
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11PixelShader>>   _pixelShader;
        inline static x_unordered_map<x_string, std::shared_ptr<D3D11VertexShader>>  _vertexShader;
    };
}

