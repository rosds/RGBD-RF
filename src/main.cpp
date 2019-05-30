#include <yaml-cpp/yaml.h>
#include <fstream>

int main(int argc, char* argv[]) {
    std::ifstream fconfig("config.yaml");
    YAML::Parser parser(fconfig);
    return 0;
}
