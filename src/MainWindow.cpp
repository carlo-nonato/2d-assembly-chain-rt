#include "MainWindow.hpp"

// #include "ConveyorBelt.hpp"
#include "Simulation.hpp"
#include "Controller.hpp"
#include "Robot.hpp"

#include <QDoubleSpinBox>
#include <QGraphicsView>
#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QTimer>


#include <opencv4/opencv2/highgui/highgui.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <cmath>

#include <iostream>

MainWindow::MainWindow() {

    m_simulation = new Simulation();
    
    m_controller = new Controller(m_simulation);
    
    QGraphicsView *view = new QGraphicsView(m_simulation);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setRenderHint(QPainter::Antialiasing);
    view->setSizePolicy(QSizePolicy::MinimumExpanding,
                        QSizePolicy::MinimumExpanding);

    m_anomalyCamera = new QLabel();
    m_stackingCamera = new QLabel();

    // QDoubleSpinBox *speedSpinBox = new QDoubleSpinBox();
    // speedSpinBox->setValue(m_simulation->conveyorBelt()->speed());
    // connect(speedSpinBox, SIGNAL(valueChanged(double)),
    //         m_simulation->conveyorBelt(), SLOT(setSpeed(double)));

    QPushButton *startButton = new QPushButton("Start");
    connect(startButton, &QPushButton::clicked,
            m_controller, &Controller::start);

    QPushButton *captureButton = new QPushButton("CAPTURE FRAME");
    connect(captureButton, &QPushButton::clicked,
            this, &MainWindow::doRec);


    // QPushButton *createItemButton = new QPushButton("Create item");
    // connect(createItemButton, &QPushButton::clicked,
    //         m_simulation, &Simulation::createItem);

    QPushButton *arGrabButton = new QPushButton("Grab");
    connect(arGrabButton, &QPushButton::clicked,
            m_simulation->anomalyRobot(), &Robot::grab);
    QPushButton *arRotateToEndButton = new QPushButton("Rotate to end");
    connect(arRotateToEndButton, &QPushButton::clicked,
            m_simulation->anomalyRobot(), &Robot::rotateToEnd);
    QPushButton *arRotateToStartButton = new QPushButton("Rotate to start");
    connect(arRotateToStartButton, &QPushButton::clicked,
            m_simulation->anomalyRobot(), &Robot::rotateToStart);
    QPushButton *arReleaseButton = new QPushButton("Release");
    connect(arReleaseButton, &QPushButton::clicked,
            m_simulation->anomalyRobot(), &Robot::release);
    QGroupBox *arControls = new QGroupBox("Anomaly Robot");

    QVBoxLayout *arControlsLayout = new QVBoxLayout();
    arControlsLayout->addWidget(arGrabButton);
    arControlsLayout->addWidget(arRotateToEndButton);
    arControlsLayout->addWidget(arRotateToStartButton);
    arControlsLayout->addWidget(arReleaseButton);
    arControls->setLayout(arControlsLayout);

    QPushButton *srGrabButton = new QPushButton("Grab");
    connect(srGrabButton, &QPushButton::clicked,
            m_simulation->stackingRobot(), &Robot::grab);
    QPushButton *srRotateToEndButton = new QPushButton("Rotate to end");
    connect(srRotateToEndButton, &QPushButton::clicked,
            m_simulation->stackingRobot(), &Robot::rotateToEnd);
    QPushButton *srRotateToStartButton = new QPushButton("Rotate to start");
    connect(srRotateToStartButton, &QPushButton::clicked,
            m_simulation->stackingRobot(), &Robot::rotateToStart);
    QPushButton *srReleaseButton = new QPushButton("Release");
    connect(srReleaseButton, &QPushButton::clicked,
            m_simulation->stackingRobot(), &Robot::release);
    QGroupBox *srControls = new QGroupBox("Stacking Robot");

    QVBoxLayout *srControlsLayout = new QVBoxLayout();
    srControlsLayout->addWidget(srGrabButton);
    srControlsLayout->addWidget(srRotateToEndButton);
    srControlsLayout->addWidget(srRotateToStartButton);
    srControlsLayout->addWidget(srReleaseButton);
    srControls->setLayout(srControlsLayout);

    QGridLayout *controlLayout = new QGridLayout();
    controlLayout->addWidget(new QLabel("Conveyor belt speed"), 0, 0);
    // controlLayout->addWidget(speedSpinBox, 0, 1);
    controlLayout->addWidget(startButton, 1, 0, 1, 2);
    controlLayout->addWidget(captureButton, 2, 0, 1, 2);
    // controlLayout->addWidget(createItemButton, 1, 0, 1, 2);
    // controlLayout->addWidget(arControls, 2, 0, 1, 2);
    // controlLayout->addWidget(srControls, 3, 0, 1, 2);
    // controlLayout->setRowStretch(4, 1);*/

    QGridLayout *layout = new QGridLayout();

    layout->addWidget(view, 0, 0, 3, 1);
    layout->addLayout(controlLayout, 0, 1);
    layout->addWidget(m_anomalyCamera, 1, 1, Qt::AlignCenter);
    layout->addWidget(m_stackingCamera, 2, 1, Qt::AlignCenter);

    QWidget *centralWidget = new QWidget();
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    QTimer *timer = new QTimer();
    connect(timer, &QTimer::timeout, this, &MainWindow::updateCameras);
    timer->start(100);

    m_controller->start();

    
}

/**
 * Helper function to display text in the center of a contour
 */
