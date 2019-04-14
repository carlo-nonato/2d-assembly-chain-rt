#ifndef CVUTILS_HPP
#define CVUTILS_HPP

#include <QImage>
#include <QGraphicsItem>

#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>

enum Shape {None, Triangle, Square, Rect, Pentagon, Hexagon, Circle, Ellipse};

/** Convert a Qt image to an OpenCV image. */
cv::Mat QImage2Mat(const QImage &image);

/**
 * Preprocess the image by improving contrast and brightness for better
 * results in the object recognition phase.
 */
void preprocess(cv::Mat &image);

Shape shapeDetection(std::vector<cv::Point> contour);

/**
 * Helper function to find a cosine of angle between vectors
 * from pt0->pt1 and pt0->pt2.
 */
double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0);

/** Display text in the center of a contour. */
void setLabel(cv::Mat &image, std::string label,
              std::vector<cv::Point> &contour);

#endif // CVUTILS_HPP