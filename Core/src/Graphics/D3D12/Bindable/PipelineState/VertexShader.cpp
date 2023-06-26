#include "pch.h"
#include "VertexShader.h"

#include <d3dcompiler.h>

Engine::Graphics::VertexShader::VertexShader(wchar_t const* path) {
    if (FAILED(D3DReadFileToBlob(path, _blob.ReleaseAndGetAddressOf()))) {
        GRAPHICS_ERROR("Read VertexShader to blob failed");
        CRASH();
    }
}
