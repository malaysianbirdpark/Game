#pragma once
                
#include "ConstantBuffer/D3D11ConstantBuffer.h"
#include "ShaderResource/D3D11ShaderResource.h"

namespace Engine::Graphics {
    class D3D11PipelineStateObject;

    class D3D11RootSignature {
    public:
        D3D11RootSignature(
            ID3D11Device& device, 
            std::shared_ptr<D3D11PipelineStateObject> const& target_pso, 
            uint16_t priority
        );
        ~D3D11RootSignature() = default;

        void AddConstantBuffer(ID3D11Device& device, D3D11ConstantBuffer const& cb);
        //void AddSampler(ID3D11Device& device, char const* tag);
        void AddShaderResource(ID3D11Device& device, D3D11ShaderResource const& sr);

        void Update(ID3D11DeviceContext& context);

        void Bind(ID3D11DeviceContext& context);
    private:
        std::shared_ptr<D3D11PipelineStateObject>      _targetPSO {};
        uint16_t                                       _priority {};

        x_vector<std::shared_ptr<D3D11ConstantBuffer>>  _cbs;
        //x_vector<std::shared_ptr<D3D11Sampler>>         _samplers;
        x_vector<std::shared_ptr<D3D11ShaderResource>>  _srs;
    };
}

