#pragma once

namespace Engine::Graphics {
    class D3D11Core;
    //class D3D12Core;
    class D3DCamera;
    class D3D11SceneGraph;
    
    class GraphicsContext {
    public:
        GraphicsContext(int width, int height, HWND nativeWnd, bool windowed);
        ~GraphicsContext();

        void Update(float const dt);
        void Render();

        ID3D11Device& Device();
    private:
        std::unique_ptr<D3D11Core> _core11;
        //std::unique_ptr<D3D12Core> _core12;

        std::unique_ptr<D3DCamera> _cam;
    };
}
