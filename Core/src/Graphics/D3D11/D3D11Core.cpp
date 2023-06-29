#include "pch.h"
#include "D3D11Core.h"

Engine::Graphics::D3D11Core::D3D11Core(int width, int height, HWND native_wnd, bool windowed)
    : _windowInfo{width, height, native_wnd, windowed}
{
    {
        DXGI_SWAP_CHAIN_DESC sd {};
        sd.BufferDesc.Width = 0;
        sd.BufferDesc.Height = 0;
        sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        sd.BufferDesc.RefreshRate.Numerator = 0;
        sd.BufferDesc.RefreshRate.Denominator = 0;
        sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
        sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
        sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        sd.BufferCount = 3;
        sd.OutputWindow = native_wnd;
        sd.Windowed = windowed;
        sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
        sd.Flags = 0u;

        auto swap_chain_flag {0u};
#ifdef _DEBUG
        swap_chain_flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

        ::D3D11CreateDeviceAndSwapChain(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            nullptr,
            swap_chain_flag,
            nullptr,
            0,
            D3D11_SDK_VERSION,
            &sd,
            _swapChain.ReleaseAndGetAddressOf(),
            _device.ReleaseAndGetAddressOf(),
            nullptr,
            _context.ReleaseAndGetAddressOf()
        );

        _swapChain->GetBuffer(0u, IID_PPV_ARGS(_backBuffers.ReleaseAndGetAddressOf()));
        _device->CreateRenderTargetView(_backBuffers.Get(), nullptr, _backBufferView.ReleaseAndGetAddressOf());

        _viewPort.Width = static_cast<float>(width);
        _viewPort.Height = static_cast<float>(height);
        _viewPort.MinDepth = 0.0f;
        _viewPort.MaxDepth = 1.0f;
        _viewPort.TopLeftX = 0.0f;
        _viewPort.TopLeftY = 0.0f;
        _context->RSSetViewports(1u, &_viewPort);

        {
            D3D11_DEPTH_STENCIL_DESC desc {};
            desc.DepthEnable = TRUE;
            desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            desc.DepthFunc = D3D11_COMPARISON_LESS;

            Microsoft::WRL::ComPtr<ID3D11DepthStencilState> ds_state;
            _device->CreateDepthStencilState(&desc, ds_state.ReleaseAndGetAddressOf());
            _context->OMSetDepthStencilState(ds_state.Get(), 1u);

            D3D11_TEXTURE2D_DESC descDepth {};
            descDepth.Usage = D3D11_USAGE_DEFAULT;
            descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
            descDepth.Width = width;
            descDepth.Height = height;
            descDepth.MipLevels = 1u;
            descDepth.ArraySize = 1u;
            descDepth.Format = DXGI_FORMAT_D32_FLOAT;
            descDepth.SampleDesc.Count = 1u;
            descDepth.SampleDesc.Quality = 0u;

            Microsoft::WRL::ComPtr<ID3D11Texture2D> _ds;
            _device->CreateTexture2D(&descDepth, nullptr, _ds.ReleaseAndGetAddressOf());

            D3D11_DEPTH_STENCIL_VIEW_DESC desc_dsv {};
            desc_dsv.Format = DXGI_FORMAT_D32_FLOAT;
            desc_dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
            desc_dsv.Texture2D.MipSlice = 0u;
            _device->CreateDepthStencilView(_ds.Get(), &desc_dsv, _DSV.ReleaseAndGetAddressOf());
        }

        _context->OMSetRenderTargets(1u, _backBufferView.GetAddressOf(), _DSV.Get());
    }
}

Engine::Graphics::D3D11Core::~D3D11Core() {
}

void Engine::Graphics::D3D11Core::Render() {
    BeginFrame();
    EndFrame();
}

void Engine::Graphics::D3D11Core::BeginFrame() {
    _context->OMSetRenderTargets(1u, _backBufferView.GetAddressOf(), _DSV.Get());
    static float clear_color[4] {0.23f, 0.08f, 0.73f, 1.0f};
    _context->ClearRenderTargetView(_backBufferView.Get(), clear_color);

}

void Engine::Graphics::D3D11Core::EndFrame() {
    _swapChain->Present(1u, 0u);
}
