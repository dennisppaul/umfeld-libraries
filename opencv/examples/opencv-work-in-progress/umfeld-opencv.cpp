#include "Umfeld.h"
#include "UmfeldOpenCV.h"

using namespace umfeld;

#include <opencv2/opencv.hpp>

int test_retrieve_contour_inline() {
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


void settings() {
    size(1024, 768);
}

void setup() {
    test_capture_frame();
}

void draw() {
    background(1);
    fill(0);
    noStroke();
}

void mousePressed() {
    test_retrieve_contour();
}

void keyPressed() {
    test_retrieve_contour_inline();
}