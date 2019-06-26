#include <ds/RgbdWashington.h>

#include <yaml-cpp/parser.h>
#include <yaml-cpp/yaml.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    try {
        po::options_description desc{"Options"};
        desc.add_options()("help,h", "Help")(
            "input,i", po::value<std::string>()->default_value("config.yaml"),
            "Input params");

        po::positional_options_description positional_args;
        positional_args.add("input", -1);

        po::variables_map vm;
        po::store(po::command_line_parser(argc, argv)
                      .options(desc)
                      .positional(positional_args)
                      .run(),
                  vm);
        po::notify(vm);

        if (vm.count("help")) {
            std::cout << desc << '\n';
            return 0;
        }

        auto node = YAML::LoadFile(vm["input"].as<std::string>());

        RgbdWashingtonDataset dataset;

        const auto image_directories = node["image_directories"];
        for (YAML::const_iterator it = image_directories.begin();
             it != image_directories.end(); ++it) {
            const auto directory_path =
                boost::filesystem::path{(*it)["path"].as<std::string>()};
            dataset.emplace_image_directory(directory_path);
        }

        const auto images = dataset.load();

    } catch (std::exception const& e) {
        std::cerr << e.what() << '\n';
    }
    return 0;
}
