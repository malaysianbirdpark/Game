#pragma once

namespace Engine::Graphics {
    class D3D12Core;
    
    class GraphicsContext {
    public:
        GraphicsContext(int width, int height, HWND nativeWnd, bool windowed);
        ~GraphicsContext();

        void Render();
    private:
        std::unique_ptr<D3D12Core> _graphicsCore;
    };
}
