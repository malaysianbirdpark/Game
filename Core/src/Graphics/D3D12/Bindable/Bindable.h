#pragma once

namespace Engine::Graphics {
    class GraphicsContext;
    
    class Bindable {
    public:
        Bindable() = default;
        virtual ~Bindable() = default;

        // deleted
        //Bindable(Bindable const&) = delete;
        //Bindable& operator=(Bindable const&) = delete;

        virtual void Bind(GraphicsContext& gfx) noexcept = 0;

        [[nodiscard]] virtual std::string GetUID() const noexcept {
            CORE_ASSERT(false, "This is Bindable Base Class!");
            return "";
        }
    };
}

