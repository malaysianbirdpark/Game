#include "pch.h"
#include "D3D11Core.h"

//TEMP
#include <random>

#include "D3D11Sampler.h"
#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"
#include "Graphics/D3D11/SceneGraph/D3D11SceneMan.h"
#include "Graphics/D3D11/RenderStrategy/D3D11RenderStrategy.h"
#include "Graphics/D3D11/D3D11RenderObject.h"

#include "Graphics/D3DCamera.h"
#include "PipelineState/D3D11PSOLibrary.h"
#include "PipelineState/D3D11PipelineStateObject.h"

#include <imgui.h>

#include "D3D11CubeMap.h"
#include "Graphics/GUI/D3D11ImGuiRenderer.h"

#include "Graphics/D3D11/ConstantBuffer/D3D11CamPosition.h"
#include "Graphics/D3D11/ConstantBuffer/D3D11LightDirectional.h"

Engine::Graphics::D3D11Core::D3D11Core(int width, int height, HWND native_wnd, bool windowed)
    : _windowInfo{width, height, native_wnd, windowed}
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

    for (auto& context : _defContexts) {
        _device->CreateDeferredContext(
            0u,
            context.ReleaseAndGetAddressOf()
        );
    }

    _swapChain->GetBuffer(0u, IID_PPV_ARGS(_backBuffers.ReleaseAndGetAddressOf()));
    _device->CreateRenderTargetView(_backBuffers.Get(), nullptr, _backBufferView.ReleaseAndGetAddressOf());

    _viewPort.Width = static_cast<float>(width);
    _viewPort.Height = static_cast<float>(height);
    _viewPort.MinDepth = 0.0f;
    _viewPort.MaxDepth = 1.0f;
    _viewPort.TopLeftX = 0.0f;
    _viewPort.TopLeftY = 0.0f;
    _immContext->RSSetViewports(1u, &_viewPort);

    // Rasterizer State
    {
        D3D11_RASTERIZER_DESC rd {};
        rd.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        rd.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        rd.FrontCounterClockwise = false;
        rd.DepthClipEnable = true;

        _device->CreateRasterizerState(&rd, _rasterizerState.ReleaseAndGetAddressOf());
    }

    {
        D3D11_DEPTH_STENCIL_DESC desc {};
        desc.DepthEnable = TRUE;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_LESS;

        Microsoft::WRL::ComPtr<ID3D11DepthStencilState> ds_state;
        _device->CreateDepthStencilState(&desc, ds_state.ReleaseAndGetAddressOf());
        _immContext->OMSetDepthStencilState(ds_state.Get(), 1u);
        for (auto& context : _defContexts)
            context->OMSetDepthStencilState(ds_state.Get(), 1u);


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
        _device->CreateDepthStencilView(_ds.Get(), &desc_dsv, _depthStencilView.ReleaseAndGetAddressOf());
    }

    DirectX::XMStoreFloat4x4(&_proj, DirectX::XMMatrixPerspectiveFovLH(
        70.0f,
        static_cast<float>(width) / height,
        0.1f,
        250.0f
    ));

    auto& device {*_device.Get()};

    D3D11SceneMan::Load(device);

    //_obj.push_back(
    //    std::move(
    //        MakeShared<D3D11RenderObject>(
    //            device,
    //            D3DCamera::GetView(),
    //            GetProj(),
    //            D3D11SceneMan::ResolveScene("Zelda")
    //        )
    //    )
    //);

    _obj.push_back(
        std::move(
            MakeShared<D3D11RenderObject>(
                device,
                D3DCamera::GetView(),
                GetProj(),
                D3D11SceneMan::ResolveScene("Goblin")
            )
        )
    );

    //_obj.push_back(
    //    std::move(
    //        MakeShared<D3D11RenderObject>(
    //            device,
    //            D3DCamera::GetView(),
    //            GetProj(),
    //            D3D11SceneMan::ResolveScene("Cube")
    //        )
    //    )
    //);

    //_obj.push_back(
    //    std::move(
    //        MakeShared<D3D11RenderObject>(
    //            device,
    //            D3DCamera::GetView(),
    //            GetProj(),
    //            D3D11SceneMan::ResolveScene("IridescenceLamp")
    //        )
    //    )
    //);

    D3D11PSOLibrary::Init(device);

    //D3D11CubeMap::Init(device, GetProj());

    _globalCB.push_back(D3D11CamPosition{device, D3DCamera::GetPos()});
    _lights.push_back(D3D11LightDirectional{device, {0.0f, -0.5f, 1.0f}, {1.0f, 1.0f, 1.0f}});

    InitRenderStrategies();

    _sampler = std::make_unique<D3D11Sampler>(device, "yes");

    D3D11ImGuiRenderer::Init(_device.Get(), _defContexts[IMGUI_CONTEXT].Get());
}

