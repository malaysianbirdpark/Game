#include "pch.h"
#include "D3D11SceneMan.h"

#include "D3D11SceneGraph.h"

#include <fstream>
#include <rapidjson/document.h>


void Engine::Graphics::D3D11SceneMan::Load(ID3D11Device& device) {
    GRAPHICS_INFO("Loading Models..");

    static constexpr auto model_list_path {"Assets/model_list.json"};

    // Read json file.. 
    std::ifstream ifile {model_list_path};
    if (!ifile.is_open()) {
        std::cout << "file is not opened!" << std::endl;
        return;
    }

    ifile.seekg(0, std::ios_base::end);
    auto const length {ifile.tellg()};
    ifile.seekg(0, std::ios_base::beg);

    std::string buffer;
    buffer.resize(static_cast<size_t>(length));
    ifile.read(reinterpret_cast<char*>(buffer.data()), length);

    //auto const succ {!ifile.fail() && length == ifile.gcount()};
    //std::cout << ifile.gcount() << std::endl;
    //if (!succ) {
    //    std::cout << "file read error!" << std::endl;
    //    return;
    //}

    ifile.close();

    rapidjson::StringStream jsonStr {buffer.data()};
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);
    if (!doc.IsObject()) {
        std::cout << "Json parsing failed!" << std::endl;
        return;
    }

    auto const& models {doc["models"]};
    for (auto i {unsigned int{}}; i != models.Size(); ++i)
        _scenes[models[i]["name"].GetString()] = MakeShared<D3D11SceneGraph>(device, models[i]["path"].GetString());

    while (true)
        ;

    GRAPHICS_INFO("Done");
}

std::shared_ptr<Engine::Graphics::D3D11SceneGraph> Engine::Graphics::D3D11SceneMan::ResolveScene(x_string const& tag) {
    if (!_scenes.contains(tag)) {
        GRAPHICS_ERROR("No scene tagged as " + tag + " has found!");
        return nullptr;
    }

    return _scenes[tag];
}
