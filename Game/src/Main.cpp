#include "Engine.h"

class TestBed : public Engine::Core::Application {
public:
    TestBed();
};

TestBed::TestBed() {
    CLIENT_INFO("Testbed Application initialzed. yeah");
}

Engine::Core::Application* Engine::Core::CreateApp() {
    return new TestBed();
}
