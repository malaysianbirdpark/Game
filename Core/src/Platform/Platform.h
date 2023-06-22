#pragma once

namespace Engine::Platform {
    class Platform {
    public:
        class WindowClass {
        public:
            WindowClass() noexcept;
            ~WindowClass();

            void Shutdown();

            WindowClass(WindowClass const&) = delete;
            WindowClass& operator=(WindowClass const&) = delete;

            [[nodiscard]] char const* GetName() const { return _name.c_str(); }
            [[nodiscard]] HINSTANCE GetInstance() const { return hInst; }
        private:
            std::string _name;
            HINSTANCE hInst;
        };
    public:
        Platform(int const width, int const height, char const* name = "window");
        virtual ~Platform();

        std::optional<int> PumpMessage();

        [[nodiscard]] HWND GetNativeWnd() { return win32Data.hWnd; }
        [[nodiscard]] int GetWidth() const { return win32Data.width; }
        [[nodiscard]] int GetHeight() const { return win32Data.height; }

        void SetWindowSize(int width, int height);
        void SetViewportSize(int width, int height);
    private:
        static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
        LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
    private:
        void EnableImGuiMouse();
        void DisableImGuiMouse();
    private:
        struct win32_data {
            int width {0};
            int height {0};
            WindowClass* wndClass;
            HWND hWnd {nullptr};
        } win32Data;
    };
}

