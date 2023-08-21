#include "pch.h"
#include "Application.h"

#include "Platform/Platform.h"
#include "Platform/Input.h"
#include "Graphics/GraphicsContext.h"
#include "Graphics/D3DCamera.h"
#include "Graphics/GUI/D3D11ImGuiRenderer.h"

#include "Utils/Clk.h"

Engine::Core::Application::Application() {
    width = 800;
    height = 450;
    Platform::Init(width, height);
    _gfx = MakeUnique<Graphics::GraphicsContext>(width, height, Platform::GetNativeWnd(), true);

    // camera
    Graphics::D3DCamera::Init();

    Clk::Init();

    // Load 3D models
}

Engine::Core::Application::~Application() {
    Platform::Shutdown();
}

int Engine::Core::Application::Run() {
    while (true) {
        if (auto const ecode {Platform::PumpMessage()}; ecode.has_value()) [[unlikely]]
            return *ecode;

        auto const dt {Clk::Mark()};


        _gfx->BeginFrame();
        ProcessInput(dt);
        Update(dt);
        _gfx->EndFrame();
    }
}

void Engine::Core::Application::ProcessInput(float const dt) {
    auto const kbd {Input::GetKeyboardState()};
    auto const mouse {Input::GetMouseState()};

    if (kbd.Escape)
        Input::ToggleRaw();

    if (!Input::_cursorEnabled) {
        if (kbd.D)
            Graphics::D3DCamera::Translate({dt, 0.0f, 0.0f});
        if (kbd.A)
            Graphics::D3DCamera::Translate({-dt, 0.0f, 0.0f});
        if (kbd.R)
            Graphics::D3DCamera::Translate({0.0f, dt, 0.0f});
        if (kbd.F)
            Graphics::D3DCamera::Translate({0.0f, -dt, 0.0f});
        if (kbd.W)
            Graphics::D3DCamera::Translate({0.0f, 0.0f, dt});
        if (kbd.S)
            Graphics::D3DCamera::Translate({0.0f, 0.0f, -dt});
    }

    while (auto const raw_delta {Input::ReadRawDelta()}) {
        if (!Input::_cursorEnabled) 
            Graphics::D3DCamera::Rotate(raw_delta.value().first * dt, raw_delta.value().second * dt);
    }

    Input::ClearKeyboard();
    Input::ClearMouse();
}

void Engine::Core::Application::Update(float const dt) {
    _gfx->Update(dt);
}
