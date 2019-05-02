#include "cvutils.hpp"

cv::Mat QImage2Mat(const QImage &image) {
    cv::Mat mat = cv::Mat(image.height(), image.width(), CV_8UC4,
                          (uchar *) image.bits(), image.bytesPerLine());
    cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3);
    int from_to[] = {0, 0, 1, 1, 2, 2};
    cv::mixChannels(&mat, 1, &mat2, 1, from_to, 3);
    return mat2;
}

QImage Mat2QImage(const cv::Mat &mat) {
    return QImage((uchar*) mat.data, mat.cols, mat.rows, mat.step,
                  QImage::Format_RGB888).rgbSwapped();
}

void preprocess(cv::Mat &src) {
    double alpha = 1.0; // Simple contrast control
    int beta = -80; // Simple brightness control

    // Adjust contrast and brightness for better detection
    // (avoid problems with conveyor belt texture)
    for (int y = 0; y < src.rows; y++) {
        for (int x = 0; x < src.cols; x++) {
            for (int c = 0; c < src.channels(); c++) {
                src.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(
                    alpha*src.at<cv::Vec3b>(y, x)[c] + beta);
            }
        }
    }

    // Convert to grayscale
    cv::cvtColor(src, src, cv::COLOR_BGR2GRAY);
    // Convert to binary
    cv::threshold(src, src, 3, 255, cv::THRESH_BINARY);
}

Shape shapeDetection(std::vector<cv::Point> contour) {
    cv::Mat contourMat(contour);

    std::vector<cv::Point> approx;
    cv::approxPolyDP(contourMat, approx,
                     cv::arcLength(contourMat, true)*0.012, true);

    if (approx.size() < 2 || !cv::isContourConvex(approx))
        return Shape::None;

    cv::Rect boundingRect = cv::boundingRect(contour);

    if (approx.size() == 3)
        return Shape::Triangle;
    else if (approx.size() >= 4 && approx.size() <= 6) {
        // Number of vertices of polygonal curve
        int vtc = approx.size();

        // Get the cosines of all corners
        std::vector<double> cos;
        for (int j = 2; j < vtc+1; j++)
            cos.push_back(angle(approx[j%vtc], approx[j-2], approx[j-1]));

        // Sort ascending the cosine values
        std::sort(cos.begin(), cos.end());

        // Get the lowest and the highest cosine
        double mincos = cos.front();
        double maxcos = cos.back();
        
        // Use the degrees obtained above and the number of vertices
        // to determine the shape of the contour
        if (vtc == 4 && mincos >= -0.1 && maxcos <= 0.3) {
            if (boundingRect.width == boundingRect.height)
                return Shape::Square;
            else
                return Shape::Rect;
        }
        else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)                                
            return Shape::Pentagon;
        else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)                   
            return Shape::Hexagon;
    }
    else if (boundingRect.width == boundingRect.height)
        return Shape::Circle;
    return Shape::Ellipse;
}

double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0) {
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) +
            1e-10);
}

void setLabel(cv::Mat &image, std::string label,
              std::vector<cv::Point> &contour) {
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness,
                                    &baseline);
	cv::Rect r = cv::boundingRect(contour);

    cv::Point pt(r.x + ((r.width - text.width)/2),
                 r.y + ((r.height + text.height)/2));
    cv::rectangle(image, pt + cv::Point(0, baseline),
                  pt + cv::Point(text.width, -text.height),
                  CV_RGB(255, 255, 255), cv::FILLED);
    cv::putText(image, label, pt, fontface, scale, CV_RGB(0, 0, 0), thickness,
                8);
}