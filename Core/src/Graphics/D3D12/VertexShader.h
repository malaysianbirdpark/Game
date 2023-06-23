#pragma once

class VertexShader {
public:
    VertexShader();
private:
    Microsoft::WRL::ComPtr<ID3DBlob>               _blob;
    Microsoft::WRL::ComPtr<D3D12_SHADER_BYTECODE>  _byteCode;
};

