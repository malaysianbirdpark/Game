#include "pch.h"
#include "AssimpLoader.h"

#include <fstream>
#include <rapidjson/document.h>

#include <assimp/scene.h>

void Engine::Graphics::AssimpLoader::LoadAll(RenderAPI api) {
    GRAPHICS_INFO("ASSIMP - Loading Models..");

    static constexpr auto model_list_path {"Assets/model_list.json"};

    // Read json file -----------------------------------------------
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
    // ---------------------------------------------------------------

    auto path_list {doc["path"]};
    for (auto path {path_list.Begin()}; path != path_list.End(); ++path) {
        std::string p {path->GetString()};
    }
}

void Engine::Graphics::AssimpLoader::LoadFile(char const* path, RenderAPI api) {
}
