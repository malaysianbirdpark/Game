#include "pch.h"
#include "D3D11RenderCommand.h"

#include "D3D11RenderData.h"
#include "Graphics/GUI/D3D11ImGuiRenderer.h"
#include "ConstantBuffer/D3D11ConstantBuffer.h"

#include <directxtk/PostProcess.h>

void Engine::Graphics::D3D11RenderCommand::Init(ID3D11Device& device, IDXGISwapChain& swap_chain, int width, int height) {
    swap_chain.GetBuffer(0u, IID_PPV_ARGS(_backBuffers.ReleaseAndGetAddressOf()));
    device.CreateRenderTargetView(_backBuffers.Get(), nullptr, _backBufferView.ReleaseAndGetAddressOf());

    for (auto& context : _defContexts) {
        device.CreateDeferredContext(
            0u,
            context.ReleaseAndGetAddressOf()
        );
    }

    _viewPort.Width = static_cast<float>(width);
    _viewPort.Height = static_cast<float>(height);
    _viewPort.MinDepth = 0.0f;
    _viewPort.MaxDepth = 1.0f;
    _viewPort.TopLeftX = 0.0f;
    _viewPort.TopLeftY = 0.0f;

    UINT quality_level {};
    device.CheckMultisampleQualityLevels(DXGI_FORMAT_R16G16B16A16_FLOAT, 8, &quality_level);

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

    device.CreateTexture2D(&desc, nullptr, _renderBuffer.ReleaseAndGetAddressOf());
    device.CreateShaderResourceView(_renderBuffer.Get(), nullptr, _renderSRV.ReleaseAndGetAddressOf());
    device.CreateRenderTargetView(_renderBuffer.Get(), nullptr, _renderRTV.ReleaseAndGetAddressOf());

    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    device.CreateTexture2D(&desc, nullptr, _intermediateBuffer.ReleaseAndGetAddressOf());
    device.CreateShaderResourceView(_intermediateBuffer.Get(), nullptr, _intermediateSRV.ReleaseAndGetAddressOf());
    device.CreateRenderTargetView(_intermediateBuffer.Get(), nullptr, _intermediateRTV.ReleaseAndGetAddressOf());

    device.CreateTexture2D(&desc, nullptr, _resolvedBuffer.ReleaseAndGetAddressOf());
    device.CreateShaderResourceView(_resolvedBuffer.Get(), nullptr, _resolvedSRV.ReleaseAndGetAddressOf());
    device.CreateRenderTargetView(_resolvedBuffer.Get(), nullptr, _resolvedRTV.ReleaseAndGetAddressOf());

    desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    device.CreateTexture2D(&desc, nullptr, _finalBuffer.ReleaseAndGetAddressOf());
    device.CreateShaderResourceView(_finalBuffer.Get(), nullptr, _finalSRV.ReleaseAndGetAddressOf());
    device.CreateRenderTargetView(_finalBuffer.Get(), nullptr, _finalRTV.ReleaseAndGetAddressOf());

    device.CreateTexture2D(&desc, nullptr, _imguiBuffer.ReleaseAndGetAddressOf());
    device.CreateShaderResourceView(_imguiBuffer.Get(), nullptr, _imguiSRV.ReleaseAndGetAddressOf());
    device.CreateRenderTargetView(_imguiBuffer.Get(), nullptr, _imguiRTV.ReleaseAndGetAddressOf());

    D3D11ImGuiRenderer::Init(_defContexts[IMGUI_CONTEXT].Get());

    InitRasterizerState(device);
    InitDepthStencil(device, width, height, quality_level);
    InitSamplers(device);
}

