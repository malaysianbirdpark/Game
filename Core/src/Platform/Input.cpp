#include "pch.h"
#include "Input.h"

#include <imgui_impl_win32.h>

void Engine::Input::Init(HWND hWnd) {
    _hWnd = hWnd;

    _mouse = MakeUnique<DirectX::Mouse>();
    _mouse->SetWindow(hWnd);

    _keyboard = MakeUnique<DirectX::Keyboard>();

    PLATFORM_INFO("InputHanlder Initialized!");
}

DirectX::Keyboard::State Engine::Input::GetKeyboardState() {
    return _keyboard->GetState();
}

DirectX::Mouse::State Engine::Input::GetMouseState() {
    return _mouse->GetState();
}

std::optional<std::pair<int, int>> Engine::Input::ReadRawDelta() {
    if (_rawDeltaBuffer.empty())
        return std::nullopt;
    auto const d {_rawDeltaBuffer.front()};
    _rawDeltaBuffer.pop();
    return std::pair{d.x, d.y};
}

void Engine::Input::ToggleActivation(UINT flag) {
    if (_cursorEnabled) {
        if (flag) {
            ConfineCursor();
            HideCursor();
        }
        else {
            FreeCursor();
            ShowCursor();
        }
    }
}

void Engine::Input::KeyboardButtonDown() {
    if (!_cursorEnabled) {
        ConfineCursor();
        HideCursor();
    }
}

void Engine::Input::ProcessRawInput(UINT msg, WPARAM wParam, LPARAM lParam) {
    if (!_rawEnabled)
        return;

    UINT size;
    // first get the size of the input data
    if (GetRawInputData(
        reinterpret_cast<HRAWINPUT>(lParam),
        RID_INPUT,
        nullptr,
        &size,
        sizeof(RAWINPUTHEADER)) == -1) 
    {
        return;     
    }
    _rawBuffer.resize(size);
    if (GetRawInputData(
        reinterpret_cast<HRAWINPUT>(lParam),
        RID_INPUT,
        _rawBuffer.data(),
        &size,
        sizeof(RAWINPUTHEADER)) == -1) 
    {
        return;     
    }

    // process the data
    auto& ri {reinterpret_cast<RAWINPUT const&>(*_rawBuffer.data())};
    if (ri.header.dwType == RIM_TYPEMOUSE && ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY)
        _rawDeltaBuffer.push<RawMouseData>(ri.data.mouse.lLastX, ri.data.mouse.lLastY);
}

void Engine::Input::ToggleRaw() {
    if (_cursorEnabled) {
        DisableCursor();
        _rawEnabled = true;
    }
    else
    {
        EnableCursor(); 
        _rawEnabled = false;
    }
}

void Engine::Input::EnableCursor() {
    _cursorEnabled = true;
    ShowCursor();
    EnableImGuiMouse();
    FreeCursor();
}

void Engine::Input::DisableCursor() {
    _cursorEnabled = false;
    HideCursor();
    DisableImGuiMouse();
    ConfineCursor();
}

void Engine::Input::ClearKeyboard() {
    _keyboard->Reset();
}

void Engine::Input::ClearMouse() {
    _mouse->ResetScrollWheelValue();
}

void Engine::Input::ConfineCursor() {
    RECT r {};
    GetClientRect(_hWnd, &r);
    MapWindowPoints(_hWnd, nullptr, reinterpret_cast<POINT*>(&r), 2);
    ClipCursor(&r);
}

void Engine::Input::HideCursor() {
    while (::ShowCursor(FALSE) >= 0)
        ;
}

void Engine::Input::FreeCursor() {
    ClipCursor(nullptr);
}

void Engine::Input::ShowCursor() {
    while (::ShowCursor(TRUE) < 0)
        ;
}

void Engine::Input::EnableImGuiMouse() {
    ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void Engine::Input::DisableImGuiMouse() {
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}
