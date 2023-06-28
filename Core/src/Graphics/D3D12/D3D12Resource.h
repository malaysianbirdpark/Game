#pragma once

namespace Engine::Graphics {
    class D3D12Resource {
    private:
        Microsoft::WRL::ComPtr<ID3D12Resource>        _resource;
    };
}

