#include "pch.h"
#include "Application.h"

#include "Platform/Platform.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/D3DCamera.h"
#include "Graphics/D3DSceneGraph.h"

#include <assimp/scene.h>

Engine::Core::Application::Application() {
    width = 800;
    height = 450;
    _platform = MakeUnique<Platform::Platform>(width, height);
    _gfx = MakeUnique<Graphics::GraphicsContext>(width, height, _platform->GetNativeWnd(), true);

    // camera
    _cam = MakeShared<Graphics::D3DCamera>();
}

Engine::Core::Application::~Application() {
}

int Engine::Core::Application::Run() {
    while (true) {
        if (auto const ecode {_platform->PumpMessage()}; ecode.has_value()) [[unlikely]]
            return *ecode;

        ProcessInput();
        Update(1.0f);
        Render();
    }
}

void Engine::Core::Application::ProcessInput() {
}

void Engine::Core::Application::Update(float const dt) {
}

void Engine::Core::Application::Render() {
    _gfx->Render();
}
