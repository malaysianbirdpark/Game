#include "pch.h"
#include "Platform.h"

#include <imgui.h>
#include <imgui_impl_win32.h>

#include "Input.h"

namespace Engine {
    Platform::WindowClass::WindowClass() noexcept
        : _name{"Window"}, hInst{GetModuleHandle(nullptr)}
    {
        WNDCLASSEX wc {};
        wc.cbSize = sizeof(wc);
        wc.style = CS_OWNDC;
        wc.lpfnWndProc = HandleMsgSetup;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = GetInstance();
        wc.hIcon = nullptr;
        wc.hCursor = nullptr;
        wc.hbrBackground = nullptr;
        wc.lpszMenuName = nullptr;
        wc.lpszClassName = GetName();
        wc.hIconSm = nullptr;
        RegisterClassEx(&wc);
    }

    Platform::WindowClass::~WindowClass() {
    }

    void Platform::WindowClass::Shutdown() {
        UnregisterClass(_name.c_str(), GetInstance());
    }

    void Platform::Init(int const width, int const height, char const* name) {
        _wndData.wndClass = new WindowClass{};

        _wndData.width = width;
        _wndData.height = width;

        // calculate window size base on desired client region size 
        RECT wr;
        wr.left = 100;
        wr.right = width + wr.left;
        wr.top = 100;
        wr.bottom = height + wr.top;
        if (!AdjustWindowRect(&wr, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU/*, | WS_THICKFRAME*/, FALSE)) {
            //throw LAST_EXCEPT();
        }

        // create window & get hWnd
        Platform p {};
        _wndData.hWnd = CreateWindow(
            _wndData.wndClass->GetName(),
            name,
            WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, //| WS_THICKFRAME,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            wr.right - wr.left,
            wr.bottom - wr.top,
            nullptr,
            nullptr,
            _wndData.wndClass->GetInstance(),
            &p
        );
        /*
        if (_wndData.hWnd == nullptr) {
            throw LAST_EXCEPT();
        } 
        */

        Input::Init(_wndData.hWnd);

        ShowWindow(_wndData.hWnd, SW_SHOWDEFAULT);
    }

    void Platform::Shutdown() {
        PLATFORM_INFO("Shutting down Platform!");
        _wndData.wndClass->Shutdown();
        delete _wndData.wndClass;
        DestroyWindow(_wndData.hWnd);
    }

    std::optional<int> Platform::PumpMessage() {
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) [[unlikely]]
                return msg.wParam;

            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return std::nullopt;
    }

    void Platform::SetWindowSize(int width, int height) {
        _wndData.width = width;
        _wndData.height = height;
    }

    void Platform::SetViewportSize(int width, int height) {
    }

    LRESULT CALLBACK Platform::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
        // when a window is first created
        if (msg == WM_NCCREATE) {
            // extract ptr to window class from create data
            CREATESTRUCT const* const pCreate {reinterpret_cast<CREATESTRUCT*>(lParam)};
            Platform* pWnd {static_cast<Platform*>(pCreate->lpCreateParams)};
            // set WinAPI-managed user data to store ptr to window class
            SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
            // set message proc to normal (non-setup) handler now that setup is finished
            SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Platform::HandleMsgThunk));
            // forward message to window class handler
            return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }

    LRESULT CALLBACK Platform::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
        Platform* pWnd {reinterpret_cast<Platform*>(GetWindowLongPtr(hWnd, GWLP_USERDATA))};
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }

    LRESULT Platform::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
        if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
            return true;

        //if (GUI::ImGuiMan::IsImguiEnabled()) [[likely]] {
        if (true) [[likely]] {
            switch (msg) {
                /* Window Resizing is currently disabled
                // WINDOW RESIZE
                case WM_SIZE: {
                    auto const w {LOWORD(lParam)};
                    auto const h {HIWORD(lParam)};
                    _SetWindowSize(w, h);
                }
                */
                case WM_CLOSE:
                    PostQuitMessage(0);
                    return 0;
                case WM_KILLFOCUS:
                    Input::ClearKeyboard();
                    break;
                // KEYBOARD
                case WM_KEYDOWN:
                case WM_SYSKEYDOWN:
                case WM_KEYUP:
                case WM_SYSKEYUP:
                case WM_CHAR:
                    DirectX::Keyboard::ProcessMessage(msg, wParam, lParam);
                    break;
                case WM_MENUCHAR:
                    return MAKELRESULT(0, MNC_CLOSE);
                // MOUSE
                case WM_ACTIVATE:
            case WM_ACTIVATEAPP:
                    Input::ToggleActivation(wParam & WA_ACTIVE);
                    DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
                    break; 
                case WM_LBUTTONDOWN:
                    SetForegroundWindow(hWnd);
                    Input::KeyboardButtonDown();
                    DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
                    break; 
                case WM_MOUSEMOVE:
                case WM_LBUTTONUP:
                case WM_RBUTTONDOWN:
                case WM_RBUTTONUP:
                case WM_MBUTTONDOWN:
                case WM_MBUTTONUP:
                case WM_MOUSEWHEEL:
                case WM_XBUTTONDOWN:
                case WM_XBUTTONUP:
                case WM_MOUSEHOVER:
                    DirectX::Mouse::ProcessMessage(msg, wParam, lParam);
                    break; 
                case WM_MOUSEACTIVATE:
                    return MA_ACTIVATEANDEAT;
                /*** RAW MOUSE INPUT ***/
                case WM_INPUT:
                    Input::ProcessRawInput(msg, wParam, lParam);
                    break; 
            }
        }
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}
