#include "pch.h"
#include "D3D11ShaderResource.h"

Engine::Graphics::D3D11ShaderResource Engine::Graphics::D3D11ShaderResourceHolder::Resolve(ID3D11Device& device, ID3D11DeviceContext& context, ShaderResourceTypes type, char const* path) {
    auto const uid {GenUIDTable[static_cast<int>(type) - static_cast<int>(ShaderResourceTypes::EmissiveMap)](path)};

    if (_srs.contains(uid))
        return _srs[uid];
    return _srs[uid] = std::move(ConstructorTable[static_cast<int>(type) - static_cast<int>(ShaderResourceTypes::EmissiveMap)](device, context, path));
}
