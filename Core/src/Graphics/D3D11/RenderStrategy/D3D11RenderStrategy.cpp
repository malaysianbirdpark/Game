#include "pch.h"
#include "D3D11RenderStrategy.h"

void Engine::Graphics::InitRenderStrategies() {
    D3D11SolidStrategy::Init();
    D3D11SolidTexStrategy::Init();
    D3D11SolidTexHeightStrategy::Init();
    D3D11PhongStrategy::Init();
    D3D11PhongTexStrategy::Init();
    D3D11PhongTexNormStrategy::Init();
    D3D11PhongTexNormSpecStrategy::Init();
    D3D11EnvironmentMappingNormalStrategy::Init();
    //D3D11EnvironmentMappingReflectStrategy::Init();
    //D3D11BasicIBLStrategy::Init();
    //D3D11BasicIBLTexturedStrategy::Init();
}
