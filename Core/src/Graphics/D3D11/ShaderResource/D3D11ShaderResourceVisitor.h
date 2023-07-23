#pragma once

#include <variant>

namespace Engine::Graphics::ShaderResource {
    class D3D11DiffuseMap;
    class D3D11SpecularMap;
    class D3D11NormalMap;

    using ShaderResource = std::variant<D3D11DiffuseMap, D3D11SpecularMap, D3D11NormalMap>;

    void BindDiffuseMap(D3D11DiffuseMap const& sr);
    void BindSpecularMap(D3D11SpecularMap const& sr);
    void BindNormalMap(D3D11NormalMap const& sr);

    struct Bind {
        void operator() (D3D11DiffuseMap const& sr) const {
            GRAPHICS_INFO("Binding Diffuse Map");
            BindDiffuseMap(sr);
        }
        void operator() (D3D11SpecularMap const& sr) const {
            GRAPHICS_INFO("Binding Diffuse Map");
            BindSpecularMap(sr);
        }
        void operator() (D3D11NormalMap const& sr) const {
            GRAPHICS_INFO("Binding Diffuse Map");
            BindNormalMap(sr);
        }
    };
}

