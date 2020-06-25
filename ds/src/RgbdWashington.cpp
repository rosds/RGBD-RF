#include <ds/RgbdWashington.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <regex>

struct ImageFileLoader {
    template <typename PixelT>
    static rf::Image<PixelT> Load(fs::path const& path) {
        auto img = cv::imread(path.string(),
                              cv::IMREAD_ANYDEPTH | cv::IMREAD_ANYCOLOR);

        if constexpr (std::is_same_v<PixelT, float>) {
            img.convertTo(img, CV_32F);
        }

        return {static_cast<size_t>(img.cols), static_cast<size_t>(img.rows),
                img.begin<PixelT>(), img.end<PixelT>()};
    }
};

void RgbdWashingtonDataset::emplace_image_directory(fs::path directory) {
    if (fs::is_directory(directory)) {
        directories_.emplace_back(directory);
    } else {
        std::ostringstream error;
        error << "Not a directory: " << directory;
        throw std::runtime_error(error.str());
    }
}

typename RgbdWashingtonDataset::TrainSet RgbdWashingtonDataset::load() const {
    auto images = TrainSet{};
    const std::regex depth_image_filename{"(\\w+)_depthcrop.png"};
    for (const auto& directory : directories_) {
        for (const auto& entry : fs::directory_iterator(directory)) {
            if (!fs::is_regular_file(entry)) {
                continue;
            }

            const auto filename = entry.path().filename().string();
            std::smatch filename_match;
            if (std::regex_match(filename, filename_match,
                                 depth_image_filename)) {
                // labels image
                const auto label_image =
                    directory /
                    fs::path{filename_match[1].str() + "_maskcrop.png"};

                auto depth = ImageFileLoader::Load<float>(entry.path());
                auto label =
                    LabelImage{ImageFileLoader::Load<uint8_t>(label_image)};

                images.emplace_back(std::move(depth), std::move(label));
            }
        }
    }
    return images;
}
