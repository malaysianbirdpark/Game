#include "pch.h"
#include "Triangle.h"

Engine::Graphics::Triangle::Triangle() {
	vertices.resize(3);
	vertices[0]._pos = DirectX::XMFLOAT3(0.f, 0.5f, 0.5f);
	vertices[0]._color = DirectX::XMFLOAT4(1.f, 0.f, 0.f, 1.f);
	vertices[1]._pos = DirectX::XMFLOAT3(0.5f, -0.5f, 0.5f);
	vertices[1]._color = DirectX::XMFLOAT4(0.f, 1.0f, 0.f, 1.f);
	vertices[2]._pos = DirectX::XMFLOAT3(-0.5f, -0.5f, 0.5f);
	vertices[2]._color = DirectX::XMFLOAT4(0.f, 0.f, 1.f, 1.f);
}
