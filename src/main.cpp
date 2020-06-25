#include <ds/RgbdWashington.h>
#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>

#include <filesystem>
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " configuration.yaml\n";
        return 1;
    }

    auto node = YAML::LoadFile(argv[1]);

    RgbdWashingtonDataset dataset;

    const auto image_directories = node["image_directories"];
    for (YAML::const_iterator it = image_directories.begin();
         it != image_directories.end(); ++it) {
        const auto directory_path =
            std::filesystem::path{(*it)["path"].as<std::string>()};
        dataset.emplace_image_directory(directory_path);
    }

    const auto images = dataset.load();

    return 0;
}
