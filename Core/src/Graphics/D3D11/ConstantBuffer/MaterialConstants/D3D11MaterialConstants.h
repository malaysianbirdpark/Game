#pragma once

#include <variant>

#include "D3D11SolidConstant.h"
#include "D3D11PhongConstants.h"
#include "D3D11UnrealPBRConstants.h"

namespace Engine::Graphics {
    using D3D11MaterialConstant = std::variant<D3D11SolidConstants, D3D11PhongConstants, D3D11UnrealPBRConstants>;
    
}
