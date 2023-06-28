#pragma once

#include "../Bindable.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/D3D12/Commander.h"

namespace Engine::Graphics {
class RootSignatureElement : public Bindable {
public:
    RootSignatureElement(UINT slot) : _slot{slot} {};
    virtual ~RootSignatureElement() override = default;
    virtual void Bind(GraphicsContext& gfx) noexcept override {}
    [[nodiscard]] std::string GetUID() const noexcept override { return {}; }
protected:
    UINT _slot;
};

template <typename T>
class RSConstant : public RootSignatureElement {
public:
    RSConstant(T& data, UINT slot) : RootSignatureElement{slot}, _data{data} {}
    virtual ~RSConstant() override = default;

    virtual void Bind(GraphicsContext& gfx) noexcept override {
        CMD_LIST().SetGraphicsRoot32BitConstants(_slot, sizeof(_data) / sizeof(DWORD), &_data, 0);
    }

    [[nodiscard]] std::string GetUID() const noexcept override { return {}; };
private:
    T _data;
};

//class RSConstantBuffer : public RootSignatureElement {
//public:
//    RSConstantBuffer(UINT slot) : RootSignatureElement{slot} {}
//    virtual ~RSConstantBuffer() override = default;
//    void Bind(GraphicsContext& gfx) noexcept override {
//        //CMD_LIST().SetGraphicsRootConstantBufferView(_slot, nullptr);
//    }
//    [[nodiscard]] std::string GetUID() const noexcept override { return {}; };
//private:
//    //std::unique_ptr<class CPUBuffer> _buffer;
//};

class RSDescTable : public RootSignatureElement {
    
};

class RootSignature : public Bindable {
public:
    RootSignature() = default;
    virtual ~RootSignature() override = default;

    template <typename T>
    RootSignature&    AddConstant(T& data, D3D12_SHADER_VISIBILITY visibility);
    RootSignature&    AddConstantBuffer();
    void              Cook(GraphicsContext& gfx);
    void              SetFlag(D3D12_ROOT_SIGNATURE_FLAGS flag) { _flag |= flag; }
    void              ClearFlag(D3D12_ROOT_SIGNATURE_FLAGS flag) { _flag &= ~flag; }
    
    Microsoft::WRL::ComPtr<ID3D12RootSignature> GetSignature() const { return _signature; }

    virtual void Bind(GraphicsContext& gfx) noexcept override;
    [[nodiscard]] virtual std::string GetUID() const noexcept override;
private:
    CD3DX12_ROOT_SIGNATURE_DESC GenerateDesc() const;
protected:
    UINT                                             _slot {};
    x_vector<std::unique_ptr<RootSignatureElement>>  _elems;
    x_vector<CD3DX12_ROOT_PARAMETER>                 _params;
    Microsoft::WRL::ComPtr<ID3D12RootSignature>      _signature;
private:
    // Default Flag
    D3D12_ROOT_SIGNATURE_FLAGS                         _flag {
        D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT    |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_MESH_SHADER_ROOT_ACCESS          |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_AMPLIFICATION_SHADER_ROOT_ACCESS |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_DOMAIN_SHADER_ROOT_ACCESS        |
        D3D12_ROOT_SIGNATURE_FLAG_DENY_GEOMETRY_SHADER_ROOT_ACCESS      
    };
};

template <typename T>
RootSignature& RootSignature::AddConstant(T& data, D3D12_SHADER_VISIBILITY visibility) {
    _params.push_back(std::move(CD3DX12_ROOT_PARAMETER{}));
    _params.back().InitAsConstants(sizeof(T) / sizeof(DWORD), _slot, 0, visibility);
    _elems.push_back(std::move(MakeUnique<RSConstant<T>>(data, _slot)));
    ++_slot;

    return *this;
}
}

