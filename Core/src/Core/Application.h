#pragma once

namespace Engine {
    namespace Platform {
        class Platform;
    }

    namespace Graphics {
        class GraphicsContext;
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
        std::unique_ptr<Platform::Platform> p_platform;
    };

    [[nodiscard]] Application* CreateApp();
}

