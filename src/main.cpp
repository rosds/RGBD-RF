#include <dataset_reader/RgbdWashington.h>

#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>
#include <boost/filesystem.hpp>

int main(int argc, char* argv[]) {
    auto node = YAML::LoadFile("config.yaml");

    RgbdWashingtonDataset dataset;

    const auto image_directories = node["image_directories"];
    for (YAML::const_iterator it = image_directories.begin();
         it != image_directories.end(); ++it) {
        const auto directory_path =
            boost::filesystem::path{(*it)["path"].as<std::string>()};
        dataset.emplace_image_directory(directory_path);
    }

    const auto images = dataset.load();

    return 0;
}
