#include "pch.h"
#include "D3D11Core.h"

//TEMP
#include <random>

#include "D3D11Sampler.h"
#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"
#include "Graphics/D3D11/SceneGraph/D3D11SceneHolder.h"
#include "Graphics/D3D11/RenderStrategy/D3D11RenderStrategy.h"
#include "Graphics/D3D11/D3D11RenderObject.h"

#include "Graphics/D3DCamera.h"
#include "PipelineState/D3D11PSOLibrary.h"
#include "PipelineState/D3D11PipelineStateObject.h"

#include <imgui.h>

#include "D3D11Cubemap.h"
#include "D3D11FullScreenFilter.h"
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

    D3D_FEATURE_LEVEL feature_level[] {D3D_FEATURE_LEVEL_11_1};
    ::D3D11CreateDeviceAndSwapChain(
        nullptr,
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        swap_chain_flag,
        feature_level,
        1,
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

    UINT quality_level {};
    _device->CheckMultisampleQualityLevels(DXGI_FORMAT_R16G16B16A16_FLOAT, 8, &quality_level);

    D3D11_TEXTURE2D_DESC desc;
    static_cast<ID3D11Texture2D*>(_backBuffers.Get())->GetDesc(&desc);
    desc.MipLevels = desc.ArraySize = 1;
    desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.MiscFlags = 0;
    desc.CPUAccessFlags = 0;
    desc.SampleDesc.Count = 8;
    desc.SampleDesc.Quality = quality_level - 1;

    _device->CreateTexture2D(&desc, nullptr, _renderBuffer.ReleaseAndGetAddressOf());
    _device->CreateShaderResourceView(_renderBuffer.Get(), nullptr, _renderSRV.ReleaseAndGetAddressOf());
    _device->CreateRenderTargetView(_renderBuffer.Get(), nullptr, _renderRTV.ReleaseAndGetAddressOf());

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    _device->CreateTexture2D(&desc, nullptr, _resolvedBuffer.ReleaseAndGetAddressOf());
    _device->CreateShaderResourceView(_resolvedBuffer.Get(), nullptr, _resolvedSRV.ReleaseAndGetAddressOf());
    _device->CreateRenderTargetView(_resolvedBuffer.Get(), nullptr, _resolvedRTV.ReleaseAndGetAddressOf());

    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    _device->CreateTexture2D(&desc, nullptr, _imguiBuffer.ReleaseAndGetAddressOf());
    _device->CreateShaderResourceView(_imguiBuffer.Get(), nullptr, _imguiSRV.ReleaseAndGetAddressOf());
    _device->CreateRenderTargetView(_imguiBuffer.Get(), nullptr, _imguiRTV.ReleaseAndGetAddressOf());

    _device->CreateTexture2D(&desc, nullptr, _finalBuffer.ReleaseAndGetAddressOf());
    _device->CreateShaderResourceView(_finalBuffer.Get(), nullptr, _finalSRV.ReleaseAndGetAddressOf());
    _device->CreateRenderTargetView(_finalBuffer.Get(), nullptr, _finalRTV.ReleaseAndGetAddressOf());


    _viewPort.Width = static_cast<float>(width);
    _viewPort.Height = static_cast<float>(height);
    _viewPort.MinDepth = 0.0f;
    _viewPort.MaxDepth = 1.0f;
    _viewPort.TopLeftX = 0.0f;
    _viewPort.TopLeftY = 0.0f;
    _immContext->RSSetViewports(1u, &_viewPort);

    // Rasterizer State
    {
        _rasterizerState.resize(3);

        D3D11_RASTERIZER_DESC rd {};
        rd.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        rd.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        rd.FrontCounterClockwise = false;
        rd.DepthClipEnable = true;

        _device->CreateRasterizerState(&rd, _rasterizerState[0].ReleaseAndGetAddressOf());

        rd.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
        rd.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        rd.FrontCounterClockwise = false;
        rd.DepthClipEnable = true;

        _device->CreateRasterizerState(&rd, _rasterizerState[1].ReleaseAndGetAddressOf());

        rd.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        rd.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        rd.FrontCounterClockwise = false;
        rd.DepthClipEnable = false;

        _device->CreateRasterizerState(&rd, _rasterizerState[2].ReleaseAndGetAddressOf());
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
        descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        descDepth.SampleDesc.Count = 8;
        descDepth.SampleDesc.Quality = quality_level - 1;

        Microsoft::WRL::ComPtr<ID3D11Texture2D> _ds;
        _device->CreateTexture2D(&descDepth, nullptr, _ds.ReleaseAndGetAddressOf());

        D3D11_DEPTH_STENCIL_VIEW_DESC desc_dsv {};
        desc_dsv.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
        desc_dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
        desc_dsv.Texture2D.MipSlice = 0u;
        _device->CreateDepthStencilView(_ds.Get(), &desc_dsv, _depthStencilView.ReleaseAndGetAddressOf());
    }

    DirectX::XMStoreFloat4x4(&_proj, DirectX::XMMatrixPerspectiveFovLH(
        70.0f,
        static_cast<float>(width) / height,
        0.01f,
        250.0f
    ));

    auto& device {*_device.Get()};

    D3D11SceneHolder::Load(device);

    //_obj.push_back(
    //    std::move(
    //        MakeShared<D3D11RenderObject>(
    //            device,
    //            D3DCamera::GetView(),
    //            GetProj(),
    //            D3D11SceneHolder::ResolveScene("Zelda")
    //        )
    //    )
    //);

    //_obj.push_back(
    //    std::move(
    //        MakeShared<D3D11RenderObject>(
    //            device,
    //            D3DCamera::GetView(),
    //            GetProj(),
    //            D3D11SceneHolder::ResolveScene("Goblin")
    //        )
    //    )
    //);

    //_obj.push_back(
    //    std::move(
    //        MakeShared<D3D11RenderObject>(
    //            device,
    //            D3DCamera::GetView(),
    //            GetProj(),
    //            D3D11SceneHolder::ResolveScene("Cube")
    //        )
    //    )
    //);

    //_obj.push_back(
    //    std::move(
    //        MakeShared<D3D11RenderObject>(
    //            device,
    //            D3DCamera::GetView(),
    //            GetProj(),
    //            D3D11SceneHolder::ResolveScene("IridescenceLamp")
    //        )
    //    )
    //);

    //_obj.push_back(
    //    std::move(
    //        MakeShared<D3D11RenderObject>(
    //            device,
    //            D3DCamera::GetView(),
    //            GetProj(),
    //            D3D11SceneHolder::ResolveScene("Sphere")
    //        )
    //    )
    //);

    _obj.push_back(
        std::move(
            MakeShared<D3D11RenderObject>(
                device,
                D3DCamera::GetView(),
                GetProj(),
                D3D11SceneHolder::ResolveScene("DamagedHelmet")
            )
        )
    );

    D3D11PSOLibrary::Init(device);

    _cubemap = MakeUnique<D3D11Cubemap>(device, GetProj());
    _fullscreenFilter = MakeUnique<D3D11FullScreenFilter>(device);

    _globalCB.push_back(D3D11CamPosition{device, D3DCamera::GetPos()});
    _lights.push_back(D3D11LightDirectional{device, {0.0f, -0.5f, 1.0f}, {1.0f, 1.0f, 1.0f}});

    InitRenderStrategies();

    _sampler = MakeUnique<D3D11Sampler>(device, "global");

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

    _cubemap->Update(dt, view, GetProj());
}

