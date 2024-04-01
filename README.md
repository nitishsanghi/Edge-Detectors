# Edge Detection Library

The Edge Detection Library is a versatile C++ toolkit designed to facilitate the detection of edges in images. Leveraging the power of Eigen for matrix operations and OpenCV for comprehensive image processing, this library currently supports a suite of Sobel edge detection methods. It's built with extensibility in mind, making it an ideal starting point for implementing and experimenting with various edge detection algorithms.

In the EdgeDetector library, three primary edge detection algorithms are supported: Sobel, Prewitt, and Roberts Cross. Each of these algorithms has unique characteristics and is suitable for different applications. Understanding the distinctions between them can help in selecting the most appropriate algorithm for a given image processing task. Here's a breakdown of each detector:

### Sobel Detector

The Sobel detector is a popular edge detection algorithm that uses convolution with Sobel kernels to emphasize edges. The method calculates the gradient magnitude of an image at each pixel, which corresponds to the strength of the edges. This is achieved using two 3x3 convolution kernels: one estimating the gradient in the x-direction (horizontal) and the other in the y-direction (vertical). The overall magnitude of the gradient is obtained by combining these two gradients, typically using the Euclidean norm. The Sobel detector is particularly effective at emphasizing edges while reducing noise compared to simpler methods like the Roberts Cross operator.

### Prewitt Detector

The Prewitt detector is similar to the Sobel detector in its approach to edge detection, using convolution with kernels to estimate the gradient of the image intensity at each pixel. However, the Prewitt kernels are slightly different, providing a simpler and more isotropic response to edges in different directions. Like the Sobel, Prewitt uses two 3x3 convolution kernels for horizontal and vertical edge detection, and the gradient magnitude is calculated as a combination of these two gradients. The Prewitt operator is good at detecting vertical and horizontal edges but might be slightly less effective at suppressing noise compared to the Sobel operator.

### Roberts Cross Detector

The Roberts Cross operator is one of the earliest and simplest edge detection algorithms. It calculates the gradient of an image using a pair of 2x2 convolution kernels. These kernels are designed to calculate a simple approximation of the gradient by computing the difference between diagonally adjacent pixels. Because of its simplicity, the Roberts Cross detector is very fast but is more sensitive to noise. It is best used in applications where speed is more critical than accuracy, or in images with low levels of noise.

### Choosing the Right Detector

- **Sobel**: Best for general use, especially when a balance between edge emphasis and noise reduction is needed. It provides a good compromise between detecting edge strength and direction.
- **Prewitt**: Similar to Sobel but might be chosen for its slightly different edge response characteristics. It can be more isotropic, meaning it treats all directions of edges more equally.
- **Roberts Cross**: Ideal for quick calculations in less noisy images or when the computational simplicity is a priority. Not as robust as Sobel or Prewitt in the presence of noise.

In conclusion, the choice of edge detector depends on the specific requirements of the application, such as the need for speed, the tolerance for noise, and the importance of detecting edges of varying orientations. The EdgeDetector library's support for multiple algorithms provides flexibility for users to experiment with and select the most suitable detector for their needs.

## Features

- **Future-Proof**: Architecturally designed to support additional edge detection algorithms.
- **Performance Optimized**: Uses Eigen for efficient matrix computations, ensuring high performance.
- **User-Friendly**: Offers a straightforward API for loading, processing, and saving images.

## Getting Started

### Prerequisites

Before you begin, ensure you have installed:

- CMake (Version 3.14 or newer)
- OpenCV (Version 4.0 or newer)
- Eigen (Version 3.3 or newer)

### Installation

1. **Clone the Repository**

   Start by cloning the repository to your local machine:

   ```sh
   git clone https://github.com/<your-username>/EdgeDetectionLibrary.git
   cd EdgeDetectionLibrary
   ```

2. **Build the Project**

   Use CMake to build the project:

   ```sh
   mkdir build && cd build
   cmake ..
   make
   ```

### Quick Start

To get up and running with the Edge Detection Library, follow these steps:

1. **Create an Instance of `EdgeDetector`**

   ```cpp
   #include "edge_detector.hpp"

   EdgeDetector detector;
   ```

2. **Load an Image**

   ```cpp
   detector.loadImage("path/to/your/image.png");
   ```

3. **Apply Edge Detection**

   ```cpp
   auto edges = detector.applyDetector(EdgeDetector::DetectorType::SOBEL);
   ```

4. **Save the Detected Edges**

   ```cpp
   detector.saveEdgeImage("path/to/save/edges.png", edges);
   ```

## Documentation

For more detailed documentation on each class and method, refer to the `docs` directory in the repository.

## Contributing

We welcome contributions from the community, whether it's adding new features, improving documentation, or reporting bugs. Please follow these steps to contribute:

1. Fork the repository.
2. Create a new branch for your feature (`git checkout -b feature/YourFeature`).
3. Commit your changes (`git commit -am 'Add some YourFeature'`).
4. Push to the branch (`git push origin feature/YourFeature`).
5. Open a pull request.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.
