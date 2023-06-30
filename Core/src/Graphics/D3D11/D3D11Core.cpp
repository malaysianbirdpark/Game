#include "pch.h"
#include "D3D11Core.h"

#include "PipelineState/D3D11PipelineStateHolder.h"
#include "PipelineState/D3D11PipelineStateObject.h"
#include "RootSignature/D3D11RootSignatureHolder.h"
#include "RootSignature/D3D11RootSignature.h"

#include "../Vertex.h"

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
            _immContext.ReleaseAndGetAddressOf()
        );

        _swapChain->GetBuffer(0u, IID_PPV_ARGS(_backBuffers.ReleaseAndGetAddressOf()));
        _device->CreateRenderTargetView(_backBuffers.Get(), nullptr, _backBufferView.ReleaseAndGetAddressOf());

        _viewPort.Width = static_cast<float>(width);
        _viewPort.Height = static_cast<float>(height);
        _viewPort.MinDepth = 0.0f;
        _viewPort.MaxDepth = 1.0f;
        _viewPort.TopLeftX = 0.0f;
        _viewPort.TopLeftY = 0.0f;
        _immContext->RSSetViewports(1u, &_viewPort);

        {
            D3D11_DEPTH_STENCIL_DESC desc {};
            desc.DepthEnable = TRUE;
            desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
            desc.DepthFunc = D3D11_COMPARISON_LESS;

            Microsoft::WRL::ComPtr<ID3D11DepthStencilState> ds_state;
            _device->CreateDepthStencilState(&desc, ds_state.ReleaseAndGetAddressOf());
            _immContext->OMSetDepthStencilState(ds_state.Get(), 1u);

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

        _immContext->OMSetRenderTargets(1u, _backBufferView.GetAddressOf(), _DSV.Get());

        // TODO: TEMP
        Vertex::Layout layout {};
        layout.Append(Vertex::Layout::Position3D)
              .Append(Vertex::Layout::Float4Color);

        Vertex::Buffer vbuf {layout};
        vbuf.EmplaceBack(
            DirectX::XMFLOAT3{0.0f, 0.5f, 0.5f},
            DirectX::XMFLOAT4{1.0f, 0.0f, 0.0f, 1.0f}
        );
        vbuf.EmplaceBack(
            DirectX::XMFLOAT3{0.5f, -0.5f, 0.5f},
            DirectX::XMFLOAT4{1.0f, 1.0f, 0.0f, 1.0f}
        );
        vbuf.EmplaceBack(
            DirectX::XMFLOAT3{-0.5f, -0.5f, 0.5f},
            DirectX::XMFLOAT4{1.0f, 0.0f, 1.0f, 1.0f}
        );

        x_vector<uint16_t> indices {0, 2, 1};

        _pso["triangle"] = MakeUnique<D3D11PipelineStateObject>();
        auto& device {*_device.Get()};
        _pso["triangle"]->SetIndexBuffer(device, indices, "triangle");
        _pso["triangle"]->SetVertexShader(device, "./ShaderLib/basicVS.cso");
        _pso["triangle"]->SetInputLayout(device, layout);
        _pso["triangle"]->SetVertexBuffer(device, vbuf, "triangle");
        _pso["triangle"]->SetPixelShader(device, "./ShaderLib/basicPS.cso");

        _rs["triangle"] = MakeUnique<D3D11RootSignature>();
        auto rotate {DirectX::XMMatrixRotationZ(10.0f)};
        _rs["triangle"]->AddConstantBuffer(device, 0u, &rotate, sizeof(rotate), true, true, true, "triangle");
    }
}

Engine::Graphics::D3D11Core::~D3D11Core() {
}

void Engine::Graphics::D3D11Core::Render() {
    BeginFrame();
    EndFrame();
}

void Engine::Graphics::D3D11Core::BeginFrame() {
    _immContext->OMSetRenderTargets(1u, _backBufferView.GetAddressOf(), _DSV.Get());
    static float clear_color[4] {0.23f, 0.08f, 0.73f, 1.0f};
    _immContext->ClearRenderTargetView(_backBufferView.Get(), clear_color);

    ID3D11DeviceContext& context {*_immContext.Get()};
    _pso["triangle"]->Bind(context);
    _rs["triangle"]->Bind(context);

    context.DrawIndexed(3, 0u, 0u);
}

void Engine::Graphics::D3D11Core::EndFrame() {
    _swapChain->Present(1u, 0u);
}