void Engine::Graphics::D3D11Core::AddScene() {
}

void* Engine::Graphics::D3D11Core::GetFinalSRV() {
    return _finalSRV.Get();
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
    _defContexts[0]->RSSetState(_rasterizerState[_selectedRS].Get());
    _defContexts[0]->RSSetViewports(1u, &_viewPort);
    _defContexts[0]->OMSetRenderTargets(1u, _renderRTV.GetAddressOf(), _depthStencilView.Get());
    _defContexts[0]->ClearRenderTargetView(_renderRTV.Get(), clear_color);
    _defContexts[0]->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f , 0u);

    for (auto i {1}; i != NUM_DEF_CONTEXTS; ++i) {
        _defContexts[i]->RSSetState(_rasterizerState[0].Get());
        _defContexts[i]->RSSetViewports(1u, &_viewPort);
        _defContexts[i]->OMSetRenderTargets(1u, _imguiRTV.GetAddressOf(), nullptr);
    }

    auto& context {*_defContexts[0].Get()};

    _sampler->Bind(context, 0u);

    for (auto& gcb : _globalCB)
        std::visit(BindConstantBuffer{context}, gcb);

    for (auto& light : _lights)
        std::visit(BindConstantBuffer{context}, light);

    for (auto const& obj : _obj)
        obj->Render(context);

    //_cubemap->Render(context);

    _defContexts[0]->ResolveSubresource(_resolvedBuffer.Get(), 0, _renderBuffer.Get(), 0, DXGI_FORMAT_R16G16B16A16_FLOAT);
    context.OMSetRenderTargets(1u, _finalRTV.GetAddressOf(), nullptr);
    _defContexts[0]->ClearRenderTargetView(_finalRTV.Get(), clear_color);
    for (auto const& obj : _obj)
        obj->RenderTest(context, _resolvedSRV.GetAddressOf());

    //_fullscreenFilter->Render(context, _resolvedSRV.GetAddressOf(), _finalRTV.GetAddressOf());

    _defContexts[1]->ResolveSubresource(_backBuffers.Get(), 0, _imguiBuffer.Get(), 0, DXGI_FORMAT_R8G8B8A8_UNORM);
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
