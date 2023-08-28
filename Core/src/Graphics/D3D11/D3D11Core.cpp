#include "pch.h"
#include "D3D11Core.h"

#include "D3D11RenderData.h"
#include "D3D11RenderCommand.h"

#include "Graphics/D3D11/SceneGraph/D3D11SceneGraph.h"
#include "Graphics/D3D11/SceneGraph/D3D11SceneHolder.h"
#include "Graphics/D3D11/RenderStrategy/D3D11RenderStrategy.h"
#include "Graphics/D3D11/RenderObject/D3D11RenderObject.h"

#include "Graphics/D3DCamera.h"
#include "PipelineState/D3D11PSOLibrary.h"
#include "PipelineState/D3D11PipelineStateObject.h"

#include <imgui.h>

#include "Graphics/GUI/D3D11ImGuiRenderer.h"

#include "Graphics/D3D11/ConstantBuffer/D3D11GlobalConstants.h"
#include "Graphics/D3D11/ConstantBuffer/D3D11DirectionalLight.h"

#include "ConstantBuffer/D3D11ConstantBuffer.h"
#include "ConstantBuffer/D3D11PointLight.h"
#include "RenderObject/D3D11ConcreteLight.h"

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

    DirectX::XMStoreFloat4x4(&_proj, DirectX::XMMatrixPerspectiveFovLH(
        70.0f,
        static_cast<float>(width) / height,
        0.01f,
        250.0f
    ));

    auto& device {*_device.Get()};

    D3D11RenderCommand::Init(device, *_immContext.Get(), *_swapChain.Get(), width, height);

    D3D11SceneHolder::Load(device);
    D3D11PSOLibrary::Init(device);

    InitRenderStrategies();
    InitData();
}

Engine::Graphics::D3D11Core::~D3D11Core() {
    D3D11ImGuiRenderer::Shutdown();
}

void Engine::Graphics::D3D11Core::Update(float const dt) {
    D3DCamera::Update();
    _data->Update(dt, D3DCamera::GetView(), GetProj());
}

void Engine::Graphics::D3D11Core::AddScene() {
}

DirectX::XMMATRIX Engine::Graphics::D3D11Core::GetProj() {
    return DirectX::XMLoadFloat4x4(&_proj);
}

void Engine::Graphics::D3D11Core::InitData() {
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


    auto& device {*_device.Get()};

    _data = MakeUnique<D3D11RenderData>(device, GetProj());

    _data->_obj.push_back(
        std::move(
            MakeShared<D3D11RenderObject>(
                device,
                D3DCamera::GetView(),
                GetProj(),
                D3D11SceneHolder::ResolveScene("Sphere")
            )
        )
    );

    //_data->_obj.push_back(
    //    std::move(
    //        MakeShared<D3D11RenderObject>(
    //            device,
    //            D3DCamera::GetView(),
    //            GetProj(),
    //            D3D11SceneHolder::ResolveScene("DamagedHelmet")
    //        )
    //    )
    //);

    _data->_concreteLights.push_back(
            MakeShared<D3D11ConcreteLight>(
                device,
                D3DCamera::GetView(),
                GetProj(),
                ConcreteLightType::PointLight
            )
    );
    _data->_concreteLights.push_back(
            MakeShared<D3D11ConcreteLight>(
                device,
                D3DCamera::GetView(),
                GetProj(),
                ConcreteLightType::PointLight
            )
    );

    _data->_ambDirLights.push_back(D3D11HemisphericAmbientLight{device});
    _data->_ambDirLights.push_back(D3D11DirectionalLight{device, {0.0f, -0.5f, 1.0f}, {1.0f, 1.0f, 1.0f}});
}

void Engine::Graphics::D3D11Core::BeginFrame() {
    D3D11RenderCommand::BeginForward(_data.get());
    D3D11ImGuiRenderer::BeginFrame();
}

void Engine::Graphics::D3D11Core::EndFrame() {
    D3D11ImGuiRenderer::EndFrame();

    x_array<ID3D11CommandList*, 3> cmd_lists {};
    D3D11RenderCommand::EndForward(cmd_lists);

    for (auto i {0}; i != 3; ++i)
        _immContext->ExecuteCommandList(cmd_lists[i], FALSE);

    for (auto i {0}; i != 3; ++i)
        cmd_lists[i]->Release();

    _swapChain->Present(1u, 0u);
}
