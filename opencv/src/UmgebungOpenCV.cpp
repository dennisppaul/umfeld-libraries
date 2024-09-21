#include <iostream>
#include <opencv2/opencv.hpp>

#include "UmgebungOpenCV.h"

void foobar() {
    std::cout << "hello world" << std::endl;

    // Initialize OpenCV VideoCapture to use the default camera
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Failed to open the camera." << std::endl;
        return;
    }

    // Capture a frame from the camera
    cv::Mat frame;
    cap >> frame; // Equivalent to cap.read(frame);

    if (frame.empty()) {
        printf("Failed to capture frame.\n");
        return;
    }

    // Convert the frame from BGR (OpenCV default) to RGB
    cv::Mat frameRGB;
    cv::cvtColor(frame, frameRGB, cv::COLOR_BGR2RGB);
    cv::Mat frameRGBA;
    cv::cvtColor(frame, frameRGBA, cv::COLOR_BGR2RGBA);

    // Get the frame dimensions
    const int width  = frameRGB.cols;
    const int height = frameRGB.rows;
    uint8_t*  pixels = frameRGB.data;

    std::cout << "Frame dimensions: " << width << "x" << height << std::endl;

    cap.release();
}

int retrieve_contour() {
    // Open the default camera (usually the first camera connected)
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "Error: Unable to open the camera." << std::endl;
        return -1;
    }

    // Create a window to display results
    cv::namedWindow("Contours", cv::WINDOW_AUTOSIZE);

    while (true) {
        cv::Mat frame;
        // Capture a new frame from the camera
        cap >> frame;

        if (frame.empty()) {
            std::cerr << "Error: Frame capture failed." << std::endl;
            break;
        }

        // Convert the frame to grayscale
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Apply Gaussian blur to reduce noise and improve contour detection
        cv::GaussianBlur(gray, gray, cv::Size(5, 5), 0);

        // Perform edge detection using Canny algorithm
        cv::Mat edges;
        cv::Canny(gray, edges, 50, 150);

        // Find contours
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(edges.clone(), contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

        // Draw contours on a blank image
        cv::Mat contourOutput = cv::Mat::zeros(edges.size(), CV_8UC3);
        cv::drawContours(contourOutput, contours, -1, cv::Scalar(0, 255, 0), 2);

        // Display the original frame and the contour image side by side
        cv::Mat display;
        cv::hconcat(frame, contourOutput, display);
        cv::imshow("Contours", display);

        // Break the loop if the user presses the 'q' key
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // Release the camera and destroy all windows
    cap.release();
    cv::destroyAllWindows();

    return 0;
}
