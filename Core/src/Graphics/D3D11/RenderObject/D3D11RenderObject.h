#pragma once

namespace Engine {
    class D3D11TransformCB;
    class D3D11SceneGraph;

    class D3D11RenderObject {
        template <typename ObjectType>
        struct ProtoType {
            ProtoType(ObjectType type)
                : _type{std::move(type)}
            {}

            ObjectType _type;
        };
    public:
        template <typename ObjectType>
        D3D11RenderObject(
            ID3D11Device& device, 
            ID3D11DeviceContext& context, 
            D3D11SceneGraph&& scene, 
            ObjectType type)
        : _pimpl {
            new ProtoType<ObjectType>(std::move(type)),
            [] (void* object_bytes) {
                using Object = ProtoType<ObjectType>;
                auto* const object {static_cast<Object*>(object_bytes)};
                delete object;
            }
        },
          _render {
            [] (void* object_bytes) {
                using Object = ProtoType<ObjectType>;
                auto* const object {static_cast<Object*>(object_bytes)};
            }
        },
          _update  {
            [] (void* object_bytes) {
                using Object = ProtoType<ObjectType>;
                auto* const object {static_cast<Object*>(object_bytes)};
            }
        }
        {}
    private:
        friend void Render(ID3D11DeviceContext& context, D3D11RenderObject const& obj) {
            obj._render(context);
        }

        friend void Update(float const dt, D3D11RenderObject& obj) {
            obj._update(dt);
        }
    private:
        using DestroyOperation = void(void*);
        using RenderOperation = void(ID3D11DeviceContext&);
        using UpdateOperation = void(float const);

        std::unique_ptr<void, DestroyOperation*> _pimpl;
        RenderOperation* _render {};
        UpdateOperation* _update {};
    };
}

