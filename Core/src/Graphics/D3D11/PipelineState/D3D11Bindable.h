#pragma once

namespace Glowing::Bind {
    class Bindable {
    public:
        Bindable() = default;
        virtual ~Bindable() = default;

        // deleted
        Bindable(Bindable const&) = delete;
        Bindable& operator=(Bindable const&) = delete;
    public:
        virtual void Bind() noexcept = 0;
        [[nodiscard]] virtual std::string GetUID() const noexcept {
            GW_CORE_ASSERT(false, "Invalid call from Base class");
            return "";
        }
    protected:
        //[[nodiscard]] static DxgiInfoManager& GetInfoManager(D3D11Device& gfx) noexcept(!GW_DEBUG);
    };
}