void Engine::Graphics::D3D11RenderCommand::BeginForward(D3D11RenderData* data) {
    SetContextsStates();

    // For Ambient and Directional Light (Both are unique)
    {
        auto& context {*_defContexts[0].Get()};

        context.PSSetSamplers(0u, 1u, _samplerWrap.GetAddressOf());
        context.PSSetSamplers(1u, 1u, _samplerClamp.GetAddressOf());
        context.VSSetSamplers(0u, 1u, _samplerWrap.GetAddressOf());

        data->_globalCB.SetLightType(LightType::Amb_Dir);
        data->_globalCB.Bind(context);
        for (auto& light : data->_ambDirLights)
            std::visit(BindConstantBuffer{context}, light);
        for (auto const& obj : data->_obj)
            obj->Render(context);
    }
    // ---------------------------------------------------

    // For Point Lights
    {
        auto& context {*_defContexts[0].Get()};

        context.PSSetShaderResources(14u, 1u, _intermediateSRV.GetAddressOf());

        data->_globalCB.SetLightType(LightType::Point);
        data->_globalCB.Bind(context);
        for (auto& light : data->_concreteLights) {
            context.ResolveSubresource(_intermediateBuffer.Get(), 0, _renderBuffer.Get(), 0, DXGI_FORMAT_R16G16B16A16_FLOAT);
            context.ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f , 0u);

            std::visit(BindConstantBuffer{context}, light->GetCB());

            for (auto const& obj : data->_obj)
                obj->Render(context);
        }
    }
    // ---------------------------------------------------

    // Draw Concrete Lights
    {
        auto& context {*_defContexts[1].Get()};
        context.PSSetSamplers(0u, 1u, _samplerWrap.GetAddressOf());
        context.PSSetSamplers(1u, 1u, _samplerClamp.GetAddressOf());
        context.VSSetSamplers(0u, 1u, _samplerWrap.GetAddressOf());

        data->_globalCB.Bind(context);
        for (auto& light : data->_concreteLights)
            light->Render(context);
    }
    // ---------------------------------------------------

    // Draw Cubemap
    {
        auto& context {*_defContexts[2].Get()};

        context.PSSetSamplers(0u, 1u, _samplerWrap.GetAddressOf());
        context.PSSetSamplers(1u, 1u, _samplerClamp.GetAddressOf());
        context.VSSetSamplers(0u, 1u, _samplerWrap.GetAddressOf());

        data->_globalCB.Bind(context);
        data->_cubemap.Render(context);
    }
    // ---------------------------------------------------

    // Draw Mirror
    //{
    //    auto& context {*_defContexts[3].Get()};
    //    data->_globalCB.Bind(context);

    //    context.OMSetDepthStencilState(_dsWriteMask.Get(), 0xff);
    //    context.OMSetBlendState(_bsMirror.Get(), nullptr, 0xFFFFFFFFu);

    //    for (auto& mirror : data->_mirrors)
    //        mirror->Render(context);

    //    context.ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0.0f);
    //    context.RSSetState(_rasterizerState[2].Get());
    //    context.OMSetDepthStencilState(_dsReadMask.Get(), 0xff);

    //    for (auto& mirror : data->_mirrors) {
    //        auto const plane {mirror->GetMirrorPlane()};
    //        data->_globalCB.Reflect(plane);
    //        data->_globalCB.Bind(context);
    //        for (auto& obj : data->_obj)
    //            obj->Render(context);
    //    }

    //    context.RSSetState(_rasterizerState[0].Get());
    //    context.OMSetDepthStencilState(_dsDefault.Get(), 1.0f);
    //    context.OMSetBlendState(_bsMirror.Get(), nullptr, 0xFFFFFFFFu);
    //    for (auto& mirror : data->_mirrors)
    //        mirror->Render(context);
    //}

    _defContexts[static_cast<int>(DefContext::PostProcess)]->ResolveSubresource(_resolvedBuffer.Get(), 0, _renderBuffer.Get(), 0, DXGI_FORMAT_R16G16B16A16_FLOAT);
    data->_fsFilter.Render(_resolvedSRV.GetAddressOf(), _finalRTV.GetAddressOf());
    _defContexts[IMGUI_CONTEXT]->ResolveSubresource(_backBuffers.Get(), 0, _imguiBuffer.Get(), 0, DXGI_FORMAT_R8G8B8A8_UNORM);
}

void Engine::Graphics::D3D11RenderCommand::EndForward(x_array<ID3D11CommandList*, NUM_DEF_CONTEXTS>& cmd_lists) {
    for (auto i {0}; i != NUM_DEF_CONTEXTS; ++i)
        _defContexts[i]->FinishCommandList(FALSE, &cmd_lists[i]);
}

void* Engine::Graphics::D3D11RenderCommand::GetFinalSRV() {
    return _finalSRV.Get();
}

