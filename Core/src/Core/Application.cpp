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

        ProcessInput(dt);

        _gfx->BeginFrame();
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
        DirectX::XMFLOAT3 translate {};

        translate.x -= kbd.IsKeyDown(DirectX::Keyboard::Keys::A) * dt;
        translate.x += kbd.IsKeyDown(DirectX::Keyboard::Keys::D) * dt;

        translate.y += kbd.IsKeyDown(DirectX::Keyboard::Keys::R) * dt;
        translate.y -= kbd.IsKeyDown(DirectX::Keyboard::Keys::F) * dt;

        translate.z += kbd.IsKeyDown(DirectX::Keyboard::Keys::W) * dt;
        translate.z -= kbd.IsKeyDown(DirectX::Keyboard::Keys::S) * dt;

        Graphics::D3DCamera::Translate(translate);
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
