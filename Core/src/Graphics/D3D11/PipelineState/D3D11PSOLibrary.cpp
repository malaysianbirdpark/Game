#include "pch.h"
#include "D3D11PSOLibrary.h"

void Engine::Graphics::D3D11PSOLibrary::AddPSO(x_string const& tag, std::shared_ptr<D3D11PipelineStateObject> const& pso) {
    if (_lib.contains(tag))
        return;

    _lib[tag] = std::move(pso);
}

std::shared_ptr<Engine::Graphics::D3D11PipelineStateObject> Engine::Graphics::D3D11PSOLibrary::ResolvePSO(x_string const& tag) {
    if (!_lib.contains(tag))
        return nullptr;

    return _lib[tag];
}
