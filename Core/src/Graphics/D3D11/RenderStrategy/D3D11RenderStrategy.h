#pragma once

#include "D3D11SolidStrategy.h"
#include "D3D11SolidTexturedStrategy.h"
#include "D3D11PhongStrategy.h"
#include "D3D11PhongTexStrategy.h"
#include "D3D11EnvironmentMappingReflectStrategy.h"
#include "D3D11EnvironmentMappingNormalStrategy.h"
#include "D3D11BasicIBLStrategy.h"
#include "D3D11BasicIBLTexturedStrategy.h"

namespace Engine::Graphics {
    // Initialize Render Strategies
    void InitRenderStrategies();

    inline x_vector<std::function<void(ID3D11DeviceContext&, D3D11Mesh const&, D3D11Material const&)>> renderStrategyTable {
        &D3D11SolidStrategy::Render,
        &D3D11SolidTexturedStrategy::Render,
        &D3D11PhongStrategy::Render,
        &D3D11PhongTexStrategy::Render,
        &D3D11EnvironmentMappingNormalStrategy::Render,
        &D3D11EnvironmentMappingReflectStrategy::Render,
        &D3D11BasicIBLStrategy::Render,
        &D3D11BasicIBLTexturedStrategy::Render,
    };
}

