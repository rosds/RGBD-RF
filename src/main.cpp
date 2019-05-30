#include <train_forest/Config.h>

#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>

int main(int argc, char* argv[]) {
    auto node = YAML::LoadFile("config.yaml");
    train_forest::TrainForestConfig config;

    const auto images = node["images"];
    if (images) {
        for (YAML::const_iterator it = images.begin(); it != images.end();
             ++it) {
            config.images.emplace_back(train_forest::ImageInfo{});
            auto& last_image = config.images.back();
            last_image.name = (*it)["name"].as<std::string>();
        }
    }

    for (const auto& img : config.images) {
        std::cout << img << "\n";
    }

    return 0;
}
