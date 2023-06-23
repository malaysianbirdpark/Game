#include "pch.h"
#include "Application.h"

#include "Platform/Platform.h"
#include "Graphics/GraphicsContext.h"

Engine::Core::Application::Application() {
    width = 800;
    height = 450;
    p_platform = MakeUnique<Platform::Platform>(width, height);
    p_gfx = MakeUnique<Graphics::GraphicsContext>(width, height, p_platform->GetNativeWnd(), true);
}

Engine::Core::Application::~Application() {
}

int Engine::Core::Application::Run() {
    while (true) {
        if (auto const ecode {p_platform->PumpMessage()}; ecode.has_value()) [[unlikely]]
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
    p_gfx->Render();
}
