#pragma once

namespace Engine {
    namespace Graphics {
        class D3D11Core;
    }
}

namespace Engine::Core {
    class Application {
    public:
        Application();
        virtual ~Application();

        Application(Application const&) = delete;
        Application& operator=(Application const&) = delete;

        int Run();
    protected:
        virtual void ProcessInput(float const dt);
        virtual void Update(float const dt);
    protected:
        inline static int width {};
        inline static int height {};
    private:
        //Graphics::GraphicsCore  _gfx;
        std::unique_ptr<Graphics::D3D11Core> _gfx;
    };

    [[nodiscard]] Application* CreateApp();
}

