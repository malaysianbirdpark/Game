#include "pch.h"
#include "D3D11RSLibrary.h"

void Engine::Graphics::D3D11RSLibrary::AddRS(x_string const& tag, std::shared_ptr<D3D11RootSignature> rs) {
    if (_lib.contains(tag)) {
        GRAPHICS_ERROR("Other RootSignature already has this tag");
        return;
    }

    _lib[tag] = std::move(rs);
}

std::shared_ptr<Engine::Graphics::D3D11RootSignature> Engine::Graphics::D3D11RSLibrary::ResolveRS(x_string const& tag) {
    if (!_lib.contains(tag)) {
        GRAPHICS_ERROR("No RootSignature has this tag");
        return nullptr;
    }

    return _lib[tag];
}