int32_t& Engine::Graphics::D3D11RenderCommand::RasterizerState() {
    return _selectedRS;
}

ID3D11DeviceContext& Engine::Graphics::D3D11RenderCommand::GetContext(DefContext context) {
    return *_defContexts[static_cast<int>(context)].Get();
}

void Engine::Graphics::D3D11RenderCommand::SetContextsStates() {
    static constexpr float clear_color[4] {0.0f, 0.0f, 0.0f, 1.0f};

    _defContexts[0]->RSSetViewports(1u, &_viewPort);
    _defContexts[0]->RSSetState(_rasterizerState[_selectedRS].Get());
    _defContexts[0]->OMSetDepthStencilState(_dsDefault.Get(), 0u);
    _defContexts[0]->OMSetBlendState(_bsDefault.Get(), nullptr, 0xFFFFFFFFu);
    _defContexts[0]->OMSetRenderTargets(1u, _renderRTV.GetAddressOf(), _depthStencilView.Get());
    _defContexts[0]->ClearRenderTargetView(_renderRTV.Get(), clear_color);
    _defContexts[0]->ClearDepthStencilView(_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f , 0u);

    _defContexts[1]->RSSetViewports(1u, &_viewPort);
    _defContexts[1]->RSSetState(_rasterizerState[_selectedRS].Get());
    _defContexts[1]->OMSetDepthStencilState(_dsDefault.Get(), 0u);
    _defContexts[1]->OMSetBlendState(_bsDefault.Get(), nullptr, 0xFFFFFFFFu);
    _defContexts[1]->OMSetRenderTargets(1u, _renderRTV.GetAddressOf(), _depthStencilView.Get());

    _defContexts[2]->RSSetViewports(1u, &_viewPort);
    _defContexts[2]->RSSetState(_rasterizerState[_selectedRS].Get());
    _defContexts[2]->OMSetDepthStencilState(_dsDefault.Get(), 0u);
    _defContexts[2]->OMSetBlendState(_bsDefault.Get(), nullptr, 0xFFFFFFFFu);
    _defContexts[2]->OMSetRenderTargets(1u, _renderRTV.GetAddressOf(), _depthStencilView.Get());

    _defContexts[3]->RSSetViewports(1u, &_viewPort);
    _defContexts[3]->RSSetState(_rasterizerState[_selectedRS].Get());
    _defContexts[3]->OMSetRenderTargets(1u, _renderRTV.GetAddressOf(), _depthStencilView.Get());

    _defContexts[4]->RSSetViewports(1u, &_viewPort);
    _defContexts[4]->RSSetState(_rasterizerState[0].Get());
    _defContexts[4]->OMSetDepthStencilState(_dsDefault.Get(), 0u);
    _defContexts[4]->OMSetBlendState(_bsDefault.Get(), nullptr, 0xFFFFFFFFu);
    _defContexts[4]->OMSetRenderTargets(1u, _renderRTV.GetAddressOf(), _depthStencilView.Get());

    _defContexts[IMGUI_CONTEXT]->RSSetViewports(1u, &_viewPort);
    _defContexts[IMGUI_CONTEXT]->RSSetState(_rasterizerState[0].Get());
    _defContexts[IMGUI_CONTEXT]->OMSetDepthStencilState(_dsDefault.Get(), 0u);
    _defContexts[IMGUI_CONTEXT]->OMSetRenderTargets(1u, _imguiRTV.GetAddressOf(), nullptr);
}

void Engine::Graphics::D3D11RenderCommand::InitRasterizerState(ID3D11Device& device) {
    _rasterizerState.resize(3);

    D3D11_RASTERIZER_DESC rd {};

    // Default Solid
    {
        rd.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        rd.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        rd.FrontCounterClockwise = false;
        rd.DepthClipEnable = true;
        rd.MultisampleEnable = true;

        device.CreateRasterizerState(&rd, _rasterizerState[0].ReleaseAndGetAddressOf());
    }

    // Default Wire
    {
        rd.FillMode = D3D11_FILL_MODE::D3D11_FILL_WIREFRAME;
        rd.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        rd.FrontCounterClockwise = false;
        rd.DepthClipEnable = true;
        rd.MultisampleEnable = true;

        device.CreateRasterizerState(&rd, _rasterizerState[1].ReleaseAndGetAddressOf());
    }

    // CCW Solid
    {
        rd.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
        rd.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
        rd.FrontCounterClockwise = true;
        rd.DepthClipEnable = true;
        rd.MultisampleEnable = true;

        device.CreateRasterizerState(&rd, _rasterizerState[2].ReleaseAndGetAddressOf());
    }
}

