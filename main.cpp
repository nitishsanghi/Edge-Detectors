#include "edge_detector.hpp"

int main(){

    std::string input_image_path = "/Users/nitishsanghi/Documents/Stereo-Matching/000000_10.png";
    std::string output_image_path = "/Users/nitishsanghi/Documents/Edge-Detector/000000_10_sobel.png";
    std::string output_image_pathx = "/Users/nitishsanghi/Documents/Edge-Detector/000000_10_sobelx.png";
    std::string output_image_pathy = "/Users/nitishsanghi/Documents/Edge-Detector/000000_10_sobely.png";
    std::string output_image_pathp = "/Users/nitishsanghi/Documents/Edge-Detector/000000_10_p.png";
    std::string output_image_pathpx = "/Users/nitishsanghi/Documents/Edge-Detector/000000_10_px.png";
    std::string output_image_pathpy = "/Users/nitishsanghi/Documents/Edge-Detector/000000_10_py.png";
    std::string output_image_pathr = "/Users/nitishsanghi/Documents/Edge-Detector/000000_10_r.png";
    std::string output_image_pathrx = "/Users/nitishsanghi/Documents/Edge-Detector/000000_10_rx.png";
    std::string output_image_pathry = "/Users/nitishsanghi/Documents/Edge-Detector/000000_10_ry.png";
 
    EdgeDetector detector;
    detector.loadImage(input_image_path);
    detector.saveEdgeImage(output_image_path, detector.applyDetector(EdgeDetector::DetectorType::SOBEL, EdgeDetector::GradientType::MAG));
    detector.saveEdgeImage(output_image_pathx, detector.applyDetector(EdgeDetector::DetectorType::SOBEL, EdgeDetector::GradientType::X));
    detector.saveEdgeImage(output_image_pathy, detector.applyDetector(EdgeDetector::DetectorType::SOBEL, EdgeDetector::GradientType::Y));
    detector.saveEdgeImage(output_image_pathp, detector.applyDetector(EdgeDetector::DetectorType::PREWITT, EdgeDetector::GradientType::MAG));
    detector.saveEdgeImage(output_image_pathpx, detector.applyDetector(EdgeDetector::DetectorType::PREWITT, EdgeDetector::GradientType::X));
    detector.saveEdgeImage(output_image_pathpy, detector.applyDetector(EdgeDetector::DetectorType::PREWITT, EdgeDetector::GradientType::Y));
    detector.saveEdgeImage(output_image_pathr, detector.applyDetector(EdgeDetector::DetectorType::ROBERTSCROSS, EdgeDetector::GradientType::MAG));
    detector.saveEdgeImage(output_image_pathrx, detector.applyDetector(EdgeDetector::DetectorType::ROBERTSCROSS, EdgeDetector::GradientType::X));
    detector.saveEdgeImage(output_image_pathry, detector.applyDetector(EdgeDetector::DetectorType::ROBERTSCROSS, EdgeDetector::GradientType::Y));


    return 0;
}