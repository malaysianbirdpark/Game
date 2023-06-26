#include "pch.h"
#include "PixelShader.h"

#include <d3dcompiler.h>

Engine::Graphics::PixelShader::PixelShader(wchar_t const* path) {
    if (FAILED(D3DReadFileToBlob(path, _blob.ReleaseAndGetAddressOf()))) {
        GRAPHICS_ERROR("Read VertexShader to blob failed");
        CRASH();
    }
}
