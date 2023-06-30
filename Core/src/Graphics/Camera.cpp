#include "pch.h"
#include "Camera.h"

#define PI (3.141592f)

Engine::Graphics::Camera::Camera(DirectX::XMFLOAT3 init_pos)
    : _pos{init_pos}
{
}

DirectX::XMMATRIX Engine::Graphics::Camera::LookAt() const {
        auto const lookAt {
            DirectX::XMVector3Transform(
            DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
            DirectX::XMMatrixRotationRollPitchYaw(_pitch, _yaw, 0.0f)
        )
        };
        auto const camPos {
            DirectX::XMLoadFloat3(&_pos)
        };
        // TODO: CamTarget
        auto const camTarget {
            DirectX::XMVectorAdd(camPos, lookAt)
        };
        return DirectX::XMMatrixLookAtLH(
            camPos, 
            camTarget, 
            DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
        );
}

void Engine::Graphics::Camera::Rotate(float const dx, float const dy) {
    _yaw = std::clamp(_yaw + dx * _rotationSpeed, -(PI / 2.0f), PI / 2.0f);
    _pitch = std::clamp(_pitch + dy * _rotationSpeed, -(PI / 2.0f), PI / 2.0f);
}

void Engine::Graphics::Camera::Translate(DirectX::XMFLOAT3 mat) {
   DirectX::XMStoreFloat3(&mat, DirectX::XMVector3Transform(
       DirectX::XMLoadFloat3(&mat),
       DirectX::XMMatrixRotationRollPitchYaw(_pitch, _yaw, 0.0f) *
       DirectX::XMMatrixScaling(_travelSpeed, _travelSpeed, _travelSpeed)
   ));

   _pos = {
       _pos.x + mat.x,
       _pos.y + mat.y,
       _pos.z + mat.z
   };
}
