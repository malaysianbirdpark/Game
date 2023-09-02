#include "pch.h"
#include "Application.h"

#include <imgui.h>

#include "Platform/Platform.h"
#include "Platform/Input.h"
#include "Graphics/D3DCamera.h"
#include "Graphics/GUI/D3D11ImGuiRenderer.h"

#include "Utils/Clk.h"

#include "Graphics/D3D11/D3D11Core.h"

Engine::Core::Application::Application() {
    width = 1600;
    height = 900;
    Platform::Init(width, height);

    // camera
    Graphics::D3DCamera::Init();
    _gfx = std::make_unique<Graphics::D3D11Core>(width, height, Platform::GetNativeWnd(), true);

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

        ProcessInput(dt);
        _gfx->BeginFrame();
        Update(dt);
        _gfx->EndFrame();
    }
}

void Engine::Core::Application::ProcessInput(float const dt) {
    auto const kbd_tracker {Input::GetKeyboardState()};
    //auto const mouse {Input::GetMouseState()};

    if (kbd_tracker.pressed.Escape)
        Input::ToggleRaw();

    auto const kbd_state {kbd_tracker.GetLastState()};
    if (!Input::_cursorEnabled) {
        DirectX::XMFLOAT3 translate {};
        translate.x += static_cast<float>(kbd_state.D) * dt;
        translate.x -= static_cast<float>(kbd_state.A) * dt;
        translate.y += static_cast<float>(kbd_state.R) * dt;
        translate.y += static_cast<float>(kbd_state.F) * dt;
        translate.z += static_cast<float>(kbd_state.W) * dt;
        translate.z -= static_cast<float>(kbd_state.S) * dt;

        Graphics::D3DCamera::Translate(translate);

        while (auto const raw_delta {Input::ReadRawDelta()})
            Graphics::D3DCamera::Rotate(raw_delta.value().first * dt, raw_delta.value().second * dt);
    }

    //Input::ClearKeyboard();
    //Input::ClearMouse();
}

void Engine::Core::Application::Update(float const dt) {
    _gfx->Update(dt);
}
