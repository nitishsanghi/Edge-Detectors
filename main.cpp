#include "edge_detector.hpp"

int main(){

    std::string input_image_path = "/Users/nitishsanghi/Documents/Stereo-Matching/000000_10.png";
    std::string output_image_path = "/Users/nitishsanghi/Documents/Edge-Detector/000000_10_sobel.png";
    EdgeDetector detector;
    detector.loadImage(input_image_path);
    detector.saveEdgeImage(output_image_path, detector.applyDetector(EdgeDetector::DetectorType::SOBEL));
    return 0;
}