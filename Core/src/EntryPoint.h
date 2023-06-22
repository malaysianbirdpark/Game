#pragma once

int main(int argc, char* argv[]) {
    Engine::Core::Log::Init();
    CORE_INFO("Log Initialized");

    auto app {Engine::Core::CreateApp()};
    auto const result {app->Run()};
    delete app;
    app = nullptr;

    Engine::Core::Log::Shutdown();

    return result;
}
