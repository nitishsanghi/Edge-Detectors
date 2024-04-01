#include "edge_detector.hpp"

// Define implementations for the STB image loading and writing libraries
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Load an image from disk using the stb_image library
bool EdgeDetector::loadImage(std::string filename){
    // Temporary variables to store image dimensions and color channels
    int width, height, channels;
    // Load image data into a smart pointer for automatic memory management
    std::unique_ptr<unsigned char, void(*)(void*)> image_data(stbi_load(filename.c_str(), &width, &height, &channels, 0), stbi_image_free);
    if(!image_data){
        std::cerr << "Error loading image" << std::endl;
        return false;
    }

    // Assign loaded image dimensions and channels to class members
    this->width = width;
    this->height = height;
    this->channels = channels;
    in_image.clear();

    // Separate the loaded image data into individual matrices for each color channel
    for(int c = 0; c < channels; ++c){
        Eigen::MatrixXd temp_image_channel(height, width);
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                temp_image_channel(i, j) = image_data.get()[i * width * channels + j * channels + c];
            }
        }
        in_image.push_back(std::move(temp_image_channel));
    }
    return true;
}

// Save an image to disk in PNG format using stb_image_write
bool EdgeDetector::saveImage(std::string filename, ImageType image_type){
    if(!in_image.size()){
        std::cerr << "No image data available" << std::endl;
        return false;
    }
    int saveChannels = 0;
    std::vector<Eigen::MatrixXd> image;
    switch(image_type){
        case ImageType::COLOR:
            saveChannels = 3;
            image = in_image;
            break;
        case ImageType::GRAYSCALE:
            saveChannels = 1;
            break;
    }

    // Convert the processed image data back to unsigned char format for saving
    std::vector<unsigned char> image_data(width * height * saveChannels);
    for(int c = 0; c < saveChannels; ++c){
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                image_data[i * width * saveChannels + j * saveChannels + c] = static_cast<unsigned char>(image[c](i, j));
            }
        }
    }

    // Write the image data to a PNG file
    if (!stbi_write_png(filename.c_str(), width, height, saveChannels, image_data.data(), width * saveChannels)) {
        std::cerr << "Failed to save image" << std::endl;
        return false;
    }

    return true;
}

// Specifically for saving grayscale images derived from edge detection
bool EdgeDetector::saveEdgeImage(std::string filename, Eigen::MatrixXd edges){
    if(!in_image.size()){
        std::cerr << "No image data available" << std::endl;
        return false;
    }
    // Edge images are saved as single-channel grayscale images
    int saveChannels = 1;
    std::vector<unsigned char> image_data(width * height * saveChannels);
    for(int i = 0; i < height; ++i){
        for(int j = 0; j < width; ++j){
            image_data[i * width * saveChannels + j * saveChannels] = static_cast<unsigned char>(edges(i, j));
        }
    }

    // Write the edge image data to a PNG file
    if (!stbi_write_png(filename.c_str(), width, height, saveChannels, image_data.data(), width * saveChannels)) {
        std::cerr << "Failed to save image" << std::endl;
        return false;
    }

    return true;
}

// Apply the specified Sobel detector to the image and return the result
Eigen::MatrixXd EdgeDetector::applyDetector(DetectorType detector_type){
    this->convertToGrayscale(); // First, convert the image to grayscale
    // Apply the specified Sobel filter based on the detector type
    switch (detector_type){
        case DetectorType::SOBEL_X:
            return sobelX();
        case DetectorType::SOBEL_Y:
            return sobelY();
        case DetectorType::SOBEL:
            return sobel();
    }
}

// Convert the loaded image to grayscale
bool EdgeDetector::convertToGrayscale() {
    if (channels >= 3) { // If the image is color, convert using standard formula
        gray_image = 0.2989 * in_image[0] + 0.5870 * in_image[1] + 0.1140 * in_image[2];
    } else if (channels == 1) { // If the image is already grayscale
        gray_image = in_image[0];
    } else {
        std::cerr << "Unsupported number of channels: " << channels << std::endl;
        return false;
    }
    return true;
}

// Implementation of the Sobel X filter
Eigen::MatrixXd EdgeDetector::sobelX(){
    // Define the Sobel X kernel
    Eigen::MatrixXd Gx(3,3);
    Gx << -1, 0, 1,
          -2, 0, 2,
          -1, 0, 1;
    
    // Apply the kernel to the grayscale image to detect edges in the X direction
    Eigen::MatrixXd sobel_x(height, width);
    for(int i = 1; i < height - 1; ++i){
        for(int j = 1; j < width - 1; ++j){
            sobel_x(i, j) = (Gx.array() * gray_image.block(i - 1, j - 1, 3, 3).array()).sum();
        }
    }
    return sobel_x;
}

// Implementation of the Sobel Y filter
Eigen::MatrixXd EdgeDetector::sobelY(){
    // Define the Sobel Y kernel
    Eigen::MatrixXd Gy(3,3);
    Gy << -1, -2, -1,
           0,  0,  0,
           1,  2,  1;
    
    // Apply the kernel to the grayscale image to detect edges in the Y direction
    Eigen::MatrixXd sobel_y(height, width);
    for(int i = 1; i < height - 1; ++i){
        for(int j = 1; j < width - 1; ++j){
            sobel_y(i, j) = (Gy.array()*gray_image.block(i - 1, j - 1, 3, 3).array()).sum();
        }
    }
    return sobel_y;
}

// Combine Sobel X and Y filters to compute the magnitude of gradients
Eigen::MatrixXd EdgeDetector::sobel(){
    Eigen::MatrixXd sobel_x = sobelX();
    Eigen::MatrixXd sobel_y = sobelY();

    // Compute the magnitude of the gradient
    Eigen::MatrixXd sobel_mag = (sobel_x.array().square() + sobel_y.array().square()).sqrt();
    return sobel_mag;
}
