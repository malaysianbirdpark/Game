#pragma once

#include "Vertex.h"

namespace Engine::Graphics {
class Triangle {
public:
    Triangle();

    x_vector<Vertex> vertices;
};
}

