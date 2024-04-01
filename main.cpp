#include "edge_detector.hpp"

int main(){

    std::string input_image_path = "/Users/nitishsanghi/Documents/Stereo-Matching/000000_10.png";
    std::string output_image_path = "/Users/nitishsanghi/Documents/Edge-Detector/000000_10_sobel.png";
    std::string output_image_pathx = "/Users/nitishsanghi/Documents/Edge-Detector/000000_10_sobelx.png";
    std::string output_image_pathy = "/Users/nitishsanghi/Documents/Edge-Detector/000000_10_sobely.png";
    EdgeDetector detector;
    detector.loadImage(input_image_path);
    detector.saveEdgeImage(output_image_path, detector.applyDetector(EdgeDetector::DetectorType::SOBEL, EdgeDetector::GradientType::MAG));
    detector.saveEdgeImage(output_image_pathx, detector.applyDetector(EdgeDetector::DetectorType::SOBEL, EdgeDetector::GradientType::X));
    detector.saveEdgeImage(output_image_pathy, detector.applyDetector(EdgeDetector::DetectorType::SOBEL, EdgeDetector::GradientType::Y));
    return 0;
}