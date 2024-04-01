#ifndef EDGE_DETECTOR_HPP
#define EDGE_DETECTOR_HPP

// Include statements for necessary libraries and dependencies.
#include <Eigen/Dense> // Eigen library for linear algebra and matrix operations.
#include <opencv2/opencv.hpp> // OpenCV for image processing tasks.
#include <iostream>
#include <string> // Standard string class for filename handling.
#include <optional> // Optional for error handling and optional return values.
#include <vector> // Standard vector class for dynamic arrays.
#include <utility> // Utility library, commonly used for pair and swap.
#include <memory> // Memory management utilities.
#include <cmath> // Standard math library.
#include <functional> // Function objects and operations.

// EdgeDetector class defines an interface and implementation for detecting edges in images.
// It supports multiple edge detection methods and can process both color and grayscale images.
class EdgeDetector{
public:
    // Enumerations to specify the type and method of edge detection.
    enum class ImageType{
        COLOR,      // Indicates a color image.
        GRAYSCALE,  // Indicates a grayscale image.
    };

    enum class DetectorType{
        SOBEL,        // Use Sobel operator for edge detection.
        PREWITT,      // Use Prewitt operator for edge detection.
        ROBERTSCROSS, // Use Roberts Cross operator for edge detection.
    };

    enum class GradientType{
        X,    // Detect edges in the horizontal (X) direction.
        Y,    // Detect edges in the vertical (Y) direction.
        MAG,  // Calculate the magnitude of edges by combining X and Y directions.
    };

    // Constructors and destructors.
    EdgeDetector() = default; // Default constructor.
    ~EdgeDetector() = default; // Default destructor.

    // Public interface methods.
    bool loadImage(std::string filename); // Loads an image from the specified file.
    Eigen::MatrixXd applyDetector(DetectorType detector_type, GradientType direction); // Applies the selected edge detection algorithm.
    bool saveImage(std::string filename, ImageType image_type = ImageType::COLOR); // Saves the processed image to a file.
    bool saveEdgeImage(std::string filename, Eigen::MatrixXd Edges); // Saves the edge-detected image.

private:
    // Private member variables for image dimensions and storage.
    int width; // Image width.
    int height; // Image height.
    int channels; // Number of color channels in the image.
    std::vector<Eigen::MatrixXd> in_image; // Vector of matrices to store the original image channels.
    Eigen::MatrixXd gray_image; // Matrix to store the grayscale version of the image.

    // Private methods for edge detection algorithms.
    Eigen::MatrixXd sobel(GradientType direction); // Implements the Sobel edge detection.
    Eigen::MatrixXd prewitt(GradientType direction); // Implements the Prewitt edge detection.
    void kernel_processor(Eigen::MatrixXd& edges, Eigen::MatrixXd kernel); // Applies a convolution kernel to detect edges.
    Eigen::MatrixXd kernel_detector(DetectorType detector_type, GradientType direction); // Detects edges using specified kernel and gradient type.

    // Utility method to convert an image to grayscale.
    bool convertToGrayscale(); // Converts the loaded image to grayscale, facilitating edge detection on color images.

};

#endif // EDGE_DETECTOR_HPP