void setLabel(cv::Mat& im, const std::string label, std::vector<cv::Point>& contour)
{
	int fontface = cv::FONT_HERSHEY_SIMPLEX;
	double scale = 0.4;
	int thickness = 1;
	int baseline = 0;

	cv::Size text = cv::getTextSize(label, fontface, scale, thickness, &baseline);
	cv::Rect r = cv::boundingRect(contour);

	cv::Point pt(r.x + ((r.width - text.width) / 2), r.y + ((r.height + text.height) / 2));
	cv::rectangle(im, pt + cv::Point(0, baseline), pt + cv::Point(text.width, -text.height), CV_RGB(255,255,255), cv::FILLED);
	cv::putText(im, label, pt, fontface, scale, CV_RGB(0,0,0), thickness, 8);
}

/**
 * Helper function to find a cosine of angle between vectors
 * from pt0->pt1 and pt0->pt2
 */
static double angle(cv::Point pt1, cv::Point pt2, cv::Point pt0)
{
	double dx1 = pt1.x - pt0.x;
	double dy1 = pt1.y - pt0.y;
	double dx2 = pt2.x - pt0.x;
	double dy2 = pt2.y - pt0.y;
	return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// Convert QT image to opencv Mat
cv::Mat qimage2mat(const QImage& qimage) {
    cv::Mat mat = cv::Mat(qimage.height(), qimage.width(), CV_8UC4, (uchar*)qimage.bits(), qimage.bytesPerLine());
    cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
    int from_to[] = { 0,0,  1,1,  2,2 };
    cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
    return mat2;
};

QImage MainWindow::doRec() {
    QImage frame = m_simulation->frameFromCamera(10, 30, 200, 250);
            
    cv::Mat src = qimage2mat(frame);        

    if (src.empty())
        return QImage((uchar*) src.data, src.cols, src.rows, src.step, QImage::Format_RGB888);

    // Adjust contrast and brightness for better detection
    // (avoid problems with conveyor belt texture)
    double alpha = 1.0; // Simple contrast control
    int beta = -100;    // Simple brightness control

    for( int y = 0; y < src.rows; y++ ) {
        for( int x = 0; x < src.cols; x++ ) {
            for( int c = 0; c < src.channels(); c++ ) {
                src.at<cv::Vec3b>(y,x)[c] =
                cv::saturate_cast<uchar>( alpha*src.at<cv::Vec3b>(y,x)[c] + beta );
            }
        }
    }

    // Convert to grayscale
    cv::Mat gray;
    cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);

    // We can use low threshold value thanks to contrast and brightness adjustment        
    cv::Mat thresh;
    cv::threshold(gray, thresh, 3, 255, cv::THRESH_BINARY);

    // Different methods to get threshold
    //cv::Mat output;
    //cv::inRange(src, cv::Scalar(110, 150, 150), cv::Scalar(130, 255, 255), thresh);
    //cv::imshow("output", output);

    /// Detect edges using canny
    //cv::Mat canny_output;
    //Canny(gray, thresh, 255, 255, 3);


    // Find contours
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(thresh.clone(), contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point> approx;
    cv::Mat dst = src.clone();

    std::vector<cv::Rect> boundRect( contours.size() );

    for (int i = 0; i < contours.size(); i++)
    {
        // Approximate contour with accuracy proportional
        // to the contour perimeter
        // 0.015 experimental value
        cv::approxPolyDP(cv::Mat(contours[i]), approx, cv::arcLength(cv::Mat(contours[i]), true)*0.015, true);

        cv::Scalar color = cv::Scalar(250, 255, 50);                
        
        boundRect[i] = cv::boundingRect(contours[i]);

        // Skip small (area < 100) or non-convex objects 
        if (std::fabs(cv::contourArea(contours[i])) < 100 || !cv::isContourConvex(approx))
            continue;

        if (approx.size() > 2) {
            cv::drawContours(dst, contours, i, color, 3);
            cv::drawContours(dst, cv::Mat(approx), -1, cv::Scalar(0, 20, 250), 6);
        }

        if (approx.size() == 3)
        {
            setLabel(dst, "TRIANGOLO", contours[i]);
        }
        else if (approx.size() >= 4 && approx.size() <= 6)
        {
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
                if (boundRect[i].width == boundRect[i].height)
                    setLabel(dst, "QUADRATO", contours[i]);
                else
                    setLabel(dst, "RETTANGOLO", contours[i]);                  
            }
            else if (vtc == 5 && mincos >= -0.34 && maxcos <= -0.27)                                
                setLabel(dst, "PENTAGONO", contours[i]);
            else if (vtc == 6 && mincos >= -0.55 && maxcos <= -0.45)                   
                setLabel(dst, "ESAGONO", contours[i]);
            else                                
                setLabel(dst, "ELLISSE", contours[i]);
        }
        else
        {
            // Detect and label circles
            //double area = cv::contourArea(contours[i]);
            //cv::Rect r = cv::boundingRect(contours[i]);
            //int radius = r.width / 2;

            if (boundRect[i].width == boundRect[i].height)
                setLabel(dst, "CERCHIO", contours[i]);    
            else
                setLabel(dst, "ELLISSE", contours[i]);  
        }               
    }
    //cv::imshow("thresh", thresh);
    //cv::imshow("src", src);
    //cv::imshow("dst", dst);
    
    return QImage((uchar*) dst.data, dst.cols, dst.rows, dst.step, QImage::Format_RGB888);

}

void MainWindow::updateCameras() {                
    m_anomalyCamera->setPixmap(QPixmap::fromImage(doRec()));

    QImage frame = m_simulation->frameFromCamera(10, 230, 200, 250);
    m_stackingCamera->setPixmap(QPixmap::fromImage(frame));    
}
