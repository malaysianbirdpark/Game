#include "pch.h"
#include "D3D11RenderStrategy.h"

void Engine::Graphics::InitRenderStrategies() {
    D3D11SolidStrategy::Init();
    D3D11PhongStrategy::Init();
    D3D11UnrealPBRStrategy::Init();
    //D3D11EnvironmentMappingReflectStrategy::Init();
    //D3D11BasicIBLStrategy::Init();
    //D3D11BasicIBLTexturedStrategy::Init();
}
