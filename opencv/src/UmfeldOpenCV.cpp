#include <iostream>
#include <opencv2/opencv.hpp>
#include <iomanip>
#include <cmath>

#include "UmfeldOpenCV.h"

struct Params {
    int    blur           = 5;    // odd >=1
    int    thresh         = 100;  // 0..255 (used if not using Otsu)
    bool   useOtsu        = true; // Otsu auto threshold
    bool   lightOnDark    = true; // true: light blobs on dark background
    double minArea        = 50.0; // px^2
    double maxArea        = 1e9;  // px^2
    double minCircularity = 0.0;  // 0..1
};

static double circularity(double area, double perimeter) {
    if (perimeter <= 0.0) {
        return 0.0;
    }
    return 4.0 * CV_PI * area / (perimeter * perimeter);
}

int blob_detect(int argc, char** argv) {
    // How it works & what to tweak
    // - UI trackbars let you switch polarity (light vs dark blobs), toggle Otsu, adjust blur, threshold, and size/circularity filters live.
    // - Stats per blob:
    // - centroid: image moments (m10/m00, m01/m00)
    // - area: contourArea
    // - perimeter: arcLength
    // - circularity: 4πA / P² (1.0 is a perfect circle)
    // - equivalent diameter: sqrt(4A/π)
    // - bounding box: boundingRect
    // - If blobs are merging, increase blur slightly, then a small morphological open is already applied; you can increase the kernel size to separate blobs more.
    // - If you specifically need SimpleBlobDetector, you can swap the contour block for SimpleBlobDetector and compute centroid/area from KeyPoint, but true circularity needs contours—this version gives you richer metrics.

    int camIndex = 0;
    if (argc >= 2) {
        camIndex = std::atoi(argv[1]);
    }

    Params           P;
    cv::VideoCapture cap(camIndex);
    if (!cap.isOpened()) {
        std::cerr << "could not open camera index " << camIndex << "\n";
        return 1;
    }

    const std::string win = "Blob Detection (press q to quit)";
    cv::namedWindow(win, cv::WINDOW_NORMAL);

    cv::createTrackbar("Blur(odd)", win, &P.blur, 31);
    int useOtsuTB = P.useOtsu ? 1 : 0;
    cv::createTrackbar("Use Otsu", win, &useOtsuTB, 1);
    cv::createTrackbar("Thresh", win, &P.thresh, 255);
    int lightOnDarkTB = P.lightOnDark ? 1 : 0;
    cv::createTrackbar("Light Blobs", win, &lightOnDarkTB, 1);

    int minAreaTB = static_cast<int>(P.minArea);
    int maxAreaTB = 50000;
    cv::createTrackbar("Min Area", win, &minAreaTB, 200000);
    cv::createTrackbar("Max Area", win, &maxAreaTB, 500000);

    int minCircTB = static_cast<int>(P.minCircularity * 100.0);
    cv::createTrackbar("Min Circ(%)", win, &minCircTB, 100);

    cv::Mat frame, gray, blurred, bin, vis;
    double  fps      = 0.0;
    auto    tickPrev = cv::getTickCount();

    while (true) {
        if (!cap.read(frame) || frame.empty()) {
            break;
        }

        // sync UI params
        P.useOtsu        = (useOtsuTB != 0);
        P.lightOnDark    = (lightOnDarkTB != 0);
        P.minArea        = std::max(1, minAreaTB);
        P.maxArea        = std::max(static_cast<int>(P.minArea) + 1, maxAreaTB);
        P.minCircularity = std::max(0, std::min(100, minCircTB)) / 100.0;
        if (P.blur % 2 == 0) {
            P.blur += 1;
        }

        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        if (P.blur > 1) {
            cv::GaussianBlur(gray, blurred, cv::Size(P.blur, P.blur), 0);
        } else {
            blurred = gray;
        }

        int threshType = P.lightOnDark ? cv::THRESH_BINARY : cv::THRESH_BINARY_INV;
        if (P.useOtsu) {
            cv::threshold(blurred, bin, 0, 255, threshType | cv::THRESH_OTSU);
        } else {
            cv::threshold(blurred, bin, P.thresh, 255, threshType);
        }

        // optional cleanup to split touching blobs a bit
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
        cv::morphologyEx(bin, bin, cv::MORPH_OPEN, kernel);

        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i>              hierarchy;
        cv::findContours(bin, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

        vis    = frame.clone();
        int id = 0;

        // fps
        auto   tickNow = cv::getTickCount();
        double dt      = (tickNow - tickPrev) / cv::getTickFrequency();
        tickPrev       = tickNow;
        fps            = (dt > 0.0) ? (1.0 / dt) : fps;

        // stats output header
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "frame: blobs=";

        int kept = 0;
        for (const auto& c: contours) {
            double area = cv::contourArea(c);
            if (area < P.minArea || area > P.maxArea) {
                continue;
            }

            double perimeter = cv::arcLength(c, true);
            double circ      = circularity(area, perimeter);
            if (circ < P.minCircularity) {
                continue;
            }

            cv::Moments m = cv::moments(c);
            if (std::abs(m.m00) < 1e-6) {
                continue;
            }
            cv::Point2f centroid(static_cast<float>(m.m10 / m.m00),
                                 static_cast<float>(m.m01 / m.m00));

            double   equivDiameter = std::sqrt(4.0 * area / CV_PI);
            cv::Rect bbox          = cv::boundingRect(c);

            // draw
            cv::Scalar color(0, 255, 0);
            cv::drawContours(vis, std::vector<std::vector<cv::Point>>{c}, -1, color, 2);
            cv::circle(vis, centroid, 3, cv::Scalar(0, 0, 255), cv::FILLED);
            cv::rectangle(vis, bbox, cv::Scalar(255, 0, 0), 2);

            std::ostringstream label;
            label << "#" << id
                  << " A=" << static_cast<int>(area)
                  << " C=" << std::setprecision(2) << circ
                  << " D=" << std::setprecision(1) << equivDiameter;
            cv::putText(vis, label.str(), bbox.tl() + cv::Point(0, -5),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1, cv::LINE_AA);

            // log to stdout
            std::cout << (kept == 0 ? "" : ",")
                      << "#" << id
                      << "(cx=" << centroid.x
                      << ", cy=" << centroid.y
                      << ", area=" << area
                      << ", perim=" << perimeter
                      << ", circ=" << circ
                      << ", eq_diam=" << equivDiameter
                      << ", bbox=" << bbox.x << "x" << bbox.y
                      << "+" << bbox.width << "x" << bbox.height << ")";
            ++id;
            ++kept;
        }
        std::cout << " | fps=" << fps << "\n";

        cv::putText(vis, "fps: " + std::to_string(static_cast<int>(fps)),
                    {10, 25}, cv::FONT_HERSHEY_SIMPLEX, 0.7, {0, 255, 0}, 2, cv::LINE_AA);
        cv::imshow(win, vis);

        int key = cv::waitKey(1);
        if (key == 'q' || key == 27) {
            break;
        }
    }

    return 0;
}

int blob_detection_simple(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <image_path>\n";
        return 1;
    }

    cv::Mat img = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
    if (img.empty()) {
        std::cerr << "failed to load image\n";
        return 1;
    }

    // optional: denoise a bit
    cv::Mat work;
    cv::GaussianBlur(img, work, cv::Size(5, 5), 0);

    // set up blob detector params
    cv::SimpleBlobDetector::Params p;
    p.minThreshold  = 10;  // intensity threshold sweep start
    p.maxThreshold  = 200; // intensity threshold sweep end
    p.thresholdStep = 10;
    // p.filterByInertia = true;
    // p.minInertiaRatio = 0.1f;

    p.filterByArea = true;
    p.minArea      = 30.0f; // adjust for your scale
    p.maxArea      = 5000.0f;

    p.filterByCircularity = false; // enable if blobs are round-ish
    p.minCircularity      = 0.7f;

    p.filterByConvexity = false;
    p.minConvexity      = 0.8f;

    p.filterByInertia = false; // enable for elongated/round constraints
    p.minInertiaRatio = 0.1f;

    p.filterByColor = true; // detect light blobs on dark background
    p.blobColor     = 255;  // set 0 to detect dark blobs

#if CV_MAJOR_VERSION >= 3
    cv::Ptr<cv::SimpleBlobDetector> detector = cv::SimpleBlobDetector::create(p);
#else
    cv::Ptr<cv::SimpleBlobDetector> detector = new cv::SimpleBlobDetector(p);
#endif

    std::vector<cv::KeyPoint> keypoints;
    detector->detect(work, keypoints);

    // draw results
    cv::Mat color;
    cv::cvtColor(img, color, cv::COLOR_GRAY2BGR);
    cv::drawKeypoints(color, keypoints, color, cv::Scalar(0, 255, 0),
                      cv::DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    std::cout << "blobs detected: " << keypoints.size() << "\n";
    cv::imwrite("blobs.png", color);

    // optional: show a window
    // cv::imshow("blobs", color);
    // cv::waitKey(0);

    return 0;
}

void test_capture_frame() {
    std::cout << "test_capture_frame" << std::endl;

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

int test_retrieve_contour() {
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
