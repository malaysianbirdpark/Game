#pragma once

namespace Engine::Graphics {
    class D3D11Core;
    //class D3D12Core;
    class D3DCamera;
    class D3D11SceneGraph;
    
    class GraphicsCore {
        template<typename GraphicsAPI>
        struct OwningModel {
            OwningModel(GraphicsAPI core)
                : _core{std::move(core)}
            {}

            GraphicsAPI _core;
        };
    private:
        using DestroyOperation = void(void*);
        using BeginFrameOperation = void(void);
        using EndFrameOperation = void(void);
        using UpdateOperation = void*(float const dt);
        using CloneOperation = void*(void*);

        std::unique_ptr<void, DestroyOperation*> _pimpl;
        BeginFrameOperation* _beginFrame {nullptr};
        EndFrameOperation*   _endFrame {nullptr};
        UpdateOperation*     _update {nullptr};
        CloneOperation*      _clone {nullptr};
    public:
        template <typename GraphicsAPI>
        GraphicsCore(GraphicsAPI core)
            : _pimpl{
                  new OwningModel<GraphicsAPI>{std::move(core)},
                  [](void* bytes) {
                      using Model = OwningModel<GraphicsAPI>;
                      auto* const model = static_cast<Model*>(bytes);
                      delete model;
                  }
            },
            _beginFrame{
                  [](void* bytes) {
                      using Model = OwningModel<GraphicsAPI>;
                      auto* const model = static_cast<Model*>(bytes);
                      (model->_core).BeginFrame();
                  }
            },
            _endFrame{
                  [](void* bytes) {
                      using Model = OwningModel<GraphicsAPI>;
                      auto* const model = static_cast<Model*>(bytes);
                      (model->_core).EndFrame();
                  }
            },
            _update{
                  [](void* bytes, float const dt) {
                      using Model = OwningModel<GraphicsAPI>;
                      auto* const model = static_cast<Model*>(bytes);
                      (model->_core).Update(dt);
                  }
            },
            _clone{
                [](void* shapeBytes) -> void* {
                    using Model = OwningModel<GraphicsAPI>;
                    auto* const model = static_cast<Model*>(shapeBytes);
                    return new Model{*model};
                }
            }
            {}
    public:
        GraphicsCore(GraphicsCore const& other)
            : _pimpl{_clone(other._pimpl.get()), other._pimpl.get_deleter()},
              _beginFrame{other._beginFrame},
              _endFrame{other._endFrame},
              _update{other._update},
              _clone{other._clone}
            {}

        GraphicsCore& operator=(GraphicsCore const& other) {
            using std::swap;
            GraphicsCore copy{other};
            swap(_pimpl, copy._pimpl);
            swap(_beginFrame, copy._beginFrame);
            swap(_endFrame, copy._endFrame);
            swap(_update, copy._update);
            swap(_clone, copy._clone);
            return *this;
        }

        ~GraphicsCore() = default;
        GraphicsCore(GraphicsCore&&) = default;
        GraphicsCore& operator=(GraphicsCore&&) = default;
    };
}
