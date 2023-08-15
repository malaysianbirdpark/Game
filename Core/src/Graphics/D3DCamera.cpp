#include "pch.h"
#include "D3DCamera.h"

#include <imgui.h>

#define PI (3.141592f)

void Engine::Graphics::D3DCamera::Init() {
    DirectX::XMVECTOR const init_pos{0.0f, 0.0f, 0.0f, 0.0f};
    DirectX::XMStoreFloat3(&_pos, init_pos);
}

DirectX::XMVECTOR Engine::Graphics::D3DCamera::GetPos() {
    return DirectX::XMLoadFloat3(&_pos);
}

DirectX::XMMATRIX Engine::Graphics::D3DCamera::GetView() {
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

void Engine::Graphics::D3DCamera::Update() {
    if (ImGui::Begin("Camera")) {
        ImGui::Text("Position");
        ImGui::SliderFloat("X", &_pos.x, -100.0f, 100.0f);
        ImGui::SliderFloat("Y", &_pos.y, -100.0f, 100.0f);
        ImGui::SliderFloat("Z", &_pos.z, -100.0f, 100.0f);

        ImGui::Text("Rotation");
        ImGui::SliderFloat("Pitch", &_pitch, -89.5f, 89.5f);
        ImGui::SliderFloat("Yaw", &_yaw, -175.0f, 175.0f);

        if (ImGui::Button("Reset"))
            Reset();
    }
    ImGui::End();
}

void Engine::Graphics::D3DCamera::Reset() {
    _pos = DirectX::XMFLOAT3{0.0f, 0.0f, 0.0f};
    _pitch = 0.0f;
    _yaw = 0.0f;
}

void Engine::Graphics::D3DCamera::Rotate(float const dx, float const dy) {
    _yaw = _yaw + dx * _rotationSpeed;
    _pitch = std::clamp(_pitch + dy * _rotationSpeed, -(PI / 2.0f) * 0.995f, PI / 2.0f * 0.995f);
}

void Engine::Graphics::D3DCamera::Translate(DirectX::XMFLOAT3 mat) {
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
