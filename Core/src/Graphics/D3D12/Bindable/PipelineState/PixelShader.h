#pragma once

namespace Engine::Graphics {
class PixelShader {
public:
    PixelShader(wchar_t const* path);

    Microsoft::WRL::ComPtr<ID3DBlob> GetBlob() const { return _blob; }
    D3D12_SHADER_BYTECODE GetByteCode() const { return CD3DX12_SHADER_BYTECODE{_blob.Get()}; }
private:
    Microsoft::WRL::ComPtr<ID3DBlob>               _blob;
    //Microsoft::WRL::ComPtr<D3D12_SHADER_BYTECODE>  _byteCode;
};
}

