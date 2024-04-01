#include "edge_detector.hpp"

// Activate the STB image loading functionality
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Activate the STB image writing functionality
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Load an image from a file using the STB library
bool EdgeDetector::loadImage(std::string filename){
    // Variables to hold the image dimensions and the number of color channels
    int width, height, channels;
    
    // Load the image data into a unique_ptr to automatically manage memory
    // stbi_load loads the image file; stbi_image_free is called when the unique_ptr is destroyed
    std::unique_ptr<unsigned char, void(*)(void*)> image_data(stbi_load(filename.c_str(), &width, &height, &channels, 0), stbi_image_free);
    
    // Check if image loading was successful
    if(!image_data){
        std::cerr << "Error loading image" << std::endl;
        return false;
    }

    // Store the image dimensions and number of channels in the class members
    this->width = width;
    this->height = height;
    this->channels = channels;
    in_image.clear(); // Clear any previous image data

    // Divide the image data into separate matrices for each color channel
    for(int c = 0; c < channels; ++c){
        Eigen::MatrixXd temp_image_channel(height, width);
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                temp_image_channel(i, j) = image_data.get()[i * width * channels + j * channels + c];
            }
        }
        // Store the channel data
        in_image.push_back(std::move(temp_image_channel));
    }
    return true;
}

// Save an image to a file in PNG format using the STB library
bool EdgeDetector::saveImage(std::string filename, ImageType image_type){
    // Check if there is image data to save
    if(!in_image.size()){
        std::cerr << "No image data available" << std::endl;
        return false;
    }

    int saveChannels = 0; // Determine the number of channels to save based on the image type
    std::vector<Eigen::MatrixXd> image; // Placeholder for image data
    
    // Set the number of channels to save based on the image type
    switch(image_type){
        case ImageType::COLOR:
            saveChannels = 3; // Color images have 3 channels
            image = in_image; // Use the original image data
            break;
        case ImageType::GRAYSCALE:
            saveChannels = 1; // Grayscale images have 1 channel
            // For grayscale, additional processing would be needed here to convert color images to grayscale
            break;
    }

    // Prepare the image data for saving
    std::vector<unsigned char> image_data(width * height * saveChannels);
    for(int c = 0; c < saveChannels; ++c){
        for(int i = 0; i < height; ++i){
            for(int j = 0; j < width; ++j){
                image_data[i * width * saveChannels + j * saveChannels + c] = static_cast<unsigned char>(image[c](i, j));
            }
        }
    }

    // Save the image data to a PNG file
    if (!stbi_write_png(filename.c_str(), width, height, saveChannels, image_data.data(), width * saveChannels)) {
        std::cerr << "Failed to save image" << std::endl;
        return false;
    }

    return true;
}

// Specifically for saving grayscale images derived from edge detection
bool EdgeDetector::saveEdgeImage(std::string filename, Eigen::MatrixXd edges){
    // Ensure there is image data to work with
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

// Convert the loaded image to grayscale
bool EdgeDetector::convertToGrayscale() {
    // Ensure the image has the correct number of channels for conversion
    if (channels >= 3) { // For color images
        // Apply standard formula to convert to grayscale
        gray_image = 0.2989 * in_image[0] + 0.5870 * in_image[1] + 0.1140 * in_image[2];
    } else if (channels == 1) { // For already grayscale images
        gray_image = in_image[0]; // Directly use the single channel
    } else {
        std::cerr << "Unsupported number of channels: " << channels << std::endl;
        return false;
    }
    return true;
}

// Apply the specified edge detection algorithm to the image and return the result
Eigen::MatrixXd EdgeDetector::applyDetector(DetectorType detector_type, GradientType direction){
    // Convert the image to grayscale before detecting edges
    this->convertToGrayscale();
    
    // Apply the chosen edge detection filter
    switch (detector_type){
        case DetectorType::SOBEL:
            // The function kernel_detector is assumed to apply the Sobel filter
            return kernel_detector(detector_type, direction);
        case DetectorType::PREWITT:
            // Similarly for Prewitt filter
            return kernel_detector(detector_type, direction);
        case DetectorType::ROBERTSCROSS:
            // And for Roberts Cross filter
            return kernel_detector(detector_type, direction);
    }
}

// Processes the edges using the provided kernel
void EdgeDetector::kernel_processor(Eigen::MatrixXd& edges, Eigen::MatrixXd kernel){
    // Size of the kernel, assuming it's square
    int n_kernel = kernel.rows();
    
    // Apply the kernel to each pixel (except the border pixels)
    for(int i = 1; i < height - 1; ++i){
        for(int j = 1; j < width - 1; ++j){
            // Perform convolution operation
            edges(i, j) = (kernel.array() * gray_image.block(i - 1, j - 1, n_kernel, n_kernel).array()).sum();
        }
    }
}

// Chooses and applies the kernel based on the detector type and gradient direction
Eigen::MatrixXd EdgeDetector::kernel_detector(DetectorType detector_type, GradientType direction){
    // Define kernels for X and Y directions
    Eigen::MatrixXd Gx(3,3);
    Eigen::MatrixXd Gy;
    
    // Placeholder for edge data
    Eigen::MatrixXd edges(height, width);

    // Set up the kernels based on the detector type
    switch (detector_type){
        case DetectorType::SOBEL: 
            // Sobel X and Y kernels
            Gx << -1, 0, 1,
                  -2, 0, 2,
                  -1, 0, 1;
            Gy = Gx.transpose(); // The Y kernel is the transpose of the X kernel
            break;
        case DetectorType::PREWITT:
            // Prewitt X and Y kernels
            Gx << -1, 0, 1,
                  -1, 0, 1,
                  -1, 0, 1;
            Gy = Gx.transpose(); // The Y kernel is the transpose of the X kernel
            break;
        case DetectorType::ROBERTSCROSS:
            // Roberts Cross kernels
            Gx = Eigen::MatrixXd(2,2);
            Gx << 1, 0,
                  0, -1;
            Gy = Gx.rowwise().reverse().transpose(); // Reverse the rows and transpose for the Y kernel
            break;
    }
     
    // Apply the kernel(s) based on the desired gradient direction
    switch (direction)
    {
        case GradientType::X:
            kernel_processor(edges, Gx); // Apply X kernel
            break;
        case GradientType::Y:
            kernel_processor(edges, Gy); // Apply Y kernel
            break;
        case GradientType::MAG:
            // For magnitude, apply both kernels and combine the results
            Eigen::MatrixXd edges_x(height, width);
            kernel_processor(edges_x, Gx); // Apply X kernel

            Eigen::MatrixXd edges_y(height, width);
            kernel_processor(edges_y, Gy); // Apply Y kernel

            // Combine the X and Y gradients to get the edge magnitude
            edges = (edges_x.array().square() + edges_y.array().square()).sqrt();
            break;
    }

    return edges;
}