Engine::Graphics::D3D11Core::~D3D11Core() {
    D3D11ImGuiRenderer::Shutdown();
}

void Engine::Graphics::D3D11Core::Update(float const dt, DirectX::XMMATRIX const& view) {
    D3DCamera::Update();

    D3D11ImGuiRenderer::ImGuiShowSceneEditWindow(_obj);
    for (auto const& obj : _obj)
        obj->Update(dt, view, DirectX::XMLoadFloat4x4(&_proj));

    for (auto& gcb : _globalCB)
        std::visit(UpdateConstantBuffer{}, gcb);

    for (auto& light : _lights)
        std::visit(UpdateConstantBuffer{}, light);

    //D3D11CubeMap::Update(dt, view, GetProj());
}

void Engine::Graphics::D3D11Core::AddScene() {
}

DirectX::XMMATRIX Engine::Graphics::D3D11Core::GetProj() {
    return DirectX::XMLoadFloat4x4(&_proj);
}

void Engine::Graphics::D3D11Core::BeginFrame() {
    //GRAPHICS_INFO("Begin Frame");

    //auto r {static_cast<float>(std::rand()) / RAND_MAX};
    //auto g {static_cast<float>(std::rand()) / RAND_MAX};
    //auto b {static_cast<float>(std::rand()) / RAND_MAX};
    //float clear_color[4] {r, g, b, 1.0f};
    static constexpr float clear_color[4] {0.0f, 0.0f, 0.0f, 1.0f};

    // first context clears the screen and buffers
    _defContexts[0]->RSSetState(_rasterizerState.Get());
    _defContexts[0]->RSSetViewports(1u, &_viewPort);
    _defContexts[0]->OMSetRenderTargets(1u, _backBufferView.GetAddressOf(), _depthStencilView.Get());
    _defContexts[0]->ClearRenderTargetView(_backBufferView.Get(), clear_color);
    _defContexts[0]->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f , 0u);

    for (auto i {1}; i != NUM_DEF_CONTEXTS; ++i) {
        _defContexts[i]->RSSetState(_rasterizerState.Get());
        _defContexts[i]->RSSetViewports(1u, &_viewPort);
        _defContexts[i]->OMSetRenderTargets(1u, _backBufferView.GetAddressOf(), _depthStencilView.Get());
    }

    auto& context {*_defContexts[0].Get()};

    _sampler->Bind(context, 0u);

    for (auto& gcb : _globalCB)
        std::visit(BindConstantBuffer{context}, gcb);

    for (auto& light : _lights)
        std::visit(BindConstantBuffer{context}, light);

    for (auto const& obj : _obj)
        obj->Render(context);

    //D3D11CubeMap::Render(context);
}

void Engine::Graphics::D3D11Core::EndFrame() {
    //GRAPHICS_INFO("End Frame");
    x_array<ID3D11CommandList*, NUM_DEF_CONTEXTS> cmd_lists {};
    for (auto i {0}; i != NUM_DEF_CONTEXTS; ++i)
        _defContexts[i]->FinishCommandList(FALSE, &cmd_lists[i]);

    for (auto i {0}; i != NUM_DEF_CONTEXTS; ++i)
        _immContext->ExecuteCommandList(cmd_lists[i], FALSE);

    for (auto i {0}; i != NUM_DEF_CONTEXTS; ++i)
        cmd_lists[i]->Release();

    _swapChain->Present(1u, 0u);
}
