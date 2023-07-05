#pragma once

namespace Engine {
    namespace Platform {
        class Platform;
    }

    namespace Graphics {
        class GraphicsContext;
        class D3DCamera;
        class D3DSceneGraph;
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
        virtual void ProcessInput();
        virtual void Update(float const dt);
        virtual void Render();
    protected:
        inline static int width {};
        inline static int height {};
    private:
        std::unique_ptr<Platform::Platform>         _platform;
        std::unique_ptr<Graphics::GraphicsContext>  _gfx;

        std::shared_ptr<Graphics::D3DCamera>        _cam; 
    };

    [[nodiscard]] Application* CreateApp();
}