void Engine::Graphics::D3D11RenderCommand::InitDepthStencil(ID3D11Device& device, int width, int height, UINT quality_level) {
    // default
    {
        D3D11_DEPTH_STENCIL_DESC desc {CD3D11_DEPTH_STENCIL_DESC{CD3D11_DEFAULT{}}};

        device.CreateDepthStencilState(&desc, _dsDefault.ReleaseAndGetAddressOf());
    }

    // write stencil
    {
        D3D11_DEPTH_STENCIL_DESC desc {CD3D11_DEPTH_STENCIL_DESC{CD3D11_DEFAULT{}}};
        desc.DepthEnable = TRUE;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        desc.DepthFunc = D3D11_COMPARISON_LESS;
        desc.StencilEnable = TRUE;
        desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
        desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
        desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;         // depth-stencil both failed
        desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP; // stencil: passed, depth: failed
        desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;      // depth-stencil both passed
        desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

        device.CreateDepthStencilState(&desc, _dsWriteMask.ReleaseAndGetAddressOf());
    }
    
    // read stencil
    {
        D3D11_DEPTH_STENCIL_DESC desc {CD3D11_DEPTH_STENCIL_DESC{CD3D11_DEFAULT{}}};
        desc.DepthEnable = TRUE;
        desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        desc.StencilEnable = TRUE;
        desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
        desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
        desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        desc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;

        device.CreateDepthStencilState(&desc, _dsReadMask.ReleaseAndGetAddressOf());
    }

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
    device.CreateTexture2D(&descDepth, nullptr, _ds.ReleaseAndGetAddressOf());

    D3D11_DEPTH_STENCIL_VIEW_DESC desc_dsv {};
    desc_dsv.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    desc_dsv.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
    desc_dsv.Texture2D.MipSlice = 0u;
    device.CreateDepthStencilView(_ds.Get(), &desc_dsv, _depthStencilView.ReleaseAndGetAddressOf());
}

void Engine::Graphics::D3D11RenderCommand::InitSamplers(ID3D11Device& device) {
    {
        D3D11_SAMPLER_DESC sd {};
        sd.Filter = D3D11_FILTER_ANISOTROPIC;
        sd.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
        sd.MaxAnisotropy = D3D11_REQ_MAXANISOTROPY;
        sd.MipLODBias = 0.0f;
        sd.MinLOD = 0.0f;
        sd.MaxLOD = D3D11_FLOAT32_MAX;

        device.CreateSamplerState(&sd, _samplerWrap.ReleaseAndGetAddressOf());
    }

    {
        D3D11_SAMPLER_DESC sd {};
        sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
        sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;

        device.CreateSamplerState(&sd, _samplerClamp.ReleaseAndGetAddressOf());
    }
}

void Engine::Graphics::D3D11RenderCommand::InitBlendState(ID3D11Device& device) {
    // Default
    {
        D3D11_BLEND_DESC desc {};
        desc.AlphaToCoverageEnable = true;
        desc.IndependentBlendEnable = false;

        desc.RenderTarget[0].BlendEnable = true;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

        desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

        device.CreateBlendState(&desc, _bsMirror.ReleaseAndGetAddressOf());
    }

    // Mirror
    {
        D3D11_BLEND_DESC desc {};
        desc.AlphaToCoverageEnable = true;
        desc.IndependentBlendEnable = false;

        desc.RenderTarget[0].BlendEnable = true;
        desc.RenderTarget[0].SrcBlend = D3D11_BLEND_INV_BLEND_FACTOR;;
        desc.RenderTarget[0].DestBlend = D3D11_BLEND_BLEND_FACTOR;
        desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

        desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

        desc.RenderTarget[0].RenderTargetWriteMask = 0;

        device.CreateBlendState(&desc, _bsMirror.ReleaseAndGetAddressOf());
    }
}
