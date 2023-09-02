#pragma once

#include "D3D11Cubemap.h"
#include "D3D11FullScreenFilter.h"
#include "Graphics/GUI/D3D11ImGuiRenderer.h"
#include "ConstantBuffer/D3D11ConstantBuffer.h"
#include "RenderObject/D3D11DefaultObject.h"
#include "RenderObject/D3D11ConcreteLight.h"
#include "RenderObject/D3D11MirrorObject.h"

namespace Engine::Graphics {
    struct D3D11RenderData {
        D3D11RenderData(ID3D11Device& device, ID3D11DeviceContext& context, DirectX::XMMATRIX proj)
            : _globalCB{device}, _cubemap{device, context, proj}, _fsFilter{device} {}

        x_vector<std::shared_ptr<D3D11DefaultObject>> _obj;
        x_vector<std::shared_ptr<D3D11ConcreteLight>> _concreteLights;
        x_vector<std::shared_ptr<D3D11MirrorObject>>  _mirrors;
        x_vector<D3D11ConstantBuffer> _ambDirLights;
        x_vector<D3D11ConstantBuffer> _pointLights;
        D3D11GlobalConstants          _globalCB;
        D3D11Cubemap                  _cubemap;
        D3D11FullScreenFilter         _fsFilter;

        inline void Update(float const dt, DirectX::XMMATRIX const& view, DirectX::XMMATRIX const& proj) {
            D3D11ImGuiRenderer::ImGuiShowSceneEditWindow(_obj);
            D3D11ImGuiRenderer::ImGuiShowConcreteLightEditWindow(_concreteLights);
            D3D11ImGuiRenderer::ImGuiShowMirrorEditWindow(_mirrors);

            for (auto const& obj : _obj)
                obj->Update(dt);
            for (auto& light : _concreteLights)
                light->Update(dt);
            for (auto& mirror : _mirrors)
                mirror->Update(dt);

            _globalCB.Update(dt);

            for (auto& light : _ambDirLights)
                std::visit(UpdateConstantBuffer{dt}, light);

            _cubemap.Update(dt);
        };
    };
}

