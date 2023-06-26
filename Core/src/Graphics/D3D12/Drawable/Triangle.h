#pragma once

namespace Engine::Graphics {
class GraphicsContext;
class VertexBuffer;

class Triangle {
public:
    Triangle(GraphicsContext& gfx);

    void Draw(GraphicsContext& gfx);
private:
    std::unique_ptr<VertexBuffer> _vertexBuffer;
};
}

