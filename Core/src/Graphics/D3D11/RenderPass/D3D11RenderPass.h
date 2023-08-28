#pragma once

#include "Graphics/D3D11/ConstantBuffer/D3D11ConstantBuffer.h"

namespace Engine::Graphics {
    // RenderStrategy를 TypeErasure로 구현해야 한다.
    // RenderPass는 Lambda로 전달하여
    // srv, rtv, cb를 bind하는 역할을 담당한다.

    class RenderPass {
        template<typename RenderPassType, typename RenderStrategy>
        struct OwningModel {
            OwningModel(RenderPassType pass, RenderStrategy render_strategy, ID3D11DeviceContext* context, ID3D11ShaderResourceView* const* srv, ID3D11RenderTargetView* const* rtv, D3D11ConstantBuffer* cb = nullptr)
                : _pass{std::move(pass)}, _strategy{std::move(render_strategy)}, _context{context}, _srv{srv}, _rtv{rtv}, _cb{cb}
            {}

            RenderPassType _pass;
            RenderStrategy _strategy;
            ID3D11DeviceContext* _context;
            ID3D11ShaderResourceView* const* _srv;
            ID3D11RenderTargetView* const* _rtv;
            D3D11ConstantBuffer* _cb;
        };
    private:
        using DestroyOperation = void(void*);
        using RenderOperation = void(void*);
        using CloneOperation = void*(void*);

        std::unique_ptr<void, DestroyOperation*> pimpl_;
        RenderOperation* render_ {nullptr};
        CloneOperation* clone_ {nullptr};
    public:
        template<typename RenderPassType, typename RenderStrategy>
        RenderPass(RenderPassType pass, RenderStrategy render_strategy, ID3D11DeviceContext& context, ID3D11ShaderResourceView* const* srv, ID3D11RenderTargetView* const* rtv, D3D11ConstantBuffer* cb)
            : pimpl_{
                  new OwningModel<RenderPassType, RenderStrategy>{std::move(pass), std::move(render_strategy), std::move(context), std::move(srv), std::move(rtv), std::move(cb)},
                  [](void* passBytes) {
                      using Model = OwningModel<RenderPassType, RenderStrategy>;
                      auto* const model = static_cast<Model*>(passBytes);
                      delete model;
                  }
              },
              render_{
                  [](void* passBytes) {
                      using Model = OwningModel<RenderPassType, RenderStrategy>;
                      auto* const model = static_cast<Model*>(passBytes);
                      (*model->_pass)(model->_context, model->_srv, model->rtv, model->_strategy, model->_cb);
                 }
              },
              clone_{
                  [](void* passBytes) -> void* {
                      using Model = OwningModel<RenderPassType, RenderStrategy>;
                      auto* const model = static_cast<Model*>(passBytes);
                      return new Model{*model};
                  }
              }
            {}

        RenderPass(RenderPass const& other)
            : pimpl_{clone_(other.pimpl_.get()), other.pimpl_.get_deleter()},
              render_{other.render_},
              clone_{other.clone_}
            {}

        RenderPass& operator=(RenderPass const& other) {
            using std::swap;
            RenderPass copy{other};
            swap(pimpl_, copy.pimpl_);
            swap(render_, copy.render_);
            swap(clone_, copy.clone_);
            return *this;
        }

        ~RenderPass() = default;
        RenderPass(RenderPass&&) = default;
        RenderPass& operator=(RenderPass&&) = default;
    };
}

