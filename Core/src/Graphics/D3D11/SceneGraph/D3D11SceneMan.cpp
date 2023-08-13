#include "pch.h"
#include "D3D11SceneMan.h"

#include "D3D11SceneGraph.h"
#include "Graphics/D3D11/RenderStrategy/D3D11RenderStrategy.h"

#include <fstream>
#include <rapidjson/document.h>

void Engine::Graphics::D3D11SceneMan::Load(ID3D11Device& device) {
    GRAPHICS_INFO("Loading Models..");

    static constexpr auto model_list_path {"Assets/model_list.json"};

    // Read json file.. 
    std::ifstream ifile {model_list_path};
    CORE_ASSERT(ifile.is_open(), "file is not opened!");

    ifile.seekg(0, std::ios_base::end);
    auto const length {ifile.tellg()};
    ifile.seekg(0, std::ios_base::beg);

    std::string buffer;
    buffer.resize(static_cast<size_t>(length));
    ifile.read(reinterpret_cast<char*>(buffer.data()), length);

    ifile.close();

    rapidjson::StringStream jsonStr {buffer.data()};
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);
    CORE_ASSERT(doc.IsObject(), "Json parsing failed!");

    auto const& models {doc["models"]};
    CORE_ASSERT(models.IsArray(), "models is not array!");

    for (auto it_model {models.Begin()}; it_model != models.End(); ++it_model) {
        auto const model {it_model->GetObj()};
        auto const name {model["name"].GetString()};

        _scenes[name] = MakeShared<D3D11SceneGraph>(device, model["path"].GetString());
    }

    GRAPHICS_INFO("Done");
}

std::shared_ptr<Engine::Graphics::D3D11SceneGraph> Engine::Graphics::D3D11SceneMan::ResolveScene(x_string const& tag) {
    if (!_scenes.contains(tag)) {
        GRAPHICS_ERROR("No scene tagged as " + tag + " has found!");
        return nullptr;
    }

    return _scenes[tag];
}
