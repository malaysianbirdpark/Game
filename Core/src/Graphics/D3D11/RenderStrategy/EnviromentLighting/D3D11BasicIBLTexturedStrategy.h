#pragma once

namespace Engine::Graphics {
    class D3D11PipelineStateObject;
    class D3D11Mesh;
    class D3D11Material;

    class D3D11BasicIBLTexturedStrategy {
    public:
        static void Init();
        static void Render(ID3D11DeviceContext& context, D3D11Mesh const& mesh, D3D11Material& material);
    private:
        inline static std::shared_ptr<D3D11PipelineStateObject> _pso {nullptr};
    };
}

