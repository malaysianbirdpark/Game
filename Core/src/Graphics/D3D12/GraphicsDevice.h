#pragma once

namespace Engine::Graphics {
    class GraphicsDevice {
    public:
        GraphicsDevice();
        ~GraphicsDevice() = default;

        [[nodiscard]] IDXGIFactory& GetDxgi() { return *_dxgi.Get(); }
        [[nodiscard]] ID3D12Device& GetDevice() { return *_device.Get(); }
    private:
        Microsoft::WRL::ComPtr<ID3D12Debug1>   _debug;
        Microsoft::WRL::ComPtr<IDXGIFactory>  _dxgi;
        Microsoft::WRL::ComPtr<ID3D12Device>  _device;
    };
}

