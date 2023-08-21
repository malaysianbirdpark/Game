#include "pch.h"
#include "D3D11ShaderResource.h"

Engine::Graphics::D3D11ShaderResource Engine::Graphics::D3D11ShaderResourceHolder::Resolve(ID3D11Device& device, ShaderResourceTypes type, char const* path) {
    auto const uid {GenUIDTable[static_cast<int>(type)](path)};

    if (_srs.contains(uid))
        return _srs[uid];
    return _srs[uid] = ConstructorTable[static_cast<int>(type)](device, path);
}
