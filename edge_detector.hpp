#ifndef EDGE_DETECTOR_HPP
#define EDGE_DETECTOR_HPP

// External library headers for matrix operations and image processing
#include <Eigen/Dense>
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <optional>
#include <vector>
#include <utility>
#include <memory>
#include <cmath>
#include <functional>

// EdgeDetector class for detecting edges in images using Sobel operators
class EdgeDetector{
    public:
        // Enumeration to specify the type of image being processed
        enum class ImageType{
            COLOR,      // For color images
            GRAYSCALE,  // For grayscale images
        };

        // Enumeration to specify the type of edge detection method to use
        enum class DetectorType{
            SOBEL,  
            PREWITT, 
        };

        enum class GradientType{
            X,  // Detect edges in the X direction
            Y,  // Detect edges in the Y direction
            MAG,    // Detect edges in both X and Y directions
        };
        
        // Default constructor and destructor
        EdgeDetector() = default;
        ~EdgeDetector() = default;

        // Load an image from a file
        bool loadImage(std::string filename);
        // Apply the specified edge detection filter and return the result
        Eigen::MatrixXd applyDetector(DetectorType detector_type, GradientType direction);
        // Save the processed image back to a file
        bool saveImage(std::string, ImageType image_type = ImageType::COLOR);
        // Save the edge-detected image to a file
        bool saveEdgeImage(std::string,  Eigen::MatrixXd Edges);

    private:
        // Dimensions and channels of the input image
        int width;
        int height;
        int channels;
        // Storage for the original image (split into channels if necessary)
        std::vector<Eigen::MatrixXd> in_image;
        // Storage for the grayscale version of the image
        Eigen::MatrixXd gray_image;
    
        // Sobel filter implementations for edge detection
        /*Eigen::MatrixXd sobelX(); // Sobel filter in X direction
        Eigen::MatrixXd sobelY(); // Sobel filter in Y direction
        Eigen::MatrixXd sobel();  // Combination of Sobel X and Y filters*/
        Eigen::MatrixXd sobel(GradientType direction);

        // Prewitt filter implementations for edge detection
        Eigen::MatrixXd prewittX(); // Prewitt filter in X direction
        Eigen::MatrixXd prewittY(); // Prewitt filter in Y direction
        Eigen::MatrixXd prewitt(GradientType direction);  // Combination of Prewitt X and Y filters
        void kernel_processor(Eigen::MatrixXd& edges, Eigen::MatrixXd kernel);
        

        // Convert loaded image to grayscale
        bool convertToGrayscale();

};

#endif // EDGE_DETECTOR_HPP
